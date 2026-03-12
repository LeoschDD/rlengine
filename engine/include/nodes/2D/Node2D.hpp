#pragma once

#include "nodes/Node.hpp"
#include "core/Types.hpp"

namespace rle
{
    class Node2D : public Node
    {
    private:
        Transform2D local_transform_;
        Transform2D global_transform_;
        bool transform_dirty_{true};

    protected:
        void MarkTransformDirty();
        void UpdateGlobalTransform();

        virtual void OnTransformChanged() {}
        virtual bool InheritsParentTransform() {return true;}
        void OnHierarchyChanged() override {MarkTransformDirty();}

    public:
        void SetTransform(Transform2D transform) {local_transform_ = transform; MarkTransformDirty();}
        void SetPosition(Vector2 position) {local_transform_.translation = position; MarkTransformDirty();}
        void SetRotation(float rotation) {local_transform_.rotation = rotation; MarkTransformDirty();}
        void SetScale(Vector2 scale) {local_transform_.scale = scale; MarkTransformDirty();}

        void Translate(Vector2 offset);
        void Rotate(float angle);

        Transform2D GetLocalTransform() const {return local_transform_;}
        Transform2D GetGlobalTransform();

        Vector2 GetRight();
        Vector2 GetUp();
    };
}
