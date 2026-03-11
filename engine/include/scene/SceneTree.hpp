#pragma once

#include "nodes/Node.hpp"

namespace rle
{
    class SceneTree
    {
    private:
        std::unique_ptr<Node> root_;

    public:
        Node* Root() {return root_.get();}
    };
}
