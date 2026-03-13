#pragma once

#include "nodes/3D/Node3D.hpp"

namespace rle
{
    class NodeMesh3D : public Node3D
    {
    private:
        Mesh mesh_{GenMeshCube(1.0f, 1.0f, 1.0f)};
        Material material_{LoadMaterialDefault()};
        
    protected:
        void Render3DInternal() override;

    public:
        ~NodeMesh3D();

        std::string GetTypeName() const override {return "NodeMesh3D";}

        Mesh& GetMesh() {return mesh_;}
        void SetMesh(const Mesh& mesh) {UnloadMesh(mesh_); mesh_ = mesh;}
        Material& GetMaterial() {return material_;}
        void SetMaterial(const Material& material) {RL_FREE(material_.maps); material_ = material;}
    };
}