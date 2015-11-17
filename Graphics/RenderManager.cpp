#include "RenderManager.h"

#include "ContextManager.h"
#include "MaterialManager.h"

void CRenderManager::SetCurrentCamera(const CCamera& _CurrentCamera)
{
	m_CurrentCamera = _CurrentCamera;
	Mat44f viewProj = m_CurrentCamera.GetView() * m_CurrentCamera.GetProjection();
	m_CullFrustum.Update(viewProj);
}

bool CRenderManager::AddRenderableObjectToRenderList(const CRenderableObject* _RenderableObject)
{
	// Alguien se atrebe a arreglar el frustum?

	//if (m_CullFrustum.SphereVisible(_RenderableObject->GetTransform().Position, _RenderableObject->GetBoundingRadius()))
	{
		if (m_CurrentRenderlistLength == m_RenderableObjects.size())
		{
			m_RenderableObjects.push_back(_RenderableObject);
		}
		else
		{
			m_RenderableObjects[m_CurrentRenderlistLength];
		}
		++m_CurrentRenderlistLength;
		return true;
	}
	//else
	//{
	//	return false;
	//}
}

void CRenderManager::Render(CContextManager* _Context, CMaterialManager* _MaterialManager)
{
	if (m_UseDebugCamera)
	{
		_Context->SetCamera(m_DebugCamera);
	}
	else
	{
		_Context->SetCamera(m_CurrentCamera);
	}

	// TODO crear un vector para objetos transparentes
	struct BlendedSubmesh
	{
	    const CRenderableVertexs* vertices;
	    Mat44f world;
	    const CMaterial* material;
	    const Vect3f& position;
	};
	std::vector<BlendedSubmesh> l_SubmeshesWithBlend;



	for (int i = 0; i < m_CurrentRenderlistLength; ++i)
	{
		const CRenderableObject* l_RenderableObject = m_RenderableObjects[i];
		const SPositionOrientation& l_Transform = l_RenderableObject->GetTransform();

		Mat44f world;
		world.SetFromPosAndAnglesYXZ(l_Transform.Position, l_Transform.Yaw, l_Transform.Pitch, l_Transform.Roll);
		_Context->SetWorldMatrix(world);

		for (int j = 0; j < l_RenderableObject->GetNumSubmeshes(); ++j)
		{
			const CRenderableObject::SSubmesh& l_Submesh = l_RenderableObject->GetSubmesh(j);
			const CMaterial* l_Material = _MaterialManager->GetMaterial(l_Submesh.material);
			// TODO no pintar el objeto, sino añadirlo a la lista l_SubmeshesWithBlend si tiene blend
			if (l_Material->HasBlending())
			{
				BlendedSubmesh l_bsm = {l_Submesh.vertices, world, l_Material, l_Transform.Position};
				l_SubmeshesWithBlend.push_back(l_bsm);
			}
			else
			{
				l_Material->SetShaderParameters(_Context);
				_Context->Draw(l_Submesh.vertices, l_Material->GetRasterizerState(), l_Material->GetDepthStencilState(), l_Material->GetBlendState());
			}
		}
	}

	// TODO: Ordenar objetos según la distáncia a la cámara
	// NOTA: El quicksort es más rápido que el buble sort cuando la lista tiene más de ~100 objetos. NO OS MATÉIS SI NO HACE FALTA.
	const Vect3f& l_CameraPosition = m_CurrentCamera.GetPosition();
	float l_DistanceSQ;
	for (int i = 0; i < l_SubmeshesWithBlend.size(); ++i)
	{
	    l_DistanceSQ = (l_SubmeshesWithBlend[i].position-l_CameraPosition).SquaredLength();
		
	}

	// TODO: Pintar objetos translúcidos
	for (int i = 0; i < l_SubmeshesWithBlend.size(); ++i)
	{
	    // TODO render
		l_SubmeshesWithBlend[i].material->SetShaderParameters(_Context);
		_Context->Draw(l_SubmeshesWithBlend[i].vertices, l_SubmeshesWithBlend[i].material->GetRasterizerState(), l_SubmeshesWithBlend[i].material->GetDepthStencilState(), l_SubmeshesWithBlend[i].material->GetBlendState());
	}

	m_CurrentRenderlistLength = 0;
}
