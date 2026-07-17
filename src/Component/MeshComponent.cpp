#include "MeshComponent.h"
#include "Material.h"
#include "Mesh.h"
#include "RenderQueue.h"
#include "GameObject.h"
#include "Engine.h"

namespace why
{
    MeshComponent::MeshComponent(const std::shared_ptr<Material>& material, const std::shared_ptr<Mesh>& mesh)
        : m_material(material), m_mesh(mesh)
    {

    }

    void MeshComponent::Update(float deltaTime)
    {
        if (!m_material || !m_mesh)
        {
            return;
        }

        RenderCommand command;
        command.material = m_material.get();
        command.mesh = m_mesh.get();
        command.modelMatrix = GetOwner()->GetWorldTransform();       
        SINGLETON_PTR(Engine)->GetRenderQueue()->Submit(command);
    }
}