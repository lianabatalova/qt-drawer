#ifndef SHAPES_H
#define SHAPES_H

#include <vector>

#include <QAbstractGraphicsShapeItem>
#include <QGraphicsEllipseItem>
#include <QGraphicsScene>

enum class ShapeType {
    kCircle,
    kSquare,
};

class CShape {
public:
    virtual void Select() = 0;
    virtual void Unselect() = 0;
    virtual void RemoveFromScene(QGraphicsScene* scene) = 0;
    virtual void Move(double dx, double dy) = 0;
    virtual ~CShape() {};

    CShape* parent = nullptr;
    CShape* GetParent() {
        if (parent == nullptr) return this;
        return parent->GetParent();
    }
};

class CItem : public CShape {
public:
    virtual void Select() override {
        item_->setPen(QPen(Qt::red,2));
    }
    virtual void Unselect() override {
        item_->setPen(QPen());
    }
    virtual void RemoveFromScene(QGraphicsScene* scene) override {
        scene->removeItem(item_.get());
    }
    virtual void Move(double dx, double dy) override {
        item_->moveBy(dx, dy);
    }
    auto GetPtr() const {
        return item_.get();
    }
protected:
    std::unique_ptr<QAbstractGraphicsShapeItem> item_;
};

class CCircle : public CItem {
public:
    CCircle(qreal x, qreal y, qreal width, qreal height, QGraphicsItem *parent = nullptr) {
        item_ = std::make_unique<QGraphicsEllipseItem>(x, y, width, height, parent);
    }
};

class CSquare : public CItem {
public:
    CSquare(qreal x, qreal y, qreal width, qreal height, QGraphicsItem *parent = nullptr) {
        item_ = std::make_unique<QGraphicsRectItem>(x, y, width, height, parent);
    }
};

class CGroup : public CShape {
public:
    void AddShape(CShape* shape) {
        shapes_.push_back(shape);
        shape->parent = this;
    }
    void Disassemble() {
        for (auto shape : shapes_) {
            shape->parent = nullptr;
        }
    }
    virtual void Select() override {
        for (auto& shape : shapes_) {
            shape->Select();
        }
    }
    virtual void Unselect() override {
        for (auto& shape : shapes_) {
            shape->Unselect();
        }
    }
    virtual void RemoveFromScene(QGraphicsScene* scene) override {
        for (auto& shape : shapes_) {
            shape->RemoveFromScene(scene);
        }
    }
    virtual void Move(double dx, double dy) override {
        for (auto& shape : shapes_) {
            shape->Move(dx, dy);
        }
    }
private:
    std::vector<CShape*> shapes_;
};

#endif // SHAPES_H
