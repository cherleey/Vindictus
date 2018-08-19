#include "stdafx.h"
#include "Sword.h"

#include "Export_Engine.h"

CSword::CSword(LPDIRECT3DDEVICE9 pGraphicDev)
: Engine::CGameObject(pGraphicDev)
, m_pParentBoneMatrix(NULL)
, m_pParentWorldMatrix(NULL)
, m_pColliderCom(NULL)
{

}

CSword::~CSword(void)
{

}

HRESULT CSword::Ready_GameObject(void)
{
	if(FAILED(Ready_Component()))
		return E_FAIL;

	m_vMax = m_pColliderCom->GetMax();
	m_vMin = m_pColliderCom->GetMin();

	m_pTransformCom->Set_Infomation(Engine::CTransform::INFO_SCALE, &Engine::_vec3(0.4f, 0.4f, 0.4f));
	m_pTransformCom->Rotation(Engine::CTransform::ANGLE_Y, D3DXToRadian(-90.f));

	return S_OK;
}

_int CSword::Update_GameObject(const _float& fTimeDelta)
{
	if(NULL == m_pParentBoneMatrix
		|| NULL == m_pParentWorldMatrix)
	{
		Engine::CDynamicMesh*	pPlayerMeshCom = (Engine::CDynamicMesh*)Engine::Get_Component(L"Layer_Player", Engine::CGameObject::TYPE_STATIC, L"Com_Mesh");
		if(NULL == pPlayerMeshCom)
			return -1;
		m_pParentBoneMatrix = pPlayerMeshCom->Get_FrameMatrix("ValveBiped_Anim_Attachment_RH");

		Engine::CTransform*	pPlayerTransCom = (Engine::CTransform*)Engine::Get_Component(L"Layer_Player", Engine::CGameObject::TYPE_DYNAMIC, L"Com_Transform");
		if(NULL == pPlayerTransCom)
			return -1;
		m_pParentWorldMatrix = pPlayerTransCom->Get_WorldMatrix();		
	}

	m_matParent = (*m_pParentBoneMatrix * *m_pParentWorldMatrix);	

	m_pTransformCom->Set_ParentMatrix(&m_matParent);

	m_pRendererCom->Add_RenderList(Engine::CRenderer::RENDER_NONEALPHA, this);

	Engine::CGameObject::Update_GameObject(fTimeDelta);

	return 0;
}

void CSword::Render_GameObject(void)
{
	m_pTransformCom->SetUp_OnGraphicDev();	
	m_pMtrlCom->SetUp_OnGraphicDev();
	m_pMeshCom->Render_Mesh();
	m_pColliderCom->Render_Collider(Engine::CBoxCollider::TYPE_OBB, m_pTransformCom->Get_WorldMatrix());
}
HRESULT CSword::Ready_Component(void)
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

	m_pMeshCom = (Engine::CStaticMesh*)Engine::Clone_Component(SCENE_STAGE, L"Component_Mesh_Sword");
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

CSword* CSword::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CSword* pInstance = new CSword(pGraphicDev);

	if(FAILED(pInstance->Ready_GameObject()))
	{
		MSG_BOX("CSword Created Failed");
		::Safe_Release(pInstance);
	}
	return pInstance;
}

void CSword::Free(void)
{
	Engine::Safe_Release(m_pRendererCom);
	Engine::Safe_Release(m_pTransformCom);
	Engine::Safe_Release(m_pMeshCom);
	Engine::Safe_Release(m_pColliderCom);
	Engine::Safe_Release(m_pMtrlCom);


	Engine::CGameObject::Free();	
}

