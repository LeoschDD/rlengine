#pragma once

#include "nodes/Node.hpp"

namespace rle
{
    class Scene
    {
    private:
        std::unique_ptr<Node> root_{nullptr};

    public:
        Scene();
        Scene(std::unique_ptr<Node> root);

        Node* Root() {return root_.get();}
        const Node* Root() const {return root_.get();}
    };
}
