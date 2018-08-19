#include "Scene.h"
#include "Layer.h"

USING(Engine)

Engine::CScene::CScene(LPDIRECT3DDEVICE9 pGraphicDev)
: m_pGraphicDev(pGraphicDev)
{
	m_pGraphicDev->AddRef();
}

Engine::CScene::~CScene(void)
{

}

Engine::CComponent* Engine::CScene::Get_Component(const _tchar* pLayerTag, Engine::CGameObject::COMTYPE eType, const _tchar* pComponentTag, const _uint& iObjectIdx /*= 0*/)
{
	CLayer*	pLayer = Find_Layer(pLayerTag);
	if(NULL == pLayer)
		return NULL;

	return pLayer->Get_Component(eType, pComponentTag, iObjectIdx);
}

HRESULT Engine::CScene::Add_Layer(const _tchar* pLayerTag, CLayer* pLayer)
{
	CLayer*	pLayer_Find = Find_Layer(pLayerTag);

	if(NULL != pLayer_Find)	
		return E_FAIL;

	m_mapLayers.insert(MAPLAYERS::value_type(pLayerTag, pLayer));

	return S_OK;	
}

HRESULT Engine::CScene::Ready_Scene(void)
{

	return S_OK;
}

_int Engine::CScene::Update_Scene(const _float& fTimeDelta)
{
	MAPLAYERS::iterator	iter = m_mapLayers.begin();
	MAPLAYERS::iterator	iter_end = m_mapLayers.end();

	_int		iExitCode = 0;

	for (; iter != iter_end; ++iter)
	{
		iExitCode = iter->second->Update_Layer(fTimeDelta);
		if(iExitCode & 0x80000000)
			return iExitCode;
	}

	for (iter = m_mapLayers.begin(); iter != iter_end; ++iter)
	{
		iExitCode = iter->second->LastUpdate_Layer(fTimeDelta);
		if(iExitCode & 0x80000000)
			return iExitCode;
	}

	return 0;
}

CLayer* Engine::CScene::Find_Layer(const _tchar* pLayerTag)
{
	MAPLAYERS::iterator	iter = find_if(m_mapLayers.begin(), m_mapLayers.end(), CTag_Finder(pLayerTag));

	if(iter == m_mapLayers.end())
		return NULL;

	return iter->second;
}

void Engine::CScene::Free(void)
{
	Engine::Safe_Release(m_pGraphicDev);

	for_each(m_mapLayers.begin(), m_mapLayers.end(), CRelease_Pair());
	m_mapLayers.clear();
}


