#include "stdafx.h"
#include "Stone.h"

#include "Export_Engine.h"

CStone::CStone(LPDIRECT3DDEVICE9 pGraphicDev)
: Engine::CGameObject(pGraphicDev)
, m_isColl(false)
{

}

CStone::~CStone(void)
{

}

HRESULT CStone::Ready_GameObject(void)
{
	if(FAILED(Ready_Component()))
		return E_FAIL;

	m_pTransformCom->Set_Infomation(Engine::CTransform::INFO_POSITION, &Engine::_vec3(rand() % 5, 3.f, rand() % 5));
	m_pTransformCom->Set_Infomation(Engine::CTransform::INFO_ANGLE, &Engine::_vec3(0.0f, D3DXToRadian(rand() % 360), 0.0f));

	return S_OK;
}

_int CStone::Update_GameObject(const _float& fTimeDelta)
{
	Engine::CGameObject::Update_GameObject(fTimeDelta);

	m_pRendererCom->Add_RenderList(Engine::CRenderer::RENDER_NONEALPHA, this);

	return 0;
}

_int CStone::LastUpdate_GameObject(const _float& fTimeDelta)
{	
	Engine::CBoxCollider*		pPlayerCollider = (Engine::CBoxCollider*)Engine::Get_Component(L"Layer_Player", Engine::CGameObject::TYPE_STATIC, L"Com_Collider", 1);

	m_isColl = m_pColliderCom->Collision_OBB(pPlayerCollider);

	return 0;
}

void CStone::Render_GameObject(void)
{
	m_pTransformCom->SetUp_OnGraphicDev();	
	m_pMtrlCom->SetUp_OnGraphicDev();
	m_pMeshCom->Render_Mesh();
	m_pColliderCom->Render_Collider(Engine::CBoxCollider::TYPE_OBB, m_isColl, m_pTransformCom->Get_WorldMatrix());
}

HRESULT CStone::Ready_Component(void)
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

	m_pMeshCom = (Engine::CStaticMesh*)Engine::Clone_Component(SCENE_STAGE, L"Component_Mesh_Stone");
	if(NULL == m_pMeshCom)
		return E_FAIL;
	if(FAILED(Add_Component(Engine::CGameObject::TYPE_STATIC, L"Com_Mesh", m_pMeshCom)))
		return E_FAIL;

	m_pColliderCom = (Engine::CBoxCollider*)Engine::Clone_Component(SCENE_STAGE, L"Component_BoxCollider");
	if(NULL == m_pColliderCom)
		return E_FAIL;
	if(FAILED(m_pColliderCom->Compute_Collider(m_pMeshCom)))
		return E_FAIL;
	if(FAILED(Add_Component(Engine::CGameObject::TYPE_STATIC, L"Com_Collider", m_pColliderCom)))
		return E_FAIL;

	m_pMtrlCom = (Engine::CMaterial*)Engine::Clone_Component(SCENE_STATIC, L"Component_Material");
	if(NULL == m_pMtrlCom)
		return E_FAIL;
	if(FAILED(Add_Component(Engine::CGameObject::TYPE_STATIC, L"Com_Material", m_pMtrlCom)))
		return E_FAIL;

	return S_OK;
}

CStone* CStone::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CStone* pInstance = new CStone(pGraphicDev);

	if(FAILED(pInstance->Ready_GameObject()))
	{
		MSG_BOX("CStone Created Failed");
		::Safe_Release(pInstance);
	}
	return pInstance;
}

void CStone::Free(void)
{
	Engine::Safe_Release(m_pRendererCom);
	Engine::Safe_Release(m_pTransformCom);
	Engine::Safe_Release(m_pMeshCom);
	Engine::Safe_Release(m_pColliderCom);
	Engine::Safe_Release(m_pMtrlCom);


	Engine::CGameObject::Free();	
}

