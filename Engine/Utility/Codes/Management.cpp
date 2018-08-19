#include "Management.h"
#include "Scene.h"
#include "Renderer.h"
#include "Component_Manager.h"

USING(Engine)
IMPLEMENT_SINGLETON(CManagement)

Engine::CManagement::CManagement(void)
: m_pCurrentScene(NULL)
, m_pRenderer(NULL)
{

}

Engine::CManagement::~CManagement(void)
{

}

Engine::CComponent* Engine::CManagement::Get_Component(const _tchar* pLayerTag, Engine::CGameObject::COMTYPE eType, const _tchar* pComponentTag, const _uint& iObjectIdx/* = 0*/)
{
	if(NULL == m_pCurrentScene)
		return NULL;

	return m_pCurrentScene->Get_Component(pLayerTag, eType, pComponentTag, iObjectIdx);
}

Engine::CLayer* Engine::CManagement::Find_Layer(const _tchar* pLayerTag)
{
	return m_pCurrentScene->Find_Layer(pLayerTag);
}

HRESULT Engine::CManagement::Ready_Management(const _uint& iIndex, const _tchar* pRendererTag)
{
	m_pRenderer = (Engine::CRenderer*)CComponent_Manager::GetInstance()->Clone_Component(iIndex, pRendererTag);
	if(NULL == m_pRenderer)
		return E_FAIL;

	return S_OK;
}

_int Engine::CManagement::Update_Management(const _float& fTimeDelta)
{
	if(NULL == m_pCurrentScene)
		return -1;	

	return m_pCurrentScene->Update_Scene(fTimeDelta);
}

void Engine::CManagement::Render_Management(void)
{
	m_pRenderer->Render_GameObject();

	if(NULL == m_pCurrentScene)
		return;
	m_pCurrentScene->Render_Scene();
}

HRESULT Engine::CManagement::SetUp_CurrentScene(CScene* pCurrentScene)
{
	if(NULL == pCurrentScene)
		return E_FAIL;

	if(NULL != m_pCurrentScene)
	{
		if(0 != Engine::Safe_Release(m_pCurrentScene))
			MSG_BOX("Current Scene Release Failed");
	}

	m_pCurrentScene = pCurrentScene;

	return S_OK;
}

void Engine::CManagement::Free(void)
{
	Engine::Safe_Release(m_pRenderer);

	Engine::Safe_Release(m_pCurrentScene);	
}

