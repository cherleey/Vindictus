#include "Mesh.h"

USING(Engine)

Engine::CMesh::CMesh(LPDIRECT3DDEVICE9 pGraphicDev)
: m_isClone(false)
, m_pGraphicDev(pGraphicDev)
{
	m_pGraphicDev->AddRef();
}

CMesh::CMesh(const CMesh& rhs)
: m_isClone(true)
, m_pGraphicDev(rhs.m_pGraphicDev)
{
	m_pGraphicDev->AddRef();
}

Engine::CMesh::~CMesh(void)
{

}


void Engine::CMesh::Free(void)
{
	Engine::CComponent::Free();

	Engine::Safe_Release(m_pGraphicDev);
}

