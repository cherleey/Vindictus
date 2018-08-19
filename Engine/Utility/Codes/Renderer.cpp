#include "Renderer.h"
#include "GameObject.h"

USING(Engine)

Engine::CRenderer::CRenderer(LPDIRECT3DDEVICE9 pGraphicDev)
: m_pGraphicDev(pGraphicDev)
{
	m_pGraphicDev->AddRef();
}

Engine::CRenderer::~CRenderer(void)
{

}

HRESULT Engine::CRenderer::Add_RenderList(RENDER eType, CGameObject* pGameObject)
{
	if(NULL == pGameObject)
		return E_FAIL;

	m_RenderList[eType].push_back(pGameObject);

	pGameObject->Add_Ref();

	return S_OK;
}

HRESULT Engine::CRenderer::Ready_Component(void)
{

	return S_OK;
}

void Engine::CRenderer::Render_GameObject(void)
{
	Render_Priority();
	Render_NoneAlpha();
	Render_Alpha();
	Render_UI();	

	Clear_RenderList();	
}

void Engine::CRenderer::Render_Priority(void)
{
	RENDERLIST::iterator	iter = m_RenderList[RENDER_PRIORITY].begin();
	RENDERLIST::iterator	iter_end = m_RenderList[RENDER_PRIORITY].end();

	for (; iter != iter_end; ++iter)
	{
		(*iter)->Render_GameObject();
	}
}
void Engine::CRenderer::Render_NoneAlpha(void)
{
	RENDERLIST::iterator	iter = m_RenderList[RENDER_NONEALPHA].begin();
	RENDERLIST::iterator	iter_end = m_RenderList[RENDER_NONEALPHA].end();

	for (; iter != iter_end; ++iter)
	{
		(*iter)->Render_GameObject();
	}
}
void Engine::CRenderer::Render_Alpha(void)
{
	
	RENDERLIST::iterator	iter = m_RenderList[RENDER_ALPHA].begin();
	RENDERLIST::iterator	iter_end = m_RenderList[RENDER_ALPHA].end();

	for (; iter != iter_end; ++iter)
	{
		(*iter)->Render_GameObject();
	}
}
void Engine::CRenderer::Render_UI(void)
{
	RENDERLIST::iterator	iter = m_RenderList[RENDER_UI].begin();
	RENDERLIST::iterator	iter_end = m_RenderList[RENDER_UI].end();

	for (; iter != iter_end; ++iter)
	{
		(*iter)->Render_GameObject();
	}
}

void Engine::CRenderer::Clear_RenderList(void)
{
	for (_ulong i = 0; i < RENDER_END; ++i)
	{
		for_each(m_RenderList[i].begin(), m_RenderList[i].end(), CRelease_Single());
		m_RenderList[i].clear();
	}

}

CComponent* Engine::CRenderer::Clone(void)
{
	Add_Ref();

	return this;
}

CRenderer* Engine::CRenderer::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CRenderer* pInstance = new CRenderer(pGraphicDev);

	if(FAILED(pInstance->Ready_Component()))
	{
		MSG_BOX("CRenderer Created Failed");
		Engine::Safe_Release(pInstance);
	}
	return pInstance;
}

void Engine::CRenderer::Free(void)
{
	for (_ulong i = 0; i < RENDER_END; ++i)
	{
		for_each(m_RenderList[i].begin(), m_RenderList[i].end(), CRelease_Single());
		m_RenderList[i].clear();
	}

	Engine::Safe_Release(m_pGraphicDev);
}

