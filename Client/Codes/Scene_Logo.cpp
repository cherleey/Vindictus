#include "stdafx.h"
#include "Scene_Logo.h"
#include "BackGround_Logo.h"
#include "Loading.h"
#include "Scene_Stage.h"
#include "Export_Engine.h"

CScene_Logo::CScene_Logo(LPDIRECT3DDEVICE9 pGraphicDev)
: Engine::CScene(pGraphicDev)
, m_pLoading(NULL)
{

}

CScene_Logo::~CScene_Logo(void)
{

}

HRESULT CScene_Logo::Ready_Scene(void)
{
	if(FAILED(Ready_RenderState()))
		return E_FAIL;

	if(FAILED(Ready_LightInfo()))
		return E_FAIL;

	if(FAILED(Ready_Component()))
		return E_FAIL;

	if(FAILED(Ready_LayerForBackGround(L"Layer_BackGround")))
		return E_FAIL;	

	m_pLoading = CLoading::Create(m_pGraphicDev, CLoading::LOADING_STAGE);
	if(NULL == m_pLoading)
		return E_FAIL;


	return S_OK;
}

_int CScene_Logo::Update_Scene(const _float& fTimeDelta)
{
	Engine::CScene::Update_Scene(fTimeDelta);

	if(GetAsyncKeyState(VK_SPACE) & 0x8000)
	{
		if(true == m_pLoading->Get_Finish())
		{
			Engine::CScene*		pStageScene = CScene_Stage::Create(m_pGraphicDev);
			if(FAILED(Engine::SetUp_CurrentScene(pStageScene)))
				return -1;
			return 0;
		}
	}

	return 0;
}

void CScene_Logo::Render_Scene(void)
{
	Engine::Render_Font(L"Font_Batang", m_pLoading->Get_String(), &_vec2(400.0f, 0.0f), D3DXCOLOR(1.f, 1.f, 1.f, 1.f));
}

HRESULT CScene_Logo::Ready_RenderState(void)
{
	// 깊이버퍼의 사용유무
	Engine::Set_RenderState(D3DRS_ZENABLE, FALSE); // 정점버퍼를 그릴때 깊이버퍼의 의한 깊이비교를 수행하지 않고 그리낟.
	Engine::Set_RenderState(D3DRS_ZWRITEENABLE, FALSE);
	Engine::Set_RenderState(D3DRS_LIGHTING, FALSE);

	return S_OK;
}

HRESULT CScene_Logo::Ready_LightInfo(void)
{
	return S_OK;
}

HRESULT CScene_Logo::Ready_LayerForBackGround(const _tchar* pLayerTag)
{
	Engine::CLayer*		pLayer = Engine::CLayer::Create();
	if(NULL == pLayer)
		return E_FAIL;

	Engine::CGameObject*		pGameObject = NULL;

	// For.Background_Logo
	pGameObject = CBackGround_Logo::Create(m_pGraphicDev);

	if(NULL == pGameObject)
		goto except;
	pLayer->Add_GameObject(pGameObject);
	
	CScene::Add_Layer(pLayerTag, pLayer);

	return S_OK;	

except:
	::Safe_Release(pLayer);
	return E_FAIL;
}

HRESULT CScene_Logo::Ready_Component(void)
{
	Engine::CComponent*			pComponent = NULL;

	pComponent = Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Texture/Logo/Logo.png", Engine::CTexture::TYPE_NORMAL);
	if(NULL == pComponent)
		return E_FAIL;
	Engine::Add_Component(SCENE_LOGO, L"Component_Texture_Logo", pComponent);


	return S_OK;
}

CScene_Logo* CScene_Logo::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CScene_Logo*		pInstance = new CScene_Logo(pGraphicDev);

	if(FAILED(pInstance->Ready_Scene()))
	{
		MessageBox(NULL, L"CScene_Logo Created Failed", L"System Error", MB_OK);
		::Safe_Release(pInstance);		
	}
	return pInstance;
}

void CScene_Logo::Free(void)
{
	Engine::CScene::Free();

	Engine::Release_Component(SCENE_LOGO);
	
	Engine::Safe_Release(m_pLoading);

}

