#include "stdafx.h"
#include "Loading.h"
#include "Export_Engine.h"


CLoading::CLoading(LPDIRECT3DDEVICE9 pGraphicDev)
: m_pGraphicDev(pGraphicDev)
, m_isFinish(false)
{
	m_pGraphicDev->AddRef();
	ZeroMemory(m_szloadingString, sizeof(_tchar) * 128);
}

CLoading::~CLoading(void)
{

}

HRESULT CLoading::Ready_Loading(LOADINGID eLoadingID)
{
	InitializeCriticalSection(&m_Crt);

	m_hThread = (HANDLE)_beginthreadex(NULL, 0, Thread_Main, this, 0, NULL);

	m_LoadingID = eLoadingID;

	return S_OK;
}

_uint CLoading::Loading_ForStage(void)
{
	// For.Textures Loading ============================================================================================================================================
	lstrcpy(m_szloadingString, L"Textures Loading...");

	Engine::CComponent*			pComponent = NULL;

	// For. Attack_Strong_0
	pComponent = Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Texture/Effect/attack_strong_00/efx_sword_sharp_line.tga", Engine::CTexture::TYPE_NORMAL);
	if(NULL == pComponent)
		return E_FAIL;
	Engine::Add_Component(SCENE_STAGE, L"Component_Texture_Sword_Sharp_Line", pComponent);

	pComponent = Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Texture/Effect/attack_strong_00/efx_sword_sharp_plane.tga", Engine::CTexture::TYPE_NORMAL);
	if(NULL == pComponent)
		return E_FAIL;
	Engine::Add_Component(SCENE_STAGE, L"Component_Texture_Sword_Sharp_Plane", pComponent);

	pComponent = Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Texture/Effect/attack_strong_00/efx_sword_radial_blur.tga", Engine::CTexture::TYPE_NORMAL);
	if(NULL == pComponent)
		return E_FAIL;
	Engine::Add_Component(SCENE_STAGE, L"Component_Texture_Sword_Radial_Blur", pComponent);

	//For. Attack_Strong_1
	pComponent = Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Texture/Effect/attack_strong_01/Shape_upshotBlue.tga", Engine::CTexture::TYPE_NORMAL);
	if(NULL == pComponent)
		return E_FAIL;
	Engine::Add_Component(SCENE_STAGE, L"Component_Texture_Shape_UpShot", pComponent);

	pComponent = Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Texture/Effect/attack_strong_01/heroes_phantom_trail05.tga", Engine::CTexture::TYPE_NORMAL);
	if(NULL == pComponent)
		return E_FAIL;
	Engine::Add_Component(SCENE_STAGE, L"Component_Texture_Phantom_Trail", pComponent);

	//For. Attack_Stroing_2
	pComponent = Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Texture/Effect/attack_strong_02/shield_zoom.tga", Engine::CTexture::TYPE_NORMAL);
	if(NULL == pComponent)
		return E_FAIL;
	Engine::Add_Component(SCENE_STAGE, L"Component_Texture_Shield_Zoom", pComponent);

	pComponent = Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Texture/Effect/attack_strong_02/ring2.tga", Engine::CTexture::TYPE_NORMAL);
	if(NULL == pComponent)
		return E_FAIL;
	Engine::Add_Component(SCENE_STAGE, L"Component_Texture_Ring2", pComponent);

	pComponent = Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Texture/Effect/attack_strong_03/ring_outer_wind06.tga", Engine::CTexture::TYPE_NORMAL);
	if(NULL == pComponent)
		return E_FAIL;
	Engine::Add_Component(SCENE_STAGE, L"Component_Texture_Ring_Outer_Wind06", pComponent);

	//For. Attack_Strong_3
	pComponent = Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Texture/Effect/attack_strong_03/wind.tga", Engine::CTexture::TYPE_NORMAL);
	if(NULL == pComponent)
		return E_FAIL;
	Engine::Add_Component(SCENE_STAGE, L"Component_Texture_Wind", pComponent);

	pComponent = Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Texture/Effect/attack_strong_03/blunt.tga", Engine::CTexture::TYPE_NORMAL);
	if(NULL == pComponent)
		return E_FAIL;
	Engine::Add_Component(SCENE_STAGE, L"Component_Texture_Blunt", pComponent);

	//For. Counter
	pComponent = Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Texture/Effect/counter/blunt_yellow.tga", Engine::CTexture::TYPE_NORMAL);
	if(NULL == pComponent)
		return E_FAIL;
	Engine::Add_Component(SCENE_STAGE, L"Component_Texture_Yellow_Blunt", pComponent);

	//For. HeavyStanderCounter
	pComponent = Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Texture/Effect/HeavyStanderCounter/ring_disk.tga", Engine::CTexture::TYPE_NORMAL);
	if(NULL == pComponent)
		return E_FAIL;
	Engine::Add_Component(SCENE_STAGE, L"Component_Texture_Ring_Disk", pComponent);

	pComponent = Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Texture/Effect/HeavyStanderCounter/holy_shield.tga", Engine::CTexture::TYPE_NORMAL);
	if(NULL == pComponent)
		return E_FAIL;
	Engine::Add_Component(SCENE_STAGE, L"Component_Texture_Holy_Shield", pComponent);

	// For. Sword Trail
	pComponent = Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Texture/Effect/Trail/trail_regina_sword01.tga", Engine::CTexture::TYPE_NORMAL);
	if(NULL == pComponent)
		return E_FAIL;
	Engine::Add_Component(SCENE_STAGE, L"Component_Texture_Trail", pComponent);

	// For.SkyBox Texture
	pComponent = Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Texture/SkyBox/Burger%d.dds", Engine::CTexture::TYPE_CUBE, 4);
	if(NULL == pComponent)
		return E_FAIL;
	Engine::Add_Component(SCENE_STAGE, L"Component_Texture_SkyBox", pComponent);
	
	
	// For.Buffer Loading ============================================================================================================================================
	lstrcpy(m_szloadingString, L"Buffers Loading...");

	pComponent = Engine::CCube_Texture::Create(m_pGraphicDev);
	if(NULL == pComponent)
		return E_FAIL;
	Engine::Add_Component(SCENE_STAGE, L"Component_Buffer_Cube", pComponent);

	// For.Collider
	lstrcpy(m_szloadingString, L"Collider Loading...");
	pComponent = Engine::CBoxCollider::Create(m_pGraphicDev);
	if(NULL == pComponent)
		return E_FAIL;
	Engine::Add_Component(SCENE_STAGE, L"Component_BoxCollider", pComponent);




	// For.Meshes Loading ============================================================================================================================================
	lstrcpy(m_szloadingString, L"Meshes Loading...");

	pComponent = Engine::CStaticMesh::Create(m_pGraphicDev, L"../Bin/Mesh/StaticMesh/Fountain_Map/", L"Map.x");
	if(NULL == pComponent)
		return E_FAIL;
	Engine::Add_Component(SCENE_STAGE, L"Component_Mesh_Map", pComponent);

	/*pComponent = Engine::CStaticMesh::Create(m_pGraphicDev, L"../Bin/Mesh/StaticMesh/Navi/", L"navi.x");
	if(NULL == pComponent)
		return E_FAIL;
	Engine::Add_Component(SCENE_STAGE, L"Component_Mesh_Navi", pComponent);*/

	pComponent = Engine::CStaticMesh::Create(m_pGraphicDev, L"../Bin/Mesh/StaticMesh/Ingkells_Sword/", L"ingkells_sword.x");
	if(NULL == pComponent)
		return E_FAIL;
	Engine::Add_Component(SCENE_STAGE, L"Component_Mesh_Ingkells_Sword", pComponent);

	pComponent = Engine::CStaticMesh::Create(m_pGraphicDev, L"../Bin/Mesh/StaticMesh/Sword/", L"sword.x");
	if(NULL == pComponent)
		return E_FAIL;
	Engine::Add_Component(SCENE_STAGE, L"Component_Mesh_Sword", pComponent);

	pComponent = Engine::CStaticMesh::Create(m_pGraphicDev, L"../Bin/Mesh/StaticMesh/Shield/", L"shield.x");
	if(NULL == pComponent)
		return E_FAIL;
	Engine::Add_Component(SCENE_STAGE, L"Component_Mesh_Shield", pComponent);
	
	pComponent = Engine::CDynamicMesh::Create(m_pGraphicDev, L"../Bin/Mesh/DynamicMesh/fiona/", L"fiona_normal.x");
	if(NULL == pComponent)
		return E_FAIL;
	Engine::Add_Component(SCENE_STAGE, L"Component_Mesh_Fiona", pComponent);

	pComponent = Engine::CDynamicMesh::Create(m_pGraphicDev, L"../Bin/Mesh/DynamicMesh/ingkells/", L"ingkells_normal.x");
	if(NULL == pComponent)
		return E_FAIL;
	Engine::Add_Component(SCENE_STAGE, L"Component_Mesh_Ingkells", pComponent);

	// For.Shader Loading ============================================================================================================================================
	lstrcpy(m_szloadingString, L"Shader Loading...");

	pComponent = Engine::CShader::Create(m_pGraphicDev, L"../Codes/Shader_Mesh.hpp");
	if(NULL == pComponent)
		return E_FAIL;
	Engine::Add_Component(SCENE_STAGE, L"Component_Shader_Mesh", pComponent);

	pComponent = Engine::CShader::Create(m_pGraphicDev, L"../Codes/Shader_Effect.hpp");
	if(NULL == pComponent)
		return E_FAIL;
	Engine::Add_Component(SCENE_STAGE, L"Component_Shader_Effect", pComponent);

	pComponent = Engine::CShader::Create(m_pGraphicDev, L"../Codes/Shader_Trail.hpp");
	if(NULL == pComponent)
		return E_FAIL;
	Engine::Add_Component(SCENE_STAGE, L"Component_Shader_Trail", pComponent);

	m_isFinish = true;

	lstrcpy(m_szloadingString, L"Loading Complete...");

	return 0;
}

unsigned int APIENTRY CLoading::Thread_Main(void* pArg)
{
	CLoading*		pLoading = (CLoading*)pArg;

	_uint		iFlag = 0;

	EnterCriticalSection(pLoading->Get_CriticalSection());

	switch(pLoading->Get_LoadingID())
	{
	case LOADING_STAGE:
		iFlag = pLoading->Loading_ForStage();
		break;
	case LOADING_BOSS:
		//pLoading->Loading_ForBoss();
		break;
	}	

	LeaveCriticalSection(pLoading->Get_CriticalSection());

	return iFlag;
}

CLoading* CLoading::Create(LPDIRECT3DDEVICE9 pGraphicDev, LOADINGID eLoadingID)
{
	CLoading*	pInstance = new CLoading(pGraphicDev);

	if(FAILED(pInstance->Ready_Loading(eLoadingID)))
	{
		MSG_BOX("CLoading Created Failed");
		::Safe_Release(pInstance);
	}
	return pInstance;
}

void CLoading::Free(void)
{
	WaitForSingleObject(m_hThread, INFINITE);
	CloseHandle(m_hThread);	
	DeleteCriticalSection(&m_Crt);

	::Safe_Release(m_pGraphicDev);
}

