#include "stdafx.h"
#include "Scene_Stage.h"
#include "Camera_Stage.h"
#include "SkyBox.h"
#include "Map.h"
#include "Player.h"
#include "Boss.h"
#include "Ingkells_Sword.h"
#include "Sword.h"
#include "Shield.h"
#include "Export_Engine.h"
#include "Camera.h"
#include "Attack_Strong_0.h"
#include "Attack_Strong_2.h"
#include "Trail.h"
#include "Phantom_Trail.h"
#include "Attack_Strong_3.h"
#include "Counter.h"
#include "HeavyStanderCounter.h"
#include "DashSmash.h"

CScene_Stage::CScene_Stage(LPDIRECT3DDEVICE9 pGraphicDev)
: Engine::CScene(pGraphicDev)
{

}

CScene_Stage::~CScene_Stage(void)
{

}

HRESULT CScene_Stage::Ready_Scene(void)
{
	if(FAILED(Ready_RenderState()))
		return E_FAIL;

	if(FAILED(Ready_LightInfo()))
		return E_FAIL;

	if(FAILED(Ready_LayerForCamera(L"Layer_Camera")))
		return E_FAIL;

	if(FAILED(Ready_LayerForEnvironment(L"Layer_Environment")))
		return E_FAIL;

	if(FAILED(Ready_LayerForBackGround(L"Layer_BackGround")))
		return E_FAIL;	

	if(FAILED(Ready_LayerForEquipment(L"Layer_Equipment")))
		return E_FAIL;	

	if(FAILED(Ready_LayerForPlayer(L"Layer_Player")))
		return E_FAIL;	

	if(FAILED(Ready_LayerForBoss(L"Layer_Boss")))
		return E_FAIL;

	if(FAILED(Ready_LayerForEffect(L"Layer_Effect")))
		return E_FAIL;

	return S_OK;
}

_int CScene_Stage::Update_Scene(const _float& fTimeDelta)
{
	Engine::CScene::Update_Scene(fTimeDelta);

	return 0;
}

void CScene_Stage::Render_Scene(void)
{
	
}

HRESULT CScene_Stage::Ready_RenderState(void)
{
	// 깊이버퍼의 사용유무
	Engine::Set_RenderState(D3DRS_ZENABLE, TRUE); // 정점버퍼를 그릴때 깊이버퍼의 의한 깊이비교를 수행하지 않고 그리낟.
	Engine::Set_RenderState(D3DRS_ZWRITEENABLE, TRUE);
	Engine::Set_RenderState(D3DRS_LIGHTING, TRUE);

	return S_OK;
}

HRESULT CScene_Stage::Ready_LightInfo(void)
{
	D3DLIGHT9				LightInfo;
	ZeroMemory(&LightInfo, sizeof(D3DLIGHT9));

	LightInfo.Type = D3DLIGHT_DIRECTIONAL;
	LightInfo.Diffuse = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	LightInfo.Ambient = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	LightInfo.Specular = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	LightInfo.Direction = _vec3(1.f, -1.f, 1.f);

	m_pGraphicDev->SetLight(0, &LightInfo);
	m_pGraphicDev->LightEnable(0, TRUE);

	return S_OK;
}

HRESULT CScene_Stage::Ready_LayerForCamera(const _tchar* pLayerTag)
{
	Engine::CLayer*		pLayer = Engine::CLayer::Create();
	if(NULL == pLayer)
		return E_FAIL;

	Engine::CGameObject*		pGameObject = NULL;

	// For.Camera_Stage
	pGameObject = CCamera_Stage::Create(m_pGraphicDev, &_vec3(0.f, 3.f, -5.f), &_vec3(0.f, 0.f, 0.f), &_vec3(0.f, 1.f, 0.f));
	if(NULL == pGameObject)
		goto except;
	pLayer->Add_GameObject(pGameObject);

	CScene::Add_Layer(pLayerTag, pLayer);

	return S_OK;

except:
	::Safe_Release(pLayer);
	return E_FAIL;
}

HRESULT CScene_Stage::Ready_LayerForEnvironment(const _tchar* pLayerTag)
{
	Engine::CLayer*		pLayer = Engine::CLayer::Create();
	if(NULL == pLayer)
		return E_FAIL;

	Engine::CGameObject*		pGameObject = NULL;

	// For.SkyBox
	pGameObject = CSkyBox::Create(m_pGraphicDev);
	if(NULL == pGameObject)
		goto except;
	pLayer->Add_GameObject(pGameObject);

	CScene::Add_Layer(pLayerTag, pLayer);

	return S_OK;

except:
	::Safe_Release(pLayer);
	return E_FAIL;

}

HRESULT CScene_Stage::Ready_LayerForBackGround(const _tchar* pLayerTag)
{
	Engine::CLayer*		pLayer = Engine::CLayer::Create();
	if(NULL == pLayer)
		return E_FAIL;

	Engine::CGameObject*		pGameObject = NULL;

	// For.Map
	pGameObject = CMap::Create(m_pGraphicDev);

	if(NULL == pGameObject)
		goto except;
	pLayer->Add_GameObject(pGameObject);

	// For.Navi
	/*pGameObject = CNavi::Create(m_pGraphicDev);

	if(NULL == pGameObject)
		goto except;
	pLayer->Add_GameObject(pGameObject);*/
	
	CScene::Add_Layer(pLayerTag, pLayer);

	return S_OK;	

except:
	::Safe_Release(pLayer);
	return E_FAIL;
}

HRESULT CScene_Stage::Ready_LayerForEquipment(const _tchar* pLayerTag)
{
	Engine::CLayer*		pLayer = Engine::CLayer::Create();
	if(NULL == pLayer)
		return E_FAIL;

	Engine::CGameObject*		pGameObject = NULL;


	// For.Sword
	pGameObject = CSword::Create(m_pGraphicDev);

	if(NULL == pGameObject)
		goto except;
	pLayer->Add_GameObject(pGameObject);

	// For.Shield
	pGameObject = CShield::Create(m_pGraphicDev);

	if(NULL == pGameObject)
		goto except;
	pLayer->Add_GameObject(pGameObject);

	CScene::Add_Layer(pLayerTag, pLayer);

	return S_OK;	

except:
	::Safe_Release(pLayer);
	return E_FAIL;	
}

HRESULT CScene_Stage::Ready_LayerForPlayer(const _tchar* pLayerTag)
{
	Engine::CLayer*		pLayer = Engine::CLayer::Create();
	if(NULL == pLayer)
		return E_FAIL;

	Engine::CGameObject*		pGameObject = NULL;

	
	// For.Player
	pGameObject = CPlayer::Create(m_pGraphicDev, (CCamera_Stage*)(Find_Layer(L"Layer_Camera")->Get_GameObject()->front()));

	if(NULL == pGameObject)
		goto except;
	pLayer->Add_GameObject(pGameObject);

	CScene::Add_Layer(pLayerTag, pLayer);

	return S_OK;	

except:
	::Safe_Release(pLayer);
	return E_FAIL;	
}

HRESULT CScene_Stage::Ready_LayerForBoss(const _tchar* pLayerTag)
{
	Engine::CLayer*		pLayer = Engine::CLayer::Create();
	if(NULL == pLayer)
		return E_FAIL;

	Engine::CGameObject*		pGameObject = NULL;


	// For.Boss
	pGameObject = CBoss::Create(m_pGraphicDev);

	if(NULL == pGameObject)
		goto except;
	pLayer->Add_GameObject(pGameObject);

	// For.Boss's Sword
	pGameObject = CIngkells_Sword::Create(m_pGraphicDev);

	if(NULL == pGameObject)
		goto except;
	pLayer->Add_GameObject(pGameObject);

	CScene::Add_Layer(pLayerTag, pLayer);

	return S_OK;	

except:
	::Safe_Release(pLayer);
	return E_FAIL;	
}


HRESULT CScene_Stage::Ready_LayerForEffect(const _tchar* pLayerTag)
{
	Engine::CLayer*		pLayer = Engine::CLayer::Create();
	if(NULL == pLayer)
		return E_FAIL;

	Engine::CGameObject*		pGameObject = NULL;

	// For.Trail
	pGameObject = CTrail::Create(m_pGraphicDev);

	if(NULL == pGameObject)
		goto except;
	pLayer->Add_GameObject(pGameObject);

	//For.Attack_Strong_0
	pGameObject = CAttack_Strong_0::Create(m_pGraphicDev);

	if(NULL == pGameObject)
		goto except;
	pLayer->Add_GameObject(pGameObject);

	//For.Attack_Strong_2
	pGameObject = CAttack_Strong_2::Create(m_pGraphicDev);

	if(NULL == pGameObject)
		goto except;
	pLayer->Add_GameObject(pGameObject);

	//For.Phantom_Trail
	pGameObject = CPhantom_Trail::Create(m_pGraphicDev);

	if(NULL == pGameObject)
		goto except;
	pLayer->Add_GameObject(pGameObject);

	//For.Attack_Strong_3
	pGameObject = CAttack_Strong_3::Create(m_pGraphicDev);

	if(NULL == pGameObject)
		goto except;
	pLayer->Add_GameObject(pGameObject);

	//For.Counter
	pGameObject = CCounter::Create(m_pGraphicDev);

	if(NULL == pGameObject)
		goto except;
	pLayer->Add_GameObject(pGameObject);

	//For.HeavyStanderCounter
	pGameObject = CHeavyStanderCounter::Create(m_pGraphicDev);

	if(NULL == pGameObject)
		goto except;
	pLayer->Add_GameObject(pGameObject);

	//For.DashSmash
	pGameObject = CDashSmash::Create(m_pGraphicDev);

	if(NULL == pGameObject)
		goto except;
	pLayer->Add_GameObject(pGameObject);

	CScene::Add_Layer(pLayerTag, pLayer);

	return S_OK;	

except:
	::Safe_Release(pLayer);
	return E_FAIL;	
}


CScene_Stage* CScene_Stage::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CScene_Stage*		pInstance = new CScene_Stage(pGraphicDev);

	if(FAILED(pInstance->Ready_Scene()))
	{
		MessageBox(NULL, L"CScene_Stage Created Failed", L"System Error", MB_OK);
		::Safe_Release(pInstance);		
	}
	return pInstance;
}

void CScene_Stage::Free(void)
{
	Engine::CScene::Free();


}
