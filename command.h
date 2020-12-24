#ifndef COMMAND_H
#define COMMAND_H

#include <set>

#include "shapes.h"

enum class MoveDirection {
    kLeft,
    kRight,
    kUp,
    kDown,
    kUnknown,
};

class Command {
public:
    class Move;
    class AddItem;
    virtual void Apply() = 0;
    virtual void Undo() = 0;
};

class Command::Move : public Command {
public:
    Move(std::set<CShape*> shapes, MoveDirection direction)
        : shapes_(shapes) {
        if (direction == MoveDirection::kLeft) dx_ = -kMoveBy;
        if (direction == MoveDirection::kRight) dx_ = kMoveBy;
        if (direction == MoveDirection::kUp) dy_ = -kMoveBy;
        if (direction == MoveDirection::kDown) dy_ = kMoveBy;
    }

    virtual void Apply() override {
        for (auto& shape : shapes_) {
            shape->Move(dx_, dy_);
        }
    }

    virtual void Undo() override {
        for (auto& shape : shapes_) {
            shape->Move(-dx_, -dy_);
        }
    }
private:
    int dx_ = 0;
    int dy_ = 0;
    static constexpr int kMoveBy = 10;
    std::set<CShape*> shapes_;
};

#endif // COMMAND_H
