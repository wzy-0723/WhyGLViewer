#pragma once
#include <memory>
#include "Application.h"
#include "Material.h"
#include "Mesh.h"
#include "Scene.h"
namespace why
{
    class Game : public why::Application
    {
    public:
        bool Init() override;
        void Update(float deltaTime) override;
        void Destroy() override;

    private:
        Scene m_scene;
    };
}

