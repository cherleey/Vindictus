#include "Component_Manager.h"
#include "Component.h"

USING(Engine)
IMPLEMENT_SINGLETON(CComponent_Manager)

Engine::CComponent_Manager::CComponent_Manager(void)
: m_pMapComponent(NULL)
, m_iContainerSize(0)
{

}

Engine::CComponent_Manager::~CComponent_Manager(void)
{

}

HRESULT Engine::CComponent_Manager::Reserve_ComponentMgr(const _uint& iSize)
{
	if(NULL != m_pMapComponent)
	{
		MSG_BOX("Reserved");
		return E_FAIL;
	}

	m_pMapComponent = new MAPCOMPONENT[iSize];

	m_iContainerSize = iSize;

	return S_OK;
}

HRESULT Engine::CComponent_Manager::Add_Component(const _uint& iIndex, const _tchar* pComponentTag, CComponent* pComponent)
{
	CComponent*	pComponent_Find = Find_Component(iIndex, pComponentTag);

	if(NULL != pComponent_Find)
		return E_FAIL;

	m_pMapComponent[iIndex].insert(MAPCOMPONENT::value_type(pComponentTag, pComponent));

	return S_OK;
}

CComponent* Engine::CComponent_Manager::Clone_Component(const _uint& iIndex, const _tchar* pComponentTag)
{
	CComponent*	pComponent_Find = Find_Component(iIndex, pComponentTag);

	if(NULL == pComponent_Find)
		return NULL;

	return pComponent_Find->Clone();
}

void Engine::CComponent_Manager::Release_Component(const _uint& iIndex)
{
	for_each(m_pMapComponent[iIndex].begin(), m_pMapComponent[iIndex].end(), CRelease_Pair());

	m_pMapComponent[iIndex].clear();
}

CComponent* Engine::CComponent_Manager::Find_Component(const _uint& iIndex, const _tchar* pComponentTag)
{
	MAPCOMPONENT::iterator	iter = find_if(m_pMapComponent[iIndex].begin(), m_pMapComponent[iIndex].end(), CTag_Finder(pComponentTag));

	if(iter == m_pMapComponent[iIndex].end())
		return NULL;

	return iter->second;
}

void Engine::CComponent_Manager::Free(void)
{
	for (_ulong i = 0; i < m_iContainerSize; ++i)
	{
		for_each(m_pMapComponent[i].begin(), m_pMapComponent[i].end(), CRelease_Pair());
		m_pMapComponent[i].clear();
	}

	Engine::Safe_Delete_Array(m_pMapComponent);
}



