#include "Layer.h"
#include "Component.h"
#include "GameObject.h"

USING(Engine)

Engine::CLayer::CLayer(void)
{

}

Engine::CLayer::~CLayer(void)
{

}

Engine::CComponent* Engine::CLayer::Get_Component(Engine::CGameObject::COMTYPE eType, const _tchar* pComponentTag, const _uint& iObjectIdx /*= 0*/)
{
	OBJECTLIST::iterator	iter = m_ObjectList.begin();	

	for (_uint i = 0; i < iObjectIdx; ++i)
		++iter;

	if(iter == m_ObjectList.end())
		return NULL;

	return (*iter)->Get_Component(eType, pComponentTag);
}

HRESULT Engine::CLayer::Ready_Layer(void)
{

	return S_OK;
}

_int Engine::CLayer::Update_Layer(const _float& fTimeDelta)
{
	OBJECTLIST::iterator	iter = m_ObjectList.begin();
	OBJECTLIST::iterator	iter_end = m_ObjectList.end();

	_int		iExitCode = 0;

	for (; iter != iter_end; ++iter)
	{
		iExitCode = (*iter)->Update_GameObject(fTimeDelta);
		if(iExitCode & 0x80000000)
			return iExitCode;
	}
	return 0;
}

_int Engine::CLayer::LastUpdate_Layer(const _float& fTimeDelta)
{
	OBJECTLIST::iterator	iter = m_ObjectList.begin();
	OBJECTLIST::iterator	iter_end = m_ObjectList.end();

	_int		iExitCode = 0;

	for (; iter != iter_end; ++iter)
	{
		iExitCode = (*iter)->LastUpdate_GameObject(fTimeDelta);
		if(iExitCode & 0x80000000)
			return iExitCode;
	}
	return 0;
}

CLayer* Engine::CLayer::Create(void)
{
	CLayer*	pInstance = new CLayer;

	if(FAILED(pInstance->Ready_Layer()))
	{
		MSG_BOX("CLayer Created Failed");
		Engine::Safe_Release(pInstance);
	}
	return pInstance;
}

void Engine::CLayer::Free(void)
{
	for_each(m_ObjectList.begin(), m_ObjectList.end(), CRelease_Single());

	m_ObjectList.clear();
}


