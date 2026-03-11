#pragma once

#include "core/Headers.hpp"

namespace rle
{
    class Node
    {
    private:
        uint32_t id_;
        std::string name_;

        Node* parent_{nullptr};
        std::vector<std::unique_ptr<Node>> children_;

    private:
        void EnterTree();
        void ExitTree();
        void Input();
        void Update();
        void Render();

    protected:
        virtual void Serialize();
        virtual void Deserialize();

    public:
    };
}
