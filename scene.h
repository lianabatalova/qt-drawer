#ifndef SCENE_H
#define SCENE_H

#include <memory>
#include <set>
#include <stack>
#include <vector>

#include <QAbstractGraphicsShapeItem>
#include <QDebug>
#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QKeyEvent>
#include <QMouseEvent>
#include <QtGlobal>

#include "command.h"
#include "shapes.h"

class Scene : public QGraphicsScene {
    Q_OBJECT
public:
    explicit Scene(QObject *parent = nullptr) : QGraphicsScene(parent) {}

    void RemoveItems() {
        for (auto& i : selected_items_) {
            i->RemoveFromScene(this);
        }
        selected_items_.clear();
        emit enableDeleteButton(selected_items_.size() != 0);
        ResetPreviousCommands();
    }

    void GroupElements() {
        CGroup *group = new CGroup();
        for (auto it = selected_items_.begin(); it != selected_items_.end(); ++it) {
            group->AddShape(*it);
        }
        UnselectAll();
        selected_items_.clear();
        emit enableGroupButton(selected_items_.size() > 0);
    }

    void UngroupElements() {
        if (selected_items_.size() != 1) return;
        auto group = dynamic_cast<CGroup*>(*selected_items_.begin());
        if (group != nullptr) {
            group->Disassemble();
            UnselectAll();
            delete group;
        }
        emit enableUngroupButton(IsOneSelectedGroup());
        ResetPreviousCommands();
    }

    void ReceiveFigure(ShapeType current_type) {
        selected_shape_type = current_type;
    }

    void Undo() {
        auto cmd = std::move(commands_.top());
        commands_.pop();
        cmd->Undo();
        emit enableUndoButton(!commands_.empty());
    }
signals:
    void enableGroupButton(bool enable);
    void enableUngroupButton(bool enable);
    void enableDeleteButton(bool enable);
    void enableUndoButton(bool enable);
protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override {
        auto x = event->scenePos().x();
        auto y = event->scenePos().y();
        QGraphicsItem *item = itemAt(event->scenePos(), QTransform());
        CItem* c_item = nullptr;
        if (added_items_.count(item)) {
            c_item = added_items_[item];
        }

        if (c_item == nullptr) {
            this->CreateItem(x, y);
            return;
        }
        auto shape = c_item->GetParent();

        Toggled(shape);
        emit enableGroupButton(selected_items_.size() > 1);
        emit enableUngroupButton(IsOneSelectedGroup());
        emit enableDeleteButton(selected_items_.size() != 0);
    }

    void ResetPreviousCommands() {
        while (!commands_.empty()) {
            commands_.pop();
        }
        emit enableUndoButton(false);
    }

    bool IsOneSelectedGroup() {
        return selected_items_.size() == 1 && IsGroup(*begin(selected_items_));
    }

    bool IsGroup(CShape* shape) {
        CGroup* group = dynamic_cast<CGroup*>(shape);
        return group != nullptr;
    }

    void keyPressEvent(QKeyEvent *keyEvent) override {
        MoveDirection dir = MoveDirection::kUnknown;

        if (keyEvent->key() == Qt::Key_Left) dir = MoveDirection::kLeft;
        if (keyEvent->key() == Qt::Key_Right) dir = MoveDirection::kRight;
        if (keyEvent->key() == Qt::Key_Down) dir = MoveDirection::kDown;
        if (keyEvent->key() == Qt::Key_Up) dir = MoveDirection::kUp;

        if (dir == MoveDirection::kUnknown) return;
        auto cmd = std::make_unique<Command::Move>(selected_items_, dir);
        cmd->Apply();
        commands_.push(std::move(cmd));
        emit enableUndoButton(true);
    }

    CItem* CreateItem(double x, double y) {
        double r = 20;
        CItem* item = nullptr;
        if (selected_shape_type == ShapeType::kCircle) {
            item = new CCircle(x-r, y-r, 2*r, 2*r);
        } else if (selected_shape_type == ShapeType::kSquare) {
            item = new CSquare(x-r, y-r, 2*r, 2*r);
        }
        this->addItem(item->GetPtr());
        added_items_[item->GetPtr()] = item;
        item->GetPtr()->setBrush(QBrush(ColorChoose()));
        return item;
    }

    QColor ColorChoose() {
        return colors_[next_color_id_++%colors_.size()];
    }

    void Toggled(CShape* shape) {
        if (selected_items_.count(shape)) Unselect(shape);
        else Select(shape);
    }

    void Select(CShape* shape) {
        shape->Select();
        selected_items_.insert(shape);
    }

    void Unselect(CShape* shape) {
        shape->Unselect();
        selected_items_.erase(shape);
    }

    void UnselectAll() {
        for (auto i : selected_items_) {
            i->Unselect();
        }
        selected_items_.clear();
    }

private:
    std::set<CShape*> selected_items_;
    std::stack<std::unique_ptr<Command>> commands_;
    std::unordered_map<QGraphicsItem*, CItem*> added_items_;
    std::vector<QColor> colors_ = {QColor("#b3d199"),QColor("#b9a2bf"),QColor("#a4c1fb"),
                                  QColor("#db839f"),QColor("#f2d9f3"), QColor("#8ec90e"),
                                  QColor("#695b8e")};
    int next_color_id_ = 0;
    ShapeType selected_shape_type = ShapeType::kCircle;
};

#endif // SCENE_H
