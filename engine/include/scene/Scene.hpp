#pragma once

#include "nodes/Node.hpp"

namespace rle
{
    class Scene
    {
    private:
        static uint32_t next_id_;
        std::string name_;
        std::unique_ptr<Node> root_{nullptr};

    public:
        Scene();
        Scene(std::unique_ptr<Node> root);
        Scene(const std::string& name);
        Scene(std::unique_ptr<Node> root, const std::string& name);

        const std::string& GetName() const {return name_;}
        Node* Root() {return root_.get();}
        const Node* Root() const {return root_.get();}
    };
}
