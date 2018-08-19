#include "stdafx.h"
#include "Map.h"

#include "Export_Engine.h"

CMap::CMap(LPDIRECT3DDEVICE9 pGraphicDev)
: Engine::CGameObject(pGraphicDev)
{

}

CMap::~CMap(void)
{

}

HRESULT CMap::Ready_GameObject(void)
{
	if(FAILED(Ready_Component()))
		return E_FAIL;

	return S_OK;
}

_int CMap::Update_GameObject(const _float& fTimeDelta)
{
	Engine::CGameObject::Update_GameObject(fTimeDelta);

	m_pRendererCom->Add_RenderList(Engine::CRenderer::RENDER_NONEALPHA, this);

	return 0;
}

_int CMap::LastUpdate_GameObject(const _float& fTimeDelta)
{	
	m_pTransformCom->Set_Infomation(Engine::CTransform::INFO_SCALE, &Engine::_vec3(0.01f, 0.01f, 0.01f));
	return 0;
}

void CMap::Render_GameObject(void)
{
	m_pTransformCom->SetUp_OnGraphicDev();	
	m_pMtrlCom->SetUp_OnGraphicDev();
	m_pMeshCom->Render_Mesh();

	COLLIDER::iterator iter = m_ColliderList.begin();
	COLLIDER::iterator iter_end = m_ColliderList.end();

	for(;iter != iter_end; ++iter)
		(*iter)->Render_Collider(Engine::CBoxCollider::TYPE_AABB, m_pTransformCom->Get_WorldMatrix());
}

HRESULT CMap::Ready_Component(void)
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

	m_pMeshCom = (Engine::CStaticMesh*)Engine::Clone_Component(SCENE_STAGE, L"Component_Mesh_Map");
	if(NULL == m_pMeshCom)
		return E_FAIL;
	if(FAILED(Add_Component(Engine::CGameObject::TYPE_STATIC, L"Com_Mesh", m_pMeshCom)))
		return E_FAIL;

	m_pMtrlCom = (Engine::CMaterial*)Engine::Clone_Component(SCENE_STATIC, L"Component_Material");
	if(NULL == m_pMtrlCom)
		return E_FAIL;
	if(FAILED(Add_Component(Engine::CGameObject::TYPE_STATIC, L"Com_Material", m_pMtrlCom)))
		return E_FAIL;

	m_pColliderCom = (Engine::CBoxCollider*)Engine::Clone_Component(SCENE_STAGE, L"Component_BoxCollider");
	if(NULL == m_pColliderCom)
		return E_FAIL;
	if(FAILED(m_pColliderCom->Compute_Collider(D3DXVECTOR3(836.f, 100.f, 260.f), D3DXVECTOR3(608.f, 180.f, 895.f))))
		return E_FAIL;
	if(FAILED(Add_Component(Engine::CGameObject::TYPE_STATIC, L"Com_URWCollider", m_pColliderCom)))
		return E_FAIL;

	m_ColliderList.push_back(m_pColliderCom);

	m_pColliderCom = (Engine::CBoxCollider*)Engine::Clone_Component(SCENE_STAGE, L"Component_BoxCollider");
	if(NULL == m_pColliderCom)
		return E_FAIL;
	if(FAILED(m_pColliderCom->Compute_Collider(D3DXVECTOR3(260.f, 100.f, 836.f), D3DXVECTOR3(897.f, 180.f, 610.f))))
		return E_FAIL;
	if(FAILED(Add_Component(Engine::CGameObject::TYPE_STATIC, L"Com_URHCollider", m_pColliderCom)))
		return E_FAIL;

	m_ColliderList.push_back(m_pColliderCom);

	m_pColliderCom = (Engine::CBoxCollider*)Engine::Clone_Component(SCENE_STAGE, L"Component_BoxCollider");
	if(NULL == m_pColliderCom)
		return E_FAIL;
	if(FAILED(m_pColliderCom->Compute_Collider(D3DXVECTOR3(836.f, 100.f, 260.f), D3DXVECTOR3(608.f, 180.f, -895.f))))
		return E_FAIL;
	if(FAILED(Add_Component(Engine::CGameObject::TYPE_STATIC, L"Com_DRWCollider", m_pColliderCom)))
		return E_FAIL;

	m_ColliderList.push_back(m_pColliderCom);

	m_pColliderCom = (Engine::CBoxCollider*)Engine::Clone_Component(SCENE_STAGE, L"Component_BoxCollider");
	if(NULL == m_pColliderCom)
		return E_FAIL;
	if(FAILED(m_pColliderCom->Compute_Collider(D3DXVECTOR3(260.f, 100.f, 836.f), D3DXVECTOR3(897.f, 180.f, -610.f))))
		return E_FAIL;
	if(FAILED(Add_Component(Engine::CGameObject::TYPE_STATIC, L"Com_DRHCollider", m_pColliderCom)))
		return E_FAIL;

	m_ColliderList.push_back(m_pColliderCom);

	m_pColliderCom = (Engine::CBoxCollider*)Engine::Clone_Component(SCENE_STAGE, L"Component_BoxCollider");
	if(NULL == m_pColliderCom)
		return E_FAIL;
	if(FAILED(m_pColliderCom->Compute_Collider(D3DXVECTOR3(836.f, 100.f, 260.f), D3DXVECTOR3(-608.f, 180.f, -895.f))))
		return E_FAIL;
	if(FAILED(Add_Component(Engine::CGameObject::TYPE_STATIC, L"Com_DLWCollider", m_pColliderCom)))
		return E_FAIL;

	m_ColliderList.push_back(m_pColliderCom);

	m_pColliderCom = (Engine::CBoxCollider*)Engine::Clone_Component(SCENE_STAGE, L"Component_BoxCollider");
	if(NULL == m_pColliderCom)
		return E_FAIL;
	if(FAILED(m_pColliderCom->Compute_Collider(D3DXVECTOR3(260.f, 100.f, 836.f), D3DXVECTOR3(-897.f, 180.f, -610.f))))
		return E_FAIL;
	if(FAILED(Add_Component(Engine::CGameObject::TYPE_STATIC, L"Com_DLHCollider", m_pColliderCom)))
		return E_FAIL;

	m_ColliderList.push_back(m_pColliderCom);

	m_pColliderCom = (Engine::CBoxCollider*)Engine::Clone_Component(SCENE_STAGE, L"Component_BoxCollider");
	if(NULL == m_pColliderCom)
		return E_FAIL;
	if(FAILED(m_pColliderCom->Compute_Collider(D3DXVECTOR3(836.f, 100.f, 260.f), D3DXVECTOR3(-608.f, 180.f, 895.f))))
		return E_FAIL;
	if(FAILED(Add_Component(Engine::CGameObject::TYPE_STATIC, L"Com_ULWCollider", m_pColliderCom)))
		return E_FAIL;

	m_ColliderList.push_back(m_pColliderCom);

	m_pColliderCom = (Engine::CBoxCollider*)Engine::Clone_Component(SCENE_STAGE, L"Component_BoxCollider");
	if(NULL == m_pColliderCom)
		return E_FAIL;
	if(FAILED(m_pColliderCom->Compute_Collider(D3DXVECTOR3(260.f, 100.f, 836.f), D3DXVECTOR3(-897.f, 180.f, 610.f))))
		return E_FAIL;
	if(FAILED(Add_Component(Engine::CGameObject::TYPE_STATIC, L"Com_ULHCollider", m_pColliderCom)))
		return E_FAIL;

	m_ColliderList.push_back(m_pColliderCom);

	m_pColliderCom = (Engine::CBoxCollider*)Engine::Clone_Component(SCENE_STAGE, L"Component_BoxCollider");
	if(NULL == m_pColliderCom)
		return E_FAIL;
	if(FAILED(m_pColliderCom->Compute_Collider(D3DXVECTOR3(500.f, 100.f, 260.f), D3DXVECTOR3(0.f, 180.f, 1155.f))))
		return E_FAIL;
	if(FAILED(Add_Component(Engine::CGameObject::TYPE_STATIC, L"Com_UCollider", m_pColliderCom)))
		return E_FAIL;

	m_ColliderList.push_back(m_pColliderCom);

	m_pColliderCom = (Engine::CBoxCollider*)Engine::Clone_Component(SCENE_STAGE, L"Component_BoxCollider");
	if(NULL == m_pColliderCom)
		return E_FAIL;
	if(FAILED(m_pColliderCom->Compute_Collider(D3DXVECTOR3(500.f, 100.f, 260.f), D3DXVECTOR3(0.f, 180.f, -1155.f))))
		return E_FAIL;
	if(FAILED(Add_Component(Engine::CGameObject::TYPE_STATIC, L"Com_DCollider", m_pColliderCom)))
		return E_FAIL;

	m_ColliderList.push_back(m_pColliderCom);

	m_pColliderCom = (Engine::CBoxCollider*)Engine::Clone_Component(SCENE_STAGE, L"Component_BoxCollider");
	if(NULL == m_pColliderCom)
		return E_FAIL;
	if(FAILED(m_pColliderCom->Compute_Collider(D3DXVECTOR3(260.f, 100.f, 500.f), D3DXVECTOR3(-1155.f, 180.f, 0.f))))
		return E_FAIL;
	if(FAILED(Add_Component(Engine::CGameObject::TYPE_STATIC, L"Com_LCollider", m_pColliderCom)))
		return E_FAIL;

	m_ColliderList.push_back(m_pColliderCom);

	m_pColliderCom = (Engine::CBoxCollider*)Engine::Clone_Component(SCENE_STAGE, L"Component_BoxCollider");
	if(NULL == m_pColliderCom)
		return E_FAIL;
	if(FAILED(m_pColliderCom->Compute_Collider(D3DXVECTOR3(260.f, 100.f, 500.f), D3DXVECTOR3(1155.f, 180.f, 0.f))))
		return E_FAIL;
	if(FAILED(Add_Component(Engine::CGameObject::TYPE_STATIC, L"Com_RCollider", m_pColliderCom)))
		return E_FAIL;

	m_ColliderList.push_back(m_pColliderCom);

	return S_OK;
}

CMap* CMap::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CMap* pInstance = new CMap(pGraphicDev);

	if(FAILED(pInstance->Ready_GameObject()))
	{
		MSG_BOX("CMap Created Failed");
		::Safe_Release(pInstance);
	}
	return pInstance;
}

void CMap::Free(void)
{
	Engine::Safe_Release(m_pRendererCom);
	Engine::Safe_Release(m_pTransformCom);
	Engine::Safe_Release(m_pMeshCom);
	Engine::Safe_Release(m_pMtrlCom);

	for_each(m_ColliderList.begin(), m_ColliderList.end(), Engine::CRelease_Single());
	m_ColliderList.clear();

	Engine::CGameObject::Free();	
}

list<Engine::CBoxCollider*>* CMap::GetCollider(void)
{
	return &m_ColliderList;
}

