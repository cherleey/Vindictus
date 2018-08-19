#include "stdafx.h"
#include "Navi.h"

#include "Export_Engine.h"

CNavi::CNavi(LPDIRECT3DDEVICE9 pGraphicDev)
: Engine::CGameObject(pGraphicDev)
{

}

CNavi::~CNavi(void)
{

}

HRESULT CNavi::Ready_GameObject(void)
{
	if(FAILED(Ready_Component()))
		return E_FAIL;

	return S_OK;
}

_int CNavi::Update_GameObject(const _float& fTimeDelta)
{
	Engine::CGameObject::Update_GameObject(fTimeDelta);

	m_pRendererCom->Add_RenderList(Engine::CRenderer::RENDER_NONEALPHA, this);

	return 0;
}

_int CNavi::LastUpdate_GameObject(const _float& fTimeDelta)
{	
	m_pTransformCom->Set_Infomation(Engine::CTransform::INFO_SCALE, &Engine::_vec3(0.01f, 0.01f, 0.01f));
	return 0;
}

void CNavi::Render_GameObject(void)
{
	m_pTransformCom->SetUp_OnGraphicDev();	
	m_pMtrlCom->SetUp_OnGraphicDev();
	m_pMeshCom->Render_Mesh();
}

HRESULT CNavi::Ready_Component(void)
{
	m_pRendererCom = (Engine::CRenderer*)Engine::Clone_Component(SCENE_STATIC, L"Component_Renderer");
	if(NULL == m_pRendererCom)
		return E_FAIL;
	if(FAILED(Add_Component(Engine::CGameObject::TYPE_STATIC, L"Com_Renderer", m_pRendererCom)))
		return E_FAIL;

	m_pTransformCom = (Engine::CTransform*)Engine::Clone_Component(SCENE_STATIC, L"Component_Transform");
	if(NULL == m_pTransformCom)
		return E_FAIL;
	if(FAILED(Add_Component(Engine::CGameObject::TYPE_DYNAMIC, L"Com_Transform", m_pTransformCom)))
		return E_FAIL;

	m_pMeshCom = (Engine::CStaticMesh*)Engine::Clone_Component(SCENE_STAGE, L"Component_Mesh_Navi");
	if(NULL == m_pMeshCom)
		return E_FAIL;
	if(FAILED(Add_Component(Engine::CGameObject::TYPE_STATIC, L"Com_Mesh", m_pMeshCom)))
		return E_FAIL;

	m_pMtrlCom = (Engine::CMaterial*)Engine::Clone_Component(SCENE_STATIC, L"Component_Material");
	if(NULL == m_pMtrlCom)
		return E_FAIL;
	if(FAILED(Add_Component(Engine::CGameObject::TYPE_STATIC, L"Com_Material", m_pMtrlCom)))
		return E_FAIL;

	return S_OK;
}

CNavi* CNavi::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CNavi* pInstance = new CNavi(pGraphicDev);

	if(FAILED(pInstance->Ready_GameObject()))
	{
		MSG_BOX("CNavi Created Failed");
		::Safe_Release(pInstance);
	}
	return pInstance;
}

void CNavi::Free(void)
{
	Engine::Safe_Release(m_pRendererCom);
	Engine::Safe_Release(m_pTransformCom);
	Engine::Safe_Release(m_pMeshCom);
	Engine::Safe_Release(m_pMtrlCom);


	Engine::CGameObject::Free();	
}

