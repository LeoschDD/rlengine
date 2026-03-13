#include "nodes/3D/NodeMesh3D.hpp"

void rle::NodeMesh3D::Render3DInternal()
{
    auto transform = this->GetGlobalTransformMatrix();
    DrawMesh(mesh_, material_, transform);
}

rle::NodeMesh3D::~NodeMesh3D()
{
    RL_FREE(material_.maps);
    UnloadMesh(mesh_);
}
