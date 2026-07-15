#pragma once
#include <memory>
#include "Application.h"
#include "Material.h"
#include "Mesh.h"

namespace why
{
    class Game : public why::Application
    {
    public:
        bool Init() override;
        void Update(const QEvent& e) override;
        void Destroy() override;

    private:
        why::Material m_material;
        std::unique_ptr<why::Mesh> m_mesh;
    };
}

