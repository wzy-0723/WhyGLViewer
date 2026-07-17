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
};