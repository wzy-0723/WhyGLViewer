#pragma once

#include "Material.h"
#include "Mesh.h"
#include "GameObject.h"


class TestObject : public why::GameObject
{
public:
    TestObject();
    void Update(float deltaTime) override;

private:
    why::Material m_material;
    std::shared_ptr<why::Mesh> m_mesh;
    float m_offsetX = 0.0f;
    float m_offsetY = 0.0f;
};