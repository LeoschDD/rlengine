#pragma once

#include "nodes/Node.hpp"

namespace rle
{
    class Node3D : public Node
    {
    private:
        Transform local_transform_ = {{0.0f, 0.0f, 0.0f}, QuaternionIdentity(), {1.0f, 1.0f, 1.0f}};
        Transform global_transform_ = {{0.0f, 0.0f, 0.0f}, QuaternionIdentity(), {1.0f, 1.0f, 1.0f}};
        bool transform_dirty_{true};

    protected:
        void MarkTransformDirty();
        void UpdateGlobalTransform();

        virtual void OnTransformChanged() {}
        virtual bool InheritsParentTransform() {return true;}
        void OnHierarchyChanged() override {MarkTransformDirty();}

    public:
        void SetTransform(Transform transform) {local_transform_ = transform; MarkTransformDirty();}
        void SetPosition(Vector3 position) {local_transform_.translation = position; MarkTransformDirty();}
        void SetRotation(Quaternion rotation) {local_transform_.rotation = rotation; MarkTransformDirty();}
        void SetScale(Vector3 scale) {local_transform_.scale = scale; MarkTransformDirty();}

        void Translate(Vector3 offset);
        void Rotate(Quaternion rotation);
        void Rotate(Vector3 axis, float angle);
        void RotateLocal(Quaternion rotation);
        void RotateLocal(Vector3 axis, float angle);
        void LookAt(Vector3 target, Vector3 up);

        Transform GetLocalTransform() const {return local_transform_;}
        Transform GetGlobalTransform();
        Matrix GetGlobalTransformMatrix();

        Vector3 GetForward();
        Vector3 GetRight();
        Vector3 GetUp();
    };
}
