#include "stdafx.h"
#include "Player.h"
#include "AnimationCtrl.h"
#include "Boss.h"
#include "Map.h"

#include "Export_Engine.h"
#include <d3dx9math.h>

CPlayer::CPlayer(LPDIRECT3DDEVICE9 pGraphicDev)
: Engine::CGameObject(pGraphicDev)
, m_fTimeDelta(0.f)
, m_pColliderCom_Foot(NULL)
, m_pColliderCom(NULL)
, m_pPreHitColliderCom(NULL)
, m_pShaderCom(NULL)
, m_fSpeed(1.5f)
, m_iAttackNum(0)
, m_bBack(false)
, m_pCamera(NULL)
, m_bGuarding(false)
, m_bRunning(false)
, m_bSmashing(false)
, m_bSprinting(false)
, m_bAttacking(false)
, m_fDamage(10.f)
, m_bIsCol(false)
, m_bDown(false)
, m_iDownState(0)
, m_bCancleEnable(false)
, m_iSmashNum(0)
, m_bAttackedFromBack(false)
, m_bDownRotate(false)
, m_bAttackRotate(false)
, m_bGuarded(false)
, m_bPreHit(false)
, m_bPGuard(false)
, m_fPGuardTiming(0.f)
, m_bStumbling(false)
, m_bCounterEnable(false)
, m_iCounterNum(0)
, m_fGroggyDamage(2.f)
, m_fSpeedOffSet(1.f)
, m_bColWithMap(false)
, m_pTrail(NULL)
, m_pAttackStrong0(NULL)
, m_pAttackStrong2(NULL)
, m_pAttackStrong3(NULL)
, m_pPhantom_Trail(NULL)
, m_pCounter(NULL)
, m_pHeavyStanderCounter(NULL)
, m_pDashSmash(NULL)
{

}

CPlayer::~CPlayer(void)
{

}

HRESULT CPlayer::Ready_GameObject(CCamera_Stage* pCamera)
{
	if(FAILED(Ready_Component()))
		return E_FAIL;

	m_pCamera = pCamera;

	m_pTransformCom->Set_Infomation(Engine::CTransform::INFO_POSITION, &Engine::_vec3(0.0f, 1.3f, 0.0f));
	m_pTransformCom->Set_Infomation(Engine::CTransform::INFO_SCALE, &Engine::_vec3(0.01f, 0.01f, 0.01f));	
	m_pTransformCom->Rotation(Engine::CTransform::ANGLE_Y, D3DXToRadian(-90.f));

	m_pMeshCom->Set_AnimationSet(15);

	return S_OK;
}

_int CPlayer::Update_GameObject(const _float& fTimeDelta)
{
	m_fTimeDelta = fTimeDelta;

	Engine::CLayer::OBJECTLIST::iterator iter_GameObj = Engine::Find_Layer(L"Layer_Effect")->Get_GameObject()->begin();

	if(NULL == m_pTrail)
		m_pTrail = (CTrail*)(*iter_GameObj);

	if(NULL == m_pAttackStrong0)
	{
		std::advance(iter_GameObj, 1);
		m_pAttackStrong0 = (CAttack_Strong_0*)(*iter_GameObj);
	}

	if(NULL == m_pAttackStrong2)
	{
		std::advance(iter_GameObj, 1);
		m_pAttackStrong2 = (CAttack_Strong_2*)(*iter_GameObj);
	}

	if(NULL == m_pPhantom_Trail)
	{
		std::advance(iter_GameObj, 1);
		m_pPhantom_Trail = (CPhantom_Trail*)(*iter_GameObj);
	}

	if(NULL == m_pAttackStrong3)
	{
		std::advance(iter_GameObj, 1);
		m_pAttackStrong3 = (CAttack_Strong_3*)(*iter_GameObj);
	}

	if(NULL == m_pCounter)
	{
		std::advance(iter_GameObj, 1);
		m_pCounter = (CCounter*)(*iter_GameObj);
	}

	if(NULL == m_pHeavyStanderCounter)
	{
		std::advance(iter_GameObj, 1);
		m_pHeavyStanderCounter = (CHeavyStanderCounter*)(*iter_GameObj);
	}

	if(NULL == m_pDashSmash)
	{
		std::advance(iter_GameObj, 1);
		m_pDashSmash = (CDashSmash*)(*iter_GameObj);
	}

	if(FAILED(m_pColliderCom_Foot->Compute_Collider(m_pMeshCom->Get_FrameMatrix("ValveBiped_Bip01_L_Foot"), 40.f, 40.f)))
		return E_FAIL;

	Engine::CGameObject::Update_GameObject(fTimeDelta);

	
	m_pRendererCom->Add_RenderList(Engine::CRenderer::RENDER_NONEALPHA, this);

	CollisionWhileAttack(fTimeDelta);
	Check_InputDev(m_fTimeDelta);
	CorrectPosisionWhileAttack(m_fTimeDelta);
	CancleEnable();

	CMap* pMap = (CMap*)Engine::Find_Layer(L"Layer_BackGround")->Get_GameObject()->front();

	CMap::COLLIDER::iterator iter = pMap->GetCollider()->begin();
	CMap::COLLIDER::iterator iter_end = pMap->GetCollider()->end();

	for(;iter != iter_end; ++iter)
	{
		m_bColWithMap = m_pColliderCom->Collision_OBB((*iter), Engine::CBoxCollider::CHANGE_YES);

		if(m_bColWithMap)
		{
			_vec3 vDir = _vec3(m_pTransformCom->Get_WorldMatrix()->_41, 0.f, m_pTransformCom->Get_WorldMatrix()->_43) - _vec3((*iter)->GetLocalMatrix()._41, 0.f, (*iter)->GetLocalMatrix()._43);

			D3DXVec3Normalize(&vDir, &vDir);

			m_pTransformCom->Go_Straight(2.f * m_fSpeed * fTimeDelta, vDir);

			break;
		}
	}

	return 0;
}

void CPlayer::Render_GameObject(void)
{
	LPD3DXEFFECT pEffect = m_pShaderCom->Get_EffectHandle();
	if(NULL == pEffect)
		return;

	pEffect->AddRef();

	if(FAILED(SetUp_ConstantTable(pEffect)))
		return;

	pEffect->Begin(NULL, 0);

	m_pMeshCom->Play_AnimationSet(m_fTimeDelta);
	m_pMeshCom->Render_Mesh(pEffect);

	pEffect->End();

	m_pTransformCom->SetUp_OnGraphicDev();	
	m_pColliderCom_Foot->Render_Collider(Engine::CBoxCollider::TYPE_OBB, m_pTransformCom->Get_WorldMatrix());
	m_pColliderCom->Render_Collider(Engine::CBoxCollider::TYPE_OBB, m_pTransformCom->Get_WorldMatrix());
	m_pPreHitColliderCom->Render_Collider(Engine::CBoxCollider::TYPE_OBB, m_pTransformCom->Get_WorldMatrix());

	Engine::Safe_Release(pEffect);
}

HRESULT CPlayer::Ready_Component(void)
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

	m_pMeshCom = (Engine::CDynamicMesh*)Engine::Clone_Component(SCENE_STAGE, L"Component_Mesh_Fiona");
	if(NULL == m_pMeshCom)
		return E_FAIL;
	if(FAILED(Add_Component(Engine::CGameObject::TYPE_STATIC, L"Com_Mesh", m_pMeshCom)))
		return E_FAIL;

	m_pColliderCom_Foot = (Engine::CBoxCollider*)Engine::Clone_Component(SCENE_STAGE, L"Component_BoxCollider");
	if(NULL == m_pColliderCom_Foot)
		return E_FAIL;
	if(FAILED(Add_Component(Engine::CGameObject::TYPE_STATIC, L"Com_Collider_Foot", m_pColliderCom_Foot)))
		return E_FAIL;

	m_pColliderCom = (Engine::CBoxCollider*)Engine::Clone_Component(SCENE_STAGE, L"Component_BoxCollider");
	if(NULL == m_pColliderCom)
		return E_FAIL;
	if(FAILED(m_pColliderCom->Compute_Collider(m_pMeshCom, 0)))
		return E_FAIL;
	if(FAILED(Add_Component(Engine::CGameObject::TYPE_STATIC, L"Com_Collider", m_pColliderCom)))
		return E_FAIL;

	m_pPreHitColliderCom = (Engine::CBoxCollider*)Engine::Clone_Component(SCENE_STAGE, L"Component_BoxCollider");
	if(NULL == m_pPreHitColliderCom)
		return E_FAIL;
	if(FAILED(m_pPreHitColliderCom->Compute_Collider(m_pMeshCom, 0, 3.f, 3.f, 8.f)))
		return E_FAIL;
	if(FAILED(Add_Component(Engine::CGameObject::TYPE_STATIC, L"Com_PreHitCollider", m_pPreHitColliderCom)))
		return E_FAIL;

	m_pMtrlCom = (Engine::CMaterial*)Engine::Clone_Component(SCENE_STATIC, L"Component_Material");
	if(NULL == m_pMtrlCom)
		return E_FAIL;
	if(FAILED(Add_Component(Engine::CGameObject::TYPE_STATIC, L"Com_Material", m_pMtrlCom)))
		return E_FAIL;

	m_pShaderCom = (Engine::CShader*)Engine::Clone_Component(SCENE_STAGE, L"Component_Shader_Mesh");
	if(NULL == m_pShaderCom)
		return E_FAIL;
	if(FAILED(Add_Component(Engine::CGameObject::TYPE_STATIC, L"Com_Shader", m_pShaderCom)))
		return E_FAIL;

	return S_OK;
}

void CPlayer::Check_InputDev(const _float& fTimeDelta)
{
	if((m_bStumbling && m_pMeshCom->Get_AnimationCtrl()->GetTimeAcc() <= m_dAnimationPeriod * 0.3f) || Engine::Get_DIKeyState(DIK_V) & 0x80)
	{
		//헤비스텐더 카운터
		if((Engine::Get_DIMouseState(Engine::CInput_Device::DIM_RB) & 0x80 && COUNTER != m_iState && m_bCounterEnable) || Engine::Get_DIKeyState(DIK_V) & 0x80)
		{
			m_bDown = false;
			m_iAttackNum = 0;
			m_iState = HCOUNTER;
			m_bCounterEnable = false;
			m_bStumbling = false;
			m_bPreHit = false;
			m_pMeshCom->Set_AnimationSet(52);
			m_dAnimationPeriod = m_pMeshCom->Get_AnimationCtrl()->Get_Period();
		}

		//return;
	}
	else if(m_bStumbling && m_pMeshCom->Get_AnimationCtrl()->GetTimeAcc() <= m_dAnimationPeriod && HCOUNTER != m_iState && COUNTER != m_iState)
	{
		m_iAttackNum = 0;
		m_bIsCol = false;
		m_bCounterEnable = false;
		m_bStumbling = false;
		m_iState = IDLE;
	}

	if(HCOUNTER == m_iState && m_pMeshCom->Get_AnimationCtrl()->GetTimeAcc() >= m_dAnimationPeriod * 0.1f && m_pMeshCom->Get_AnimationCtrl()->GetTimeAcc() <= m_dAnimationPeriod * 0.3f)
	{
		if(NULL != m_pHeavyStanderCounter)
			m_pHeavyStanderCounter->SetRenderEnable(true);
	}
	else if(HCOUNTER == m_iState)
	{
		if(NULL != m_pHeavyStanderCounter)
			m_pHeavyStanderCounter->SetRenderEnable(false);
	}

	if(HCOUNTER == m_iState && m_pMeshCom->Get_AnimationCtrl()->GetTimeAcc() >= m_dAnimationPeriod * 0.6f)
		m_iState = IDLE;
	/*else if(HCOUNTER == m_iState)
		return;*/

	//카운터
	if((Engine::Get_DIMouseState(Engine::CInput_Device::DIM_RB) & 0x80 && m_bCounterEnable && m_iCounterNum < 3 && HCOUNTER != m_iState && !m_bStumbling) || Engine::Get_DIKeyState(DIK_C) & 0x80)
	{
		if(0 == m_iCounterNum)
		{
			m_iAttackNum = 0;
			m_iCounterNum = 2;
			m_iState = COUNTER;
			m_bPreHit = false;
			m_pMeshCom->Set_AnimationSet(20);
			m_dAnimationPeriod = m_pMeshCom->Get_AnimationCtrl()->Get_Period();
		}
		else if(1 == m_iCounterNum)
		{
			m_bCounterEnable = false;
			m_pMeshCom->Set_AnimationSet(16);
			m_pMeshCom->Set_AnimationSet(20);
			m_dAnimationPeriod = m_pMeshCom->Get_AnimationCtrl()->Get_Period();
		}
 
		if(m_pMeshCom->Get_AnimationCtrl()->GetTimeAcc() >= m_dAnimationPeriod * 0.2f)
			m_iCounterNum = 1;

		return;
	}

	if(COUNTER == m_iState && m_pMeshCom->Get_AnimationCtrl()->GetTimeAcc() >= m_dAnimationPeriod * 0.1f && m_pMeshCom->Get_AnimationCtrl()->GetTimeAcc() <= m_dAnimationPeriod * 0.2f)
	{
		if(NULL != m_pCounter)
			m_pCounter->SetRenderEnable(true);
	}
	else if(COUNTER == m_iState)
	{
		if(NULL != m_pCounter)
			m_pCounter->SetRenderEnable(false);
	}

	if(COUNTER == m_iState && m_pMeshCom->Get_AnimationCtrl()->GetTimeAcc() >= m_dAnimationPeriod * 0.4f && HCOUNTER != m_iState && !m_bStumbling)
	{
		m_iCounterNum = 0;
		//m_bCounterEnable = false;
		m_iState = IDLE;
	}
	else if(COUNTER == m_iState)
		return;

	//가드
	if(Engine::Get_DIKeyState(DIK_SPACE) & 0x80 && DOWN != m_iState && HCOUNTER != m_iState && !m_bStumbling)
	{
		if(Engine::Get_DIKeyState(DIK_W) & 0x80 && HGUARD != m_iState)
		{
			if((ATTACK == m_iState || SMASH == m_iState) && m_bCancleEnable)
			{
				m_iState = HGUARD;
				m_bIsCol = false;
				m_bGuarding = false;
				m_iAttackNum = 0;
				m_pMeshCom->Set_AnimationSet(51);
				m_dAnimationPeriod = m_pMeshCom->Get_AnimationCtrl()->Get_Period();
			}
			else if(ATTACK != m_iState && SMASH != m_iState)
			{
				m_iState = HGUARD;
				m_bIsCol = false;
				m_bGuarding = false;
				m_iAttackNum = 0;
				m_pMeshCom->Set_AnimationSet(51);
				m_dAnimationPeriod = m_pMeshCom->Get_AnimationCtrl()->Get_Period();
			}
		}
		else if(HGUARD == m_iState && !m_bGuarding && m_pMeshCom->Get_AnimationCtrl()->GetTimeAcc() >= m_dAnimationPeriod * 0.3f)
		{
			m_bIsCol = false;
			m_bGuarding = true;
			m_pMeshCom->Set_AnimationSet(53);
		}
		else if(HGUARD != m_iState && ATTACK != m_iState && SMASH != m_iState && GUARD != m_iState)
		{
			m_iState = GUARD;
			m_bGuarding = true;
			m_bIsCol = false;
			m_pMeshCom->Set_AnimationSet(48);
		}
	}
	else if(!(Engine::Get_DIKeyState(DIK_SPACE) & 0x80) && m_bGuarding && HGUARD != m_iState && DOWN != m_iState && HCOUNTER != m_iState && !m_bStumbling)
	{
		m_iState = IDLE;
		m_bGuarding = false;
		m_bIsCol = false;
		m_bPreHit = false;
		m_pMeshCom->Set_AnimationSet(15);
	}
	else if(!(Engine::Get_DIKeyState(DIK_SPACE) & 0x80) && !(Engine::Get_DIKeyState(DIK_W) & 0x80) && HGUARD == m_iState && DOWN != m_iState && HCOUNTER != m_iState && !m_bStumbling/* && m_pMeshCom->Get_AnimationCtrl()->GetTimeAcc() >= m_dAnimationPeriod * 0.45f*/)
	{
		m_iState = IDLE;
		m_bGuarding = false;
		m_bIsCol = false;
		m_bPreHit = false;
		m_pMeshCom->Set_AnimationSet(15);
	}

	//다운
	if(0 == m_iDownState && m_bIsCol && DOWN != m_iState && !m_bDown && !m_bGuarded && HGUARD != m_iState && !m_bStumbling)
	{
		Engine::CTransform* pBossTransCom = (Engine::CTransform*)Engine::Get_Component(L"Layer_Boss", Engine::CGameObject::TYPE_DYNAMIC, L"Com_Transform");
		_vec3 vDir = _vec3(pBossTransCom->Get_WorldMatrix()->_41, pBossTransCom->Get_WorldMatrix()->_42, pBossTransCom->Get_WorldMatrix()->_43) - _vec3(m_pTransformCom->Get_WorldMatrix()->_41, m_pTransformCom->Get_WorldMatrix()->_42, m_pTransformCom->Get_WorldMatrix()->_43);
		_vec3 vPlayerLook = _vec3(m_pTransformCom->Get_WorldMatrix()->_31, 0.f, m_pTransformCom->Get_WorldMatrix()->_33);
		_vec3 vPlayerRight = _vec3(m_pTransformCom->Get_WorldMatrix()->_11, 0.f, m_pTransformCom->Get_WorldMatrix()->_13);
		_vec3 vBossRight = _vec3(pBossTransCom->Get_WorldMatrix()->_11, 0.f, pBossTransCom->Get_WorldMatrix()->_13);

		D3DXVec3Normalize(&vPlayerLook, &vPlayerLook);
		D3DXVec3Normalize(&vDir, &vDir);
		D3DXVec3Normalize(&vPlayerRight, &vPlayerRight);
		D3DXVec3Normalize(&vBossRight, &vBossRight);

		_float fIsFront = D3DXVec3Dot(&vDir, &vPlayerRight);
		_float fRad = D3DXVec3Dot(&vPlayerLook, &vBossRight);

		if(DOWN != m_iState)
		{
			m_iState = DOWN;
			m_bDown = true;
			m_iDownState = 1;
			m_iAttackNum = 0;
			m_bCounterEnable = false;
			m_bStumbling = false;
			m_fAttackedRad = D3DXToRadian(90.f) - acosf(fRad);
			m_pTransformCom->Rotation(Engine::CTransform::ANGLE_Y, -m_fAttackedRad);

			m_pMeshCom->Set_AnimationSet(25);

			if(fIsFront < 0)
				m_bAttackedFromBack = false;
			else
				m_bAttackedFromBack = true;
		}

		m_dAnimationPeriod = m_pMeshCom->Get_AnimationCtrl()->Get_Period();

		return;
	}
	else if(1 == m_iDownState && m_pMeshCom->Get_AnimationCtrl()->GetTimeAcc() >= m_dAnimationPeriod * 0.5f && !m_bStumbling)
	{
		m_iDownState = 2;
		m_pMeshCom->Set_AnimationSet(37);

		m_dAnimationPeriod = m_pMeshCom->Get_AnimationCtrl()->Get_Period();

		return;
	}
	else if(2 == m_iDownState && m_pMeshCom->Get_AnimationCtrl()->GetTimeAcc() >= m_dAnimationPeriod * 0.4f && !m_bStumbling)
	{
		m_iDownState = 3;
		m_pMeshCom->Set_AnimationSet(42);

		m_dAnimationPeriod = m_pMeshCom->Get_AnimationCtrl()->Get_Period();

		return;
	}
	else if(3 == m_iDownState && DOWN == m_iState && m_pMeshCom->Get_AnimationCtrl()->GetTimeAcc() >= m_dAnimationPeriod * 0.5f && !m_bStumbling)
	{
		m_iState = IDLE;
		m_bPreHit = false;
		m_bDown = false;
		m_bIsCol = false;
		m_bGuarded = false;
		m_bPGuard = false;
		m_iDownState = 0;
	}

	if(DOWN == m_iState)
		return;

	//왼쪽 회전
	if(Engine::Get_DIKeyState(DIK_A) & 0x80 && GUARD != m_iState && HCOUNTER != m_iState && !m_bStumbling)
	{

		// 공격중 A, W키 눌렀을 때
		if(ATTACK == m_iState || SMASH == m_iState)
		{
			if(m_pMeshCom->Get_AnimationCtrl()->GetTimeAcc() <= m_dAnimationPeriod * 0.006f)
			{
				_matrix matCamWorld;
				_vec3 vCamLook;
				_vec3 vCamRight;
				_vec3 vPlayerLook = _vec3(m_pTransformCom->Get_WorldMatrix()->_11, 0.f, m_pTransformCom->Get_WorldMatrix()->_13);
				_float fRad;
				_float fCos;

				D3DXMatrixInverse(&matCamWorld, NULL, &m_pCamera->Get_ViewMatrix());
				vCamRight = _vec3(matCamWorld._11, 0.f, matCamWorld._13);
				vCamLook = _vec3(matCamWorld._31, 0.f, matCamWorld._33);

				D3DXVec3Normalize(&vPlayerLook, &vPlayerLook);
				D3DXVec3Normalize(&vCamLook, &vCamLook);
				D3DXVec3Normalize(&vCamRight, & vCamRight);

				fCos = D3DXVec3Dot(&vPlayerLook, &vCamLook);
				fRad = acosf(D3DXVec3Dot(&vPlayerLook, &vCamRight));

				if(Engine::Get_DIKeyState(DIK_W) & 0x80)
				{
					if(fCos > 0 && !m_bAttackRotate)
					{
						m_pTransformCom->Rotation(Engine::CTransform::ANGLE_Y, -(D3DXToRadian(180.f) - fRad) * 0.5f);
						m_bAttackRotate = true;
					}
					else if(fCos <= 0 && !m_bAttackRotate)
					{
						m_pTransformCom->Rotation(Engine::CTransform::ANGLE_Y, (D3DXToRadian(180.f) - fRad) * 0.5f);
						m_bAttackRotate = true;
					}
				}
				else
				{
					if(fCos > 0 && !m_bAttackRotate)
					{
						m_pTransformCom->Rotation(Engine::CTransform::ANGLE_Y, -(D3DXToRadian(180.f) - fRad));
						m_bAttackRotate = true;
					}
					else if(fCos <= 0 && !m_bAttackRotate)
					{
						m_pTransformCom->Rotation(Engine::CTransform::ANGLE_Y, (D3DXToRadian(180.f) - fRad));
						m_bAttackRotate = true;
					}
				}
			}
			else
				m_bAttackRotate = false;
		}
		else
		{
			if(m_bBack)
				m_pTransformCom->Rotation(Engine::CTransform::ANGLE_Y, D3DXToRadian(90.0f) * fTimeDelta * 1.5f);
			else
				m_pTransformCom->Rotation(Engine::CTransform::ANGLE_Y, -D3DXToRadian(90.0f) * fTimeDelta * 1.5f);
		}
	}

	//오른쪽 회전
	if(Engine::Get_DIKeyState(DIK_D) & 0x80 && GUARD != m_iState && HCOUNTER != m_iState && !m_bStumbling)
	{
		// 공격중 D, W키 눌렀을 때
		if(ATTACK == m_iState || SMASH == m_iState)
		{
			if(m_pMeshCom->Get_AnimationCtrl()->GetTimeAcc() <= m_dAnimationPeriod * 0.006f)
			{
				_matrix matCamWorld;
				_vec3 vCamLook;
				_vec3 vCamRight;
				_vec3 vPlayerLook = _vec3(m_pTransformCom->Get_WorldMatrix()->_11, 0.f, m_pTransformCom->Get_WorldMatrix()->_13);
				_float fRad;
				_float fCos;

				D3DXMatrixInverse(&matCamWorld, NULL, &m_pCamera->Get_ViewMatrix());
				vCamRight = _vec3(matCamWorld._11, 0.f, matCamWorld._13);
				vCamLook = _vec3(matCamWorld._31, 0.f, matCamWorld._33);

				D3DXVec3Normalize(&vPlayerLook, &vPlayerLook);
				D3DXVec3Normalize(&vCamLook, &vCamLook);
				D3DXVec3Normalize(&vCamRight, & vCamRight);

				fCos = D3DXVec3Dot(&vPlayerLook, &vCamLook);
				fRad = acosf(D3DXVec3Dot(&vPlayerLook, &vCamRight));

				if(Engine::Get_DIKeyState(DIK_W) & 0x80)
				{
					if(fCos > 0 && !m_bAttackRotate)
					{
						m_pTransformCom->Rotation(Engine::CTransform::ANGLE_Y, fRad * 0.5f);
						m_bAttackRotate = true;
					}
					else if(fCos <= 0 && !m_bAttackRotate)
					{
						m_pTransformCom->Rotation(Engine::CTransform::ANGLE_Y, -fRad * 0.5f);
						m_bAttackRotate = true;
					}
				}
				else
				{
					if(fCos > 0 && !m_bAttackRotate)
					{
						m_pTransformCom->Rotation(Engine::CTransform::ANGLE_Y, fRad);
						m_bAttackRotate = true;
					}
					else if(fCos <= 0 && !m_bAttackRotate)
					{
						m_pTransformCom->Rotation(Engine::CTransform::ANGLE_Y, -fRad);
						m_bAttackRotate = true;
					}
				}
			}
			else
				m_bAttackRotate = false;
		}
		else
		{
			if(m_bBack)
				m_pTransformCom->Rotation(Engine::CTransform::ANGLE_Y, -D3DXToRadian(90.0f) * fTimeDelta * 1.5f);
			else
				m_pTransformCom->Rotation(Engine::CTransform::ANGLE_Y, D3DXToRadian(90.0f) * fTimeDelta * 1.5f);
		}
	}

	//디버그
	/*g_fDebugTime += m_fTimeDelta;
	if(g_fDebugTime > 0.5f)
	{
		system("cls");

		g_fDebugTime = 0.f;
	}*/

	//공격 중 W키 눌렀을 때
	if(Engine::Get_DIKeyState(DIK_W) & 0x80 && (ATTACK == m_iState || SMASH == m_iState))
	{
		if(m_pMeshCom->Get_AnimationCtrl()->GetTimeAcc() <= m_dAnimationPeriod * 0.006f)
		{
			_matrix matCamWorld;
			_vec3 vCamLook;
			_vec3 vCamRight;
			_vec3 vPlayerLook = _vec3(m_pTransformCom->Get_WorldMatrix()->_11, 0.f, m_pTransformCom->Get_WorldMatrix()->_13);
			_float fRad;
			_float fCos;

			D3DXMatrixInverse(&matCamWorld, NULL, &m_pCamera->Get_ViewMatrix());
			vCamRight = _vec3(matCamWorld._11, 0.f, matCamWorld._13);
			vCamLook = _vec3(matCamWorld._31, 0.f, matCamWorld._33);

			D3DXVec3Normalize(&vPlayerLook, &vPlayerLook);
			D3DXVec3Normalize(&vCamLook, &vCamLook);
			D3DXVec3Normalize(&vCamRight, & vCamRight);

			fCos = D3DXVec3Dot(&vPlayerLook, &vCamRight);
			fRad = acosf(D3DXVec3Dot(&vPlayerLook, &vCamLook));

			if(fCos > 0)
			{
				m_pTransformCom->Rotation(Engine::CTransform::ANGLE_Y, -fRad);
				m_bAttackRotate = true;
			}
			else
			{
				m_pTransformCom->Rotation(Engine::CTransform::ANGLE_Y, fRad);
				m_bAttackRotate = true;
			}
		}
		else
			m_bAttackRotate = false;
	}

	//전력질주
	if(Engine::Get_DIKeyState(DIK_LSHIFT) & 0x80 && Engine::Get_DIKeyState(DIK_W) & 0x80 && ATTACK != m_iState && SPRINTATT != m_iState && SMASH != m_iState && GUARD != m_iState && HCOUNTER != m_iState && !m_bStumbling)
	{
		if(Engine::Get_DIMouseState(Engine::CInput_Device::DIM_LB) & 0x80)
			m_iState = ATTACK;

		if(Engine::Get_DIMouseState(Engine::CInput_Device::DIM_RB) & 0x80)
			m_iState = SPRINTATT;

		m_bCounterEnable = false;

		m_fSpeedOffSet = 1.8f;

		m_pTransformCom->Go_StraightForPlayer(m_fSpeed * m_fSpeedOffSet * fTimeDelta);

		if(SPRINT == m_iState)
			return;

		if(SPRINTATT != m_iState)
			m_iState = SPRINT;

		m_pMeshCom->Set_AnimationSet(17);
	}
	else if(RUN != m_iState && ATTACK != m_iState && SPRINTATT != m_iState && SMASH != m_iState && GUARD != m_iState && HGUARD != m_iState && HCOUNTER != m_iState && !m_bStumbling)
		m_iState = IDLE;

	//전진
	if(Engine::Get_DIKeyState(DIK_W) & 0x80 && SPRINT != m_iState && ATTACK != m_iState && SPRINTATT != m_iState && SMASH != m_iState && GUARD != m_iState && HGUARD != m_iState && HCOUNTER != m_iState && !m_bStumbling)
	{
		if(m_bBack)
			m_pTransformCom->Rotation(Engine::CTransform::ANGLE_Y, D3DXToRadian(180.f));

		if(m_bBack)
			m_bBack = false;

		if(Engine::Get_DIMouseState(Engine::CInput_Device::DIM_LB) & 0x80)
		{
			m_iState = ATTACK;

			m_pMeshCom->Set_AnimationSet(1);
		}

		if(Engine::Get_DIMouseState(Engine::CInput_Device::DIM_RB) & 0x80)
		{
			m_iState = SMASH;

			m_pMeshCom->Set_AnimationSet(6);
		}

		m_bCounterEnable = false;

		m_fSpeedOffSet = 1.f;

		m_pTransformCom->Go_StraightForPlayer(m_fSpeed * m_fSpeedOffSet * fTimeDelta);
		if(RUN == m_iState)
			return;

		if(SPRINTATT != m_iState)
			m_iState = RUN;

		m_pMeshCom->Set_AnimationSet(16);
	}
	else if(SPRINT != m_iState && ATTACK != m_iState && SPRINTATT != m_iState && SMASH != m_iState && false == m_bBack && GUARD != m_iState && HGUARD != m_iState && HCOUNTER != m_iState && !m_bStumbling)
	{
		m_pMeshCom->Set_AnimationSet(15);
		m_iState = IDLE;
	}

	//후진
	if(Engine::Get_DIKeyState(DIK_S) & 0x80 && SPRINT != m_iState && ATTACK != m_iState && SPRINTATT != m_iState && SMASH != m_iState && GUARD != m_iState && HGUARD != m_iState && HCOUNTER != m_iState && !m_bStumbling)
	{
		if(!m_bBack)
			m_pTransformCom->Rotation(Engine::CTransform::ANGLE_Y, D3DXToRadian(180.f));

		if(!m_bBack)
			m_bBack = true;

		if(Engine::Get_DIMouseState(Engine::CInput_Device::DIM_LB) & 0x80)
		{
			m_iState = ATTACK;

			m_pMeshCom->Set_AnimationSet(1);
		}

		if(Engine::Get_DIMouseState(Engine::CInput_Device::DIM_RB) & 0x80)
		{
			m_iState = SMASH;

			m_pMeshCom->Set_AnimationSet(6);
		}

		m_bCounterEnable = false;

		m_fSpeedOffSet = 1.f;

		m_pTransformCom->Go_StraightForPlayer(m_fSpeed * m_fSpeedOffSet * fTimeDelta);
		if(RUN == m_iState)
			return;

		if(SPRINTATT != m_iState)
			m_iState = RUN;

		m_pMeshCom->Set_AnimationSet(16);
	}
	else if(SPRINT != m_iState && ATTACK != m_iState && SPRINTATT != m_iState && SMASH != m_iState && true == m_bBack && GUARD != m_iState && HGUARD != m_iState && HCOUNTER != m_iState && !m_bStumbling)
	{
		m_pMeshCom->Set_AnimationSet(15);
		m_iState = IDLE;
	}

	//일반공격
	if(Engine::Get_DIMouseState(Engine::CInput_Device::DIM_LB) & 0x80 && GUARD != m_iState && HGUARD != m_iState && COUNTER != m_iState && HCOUNTER != m_iState && !m_bStumbling)
	{
		m_bCounterEnable = false;

		if(ATTACK == m_iState && 2 == m_iAttackNum && m_pMeshCom->Get_AnimationCtrl()->GetTimeAcc() <= m_dAnimationPeriod * 0.25f)
			return;
		else if(ATTACK == m_iState && m_pMeshCom->Get_AnimationCtrl()->GetTimeAcc() <= m_dAnimationPeriod * 0.2f)
			return;
		else if(SMASH == m_iState && m_pMeshCom->Get_AnimationCtrl()->GetTimeAcc() <= m_dAnimationPeriod * 0.3f)
			return;

		m_iState = ATTACK;
		m_bCounterEnable = false;

		if(0 == m_iAttackNum)
		{
			m_pMeshCom->Set_AnimationSet(1);
			++m_iAttackNum;
		}
		else if(1 == m_iAttackNum)
		{
			m_pMeshCom->Set_AnimationSet(2);
			++m_iAttackNum;
		}
		else if(2 == m_iAttackNum)
		{
			m_pMeshCom->Set_AnimationSet(3);
			++m_iAttackNum;
		}
		else if(3 == m_iAttackNum)
		{
			m_pMeshCom->Set_AnimationSet(4);
			++m_iAttackNum;
		}

		m_dAnimationPeriod = m_pMeshCom->Get_AnimationCtrl()->Get_Period();
	}
	else if(4 != m_iAttackNum && ATTACK == m_iState && m_pMeshCom->Get_AnimationCtrl()->GetTimeAcc() >= m_dAnimationPeriod * 0.5f && HCOUNTER != m_iState && !m_bStumbling)
	{
		m_iState = IDLE;
		m_iAttackNum = 0;
	}
	else if(4 == m_iAttackNum && ATTACK == m_iState && m_pMeshCom->Get_AnimationCtrl()->GetTimeAcc() >= m_dAnimationPeriod * 0.6f && HCOUNTER != m_iState && !m_bStumbling)
	{
		m_iState = IDLE;
		m_iAttackNum = 0;
	}
	
	if(1 == m_iAttackNum && ATTACK == m_iState && m_pMeshCom->Get_AnimationCtrl()->GetTimeAcc() >= m_dAnimationPeriod * 0.12f && m_pMeshCom->Get_AnimationCtrl()->GetTimeAcc() <= m_dAnimationPeriod * 0.18f && HCOUNTER != m_iState && !m_bStumbling)
		m_pTrail->SetRenderEnable(true);
	else if(2 == m_iAttackNum && ATTACK == m_iState && m_pMeshCom->Get_AnimationCtrl()->GetTimeAcc() >= m_dAnimationPeriod * 0.15f && m_pMeshCom->Get_AnimationCtrl()->GetTimeAcc() <= m_dAnimationPeriod * 0.21f && HCOUNTER != m_iState && !m_bStumbling)
		m_pTrail->SetRenderEnable(true);
	else if(3 == m_iAttackNum && ATTACK == m_iState && m_pMeshCom->Get_AnimationCtrl()->GetTimeAcc() >= m_dAnimationPeriod * 0.09f && m_pMeshCom->Get_AnimationCtrl()->GetTimeAcc() <= m_dAnimationPeriod * 0.15f && HCOUNTER != m_iState && !m_bStumbling)
		m_pTrail->SetRenderEnable(true);
	else if(4 == m_iAttackNum && ATTACK == m_iState && m_pMeshCom->Get_AnimationCtrl()->GetTimeAcc() >= m_dAnimationPeriod * 0.20f && m_pMeshCom->Get_AnimationCtrl()->GetTimeAcc() <= m_dAnimationPeriod * 0.26f && HCOUNTER != m_iState && !m_bStumbling)
		m_pTrail->SetRenderEnable(true);
	else if(NULL != m_pTrail)
		m_pTrail->SetRenderEnable(false);

	//스메쉬
	if(Engine::Get_DIMouseState(Engine::CInput_Device::DIM_RB) & 0x80 && GUARD != m_iState && HGUARD != m_iState && COUNTER != m_iState && HCOUNTER != m_iState && !m_bStumbling)
	{
		m_bCounterEnable = false;

		if(ATTACK == m_iState && m_pMeshCom->Get_AnimationCtrl()->GetTimeAcc() * 4.f <= m_dAnimationPeriod)
			return;
		else if(SMASH == m_iState && m_pMeshCom->Get_AnimationCtrl()->GetTimeAcc() * 2.f <= m_dAnimationPeriod)
			return;

		if(SPRINTATT == m_iState)
		{
			m_pMeshCom->Set_AnimationSet(5);
			m_iAttackNum = 0;
		}
		else if(0 == m_iAttackNum)
		{
			m_pMeshCom->Set_AnimationSet(6);
			m_iSmashNum = 0;
		}
		else if(1 == m_iAttackNum)
		{
			m_pMeshCom->Set_AnimationSet(6);
			m_iAttackNum = 0;
			m_iSmashNum = 1;
		}
		else if(2 == m_iAttackNum)
		{
			m_pMeshCom->Set_AnimationSet(9);
			m_iAttackNum = 0;
			m_iSmashNum = 2;
		}
		else if(3 == m_iAttackNum)
		{
			m_pMeshCom->Set_AnimationSet(10);
			m_iAttackNum = 0;
			m_iSmashNum = 3;
		}
		else if(4 == m_iAttackNum)
		{
			m_pMeshCom->Set_AnimationSet(14);
			m_iAttackNum = 0;
			m_iSmashNum = 4;
		}

		if(SPRINTATT != m_iState)
			m_iState = SMASH;

		m_bCounterEnable = false;
		m_dAnimationPeriod = m_pMeshCom->Get_AnimationCtrl()->Get_Period();
	}
	else if(SMASH == m_iState && m_pMeshCom->Get_AnimationCtrl()->GetTimeAcc() >= m_dAnimationPeriod * 0.5f && HCOUNTER != m_iState && !m_bStumbling)
	{
		m_iState = IDLE;
		m_iAttackNum = 0;
		m_iSmashNum = 0;
	}
	else if(SPRINTATT == m_iState && m_pMeshCom->Get_AnimationCtrl()->GetTimeAcc() >= m_dAnimationPeriod * 0.55f && HCOUNTER != m_iState && !m_bStumbling)
	{
		m_iState = IDLE;
		m_iAttackNum = 0;
		m_iSmashNum = 0;
	}

	if(SMASH == m_iState && (0 == m_iSmashNum || 1 == m_iSmashNum) && m_pMeshCom->Get_AnimationCtrl()->GetTimeAcc() >= m_dAnimationPeriod * 0.14f && m_pMeshCom->Get_AnimationCtrl()->GetTimeAcc() <= m_dAnimationPeriod * 0.25f)
		m_pAttackStrong0->SetRenderEnable(true);
	else if(SMASH == m_iState && (0 == m_iSmashNum || 1 == m_iSmashNum))
		m_pAttackStrong0->SetRenderEnable(false);

	if(SMASH == m_iState && 2 == m_iSmashNum && m_pMeshCom->Get_AnimationCtrl()->GetTimeAcc() >= m_dAnimationPeriod * 0.08f && m_pMeshCom->Get_AnimationCtrl()->GetTimeAcc() <= m_dAnimationPeriod * 0.16f)
		m_pPhantom_Trail->SetRenderEnable(true);
	else if(SMASH == m_iState && 2 == m_iSmashNum)
		m_pPhantom_Trail->SetRenderEnable(false);

	if(SMASH == m_iState && 3 == m_iSmashNum && m_pMeshCom->Get_AnimationCtrl()->GetTimeAcc() >= m_dAnimationPeriod * 0.2f && m_pMeshCom->Get_AnimationCtrl()->GetTimeAcc() <= m_dAnimationPeriod * 0.4f)
		m_pAttackStrong2->SetRenderEnable(true);
	else if(SMASH == m_iState && 3 == m_iSmashNum)
		m_pAttackStrong2->SetRenderEnable(false);

	if(SMASH == m_iState && 4 == m_iSmashNum && m_pMeshCom->Get_AnimationCtrl()->GetTimeAcc() >= m_dAnimationPeriod * 0.2f && m_pMeshCom->Get_AnimationCtrl()->GetTimeAcc() <= m_dAnimationPeriod * 0.3f)
		m_pAttackStrong3->SetRenderEnable(true);
	else if(SMASH == m_iState && 4 == m_iSmashNum)
		m_pAttackStrong3->SetRenderEnable(false);

	if(SPRINTATT == m_iState && m_pMeshCom->Get_AnimationCtrl()->GetTimeAcc() >= m_dAnimationPeriod * 0.1f && m_pMeshCom->Get_AnimationCtrl()->GetTimeAcc() <= m_dAnimationPeriod * 0.17f)
		m_pDashSmash->SetRenderEnable(true);
	else if(SPRINTATT == m_iState)
		m_pDashSmash->SetRenderEnable(false);

	if(SMASH != m_iState)
	{
		m_pAttackStrong0->SetRenderEnable(false);
		m_pAttackStrong2->SetRenderEnable(false);
		m_pAttackStrong3->SetRenderEnable(false);
		m_pPhantom_Trail->SetRenderEnable(false);
		m_pAttackStrong3->SetRenderEnable(false);
	}
}

void CPlayer::CorrectPosisionWhileAttack(const _float& fTimeDelta)
{
	if(ATTACK == m_iState && m_pMeshCom->Get_AnimationCtrl()->GetTimeAcc() < 0.5)
	{
		m_fSpeedOffSet = 0.47f;
		m_pTransformCom->Go_StraightForPlayer(m_fSpeed * m_fSpeedOffSet * fTimeDelta);
	}
	else if(ATTACK == m_iState && m_pMeshCom->Get_AnimationCtrl()->GetTimeAcc() > 1.3 && m_pMeshCom->Get_AnimationCtrl()->GetTimeAcc() < 1.8)
	{
		m_fSpeedOffSet = 0.33f;
		m_pTransformCom->Go_StraightForPlayer(m_fSpeed * m_fSpeedOffSet * fTimeDelta);
	}

	if(SMASH == m_iState && (0 == m_iSmashNum || 1 == m_iSmashNum || 2 == m_iSmashNum) && m_pMeshCom->Get_AnimationCtrl()->GetTimeAcc() < 0.5)
	{
		m_fSpeedOffSet = 0.47f;
		m_pTransformCom->Go_StraightForPlayer(m_fSpeed * m_fSpeedOffSet * fTimeDelta);
	}

	if(SMASH == m_iState && 3 == m_iSmashNum && m_pMeshCom->Get_AnimationCtrl()->GetTimeAcc() > 0.4 && m_pMeshCom->Get_AnimationCtrl()->GetTimeAcc() < 0.5)
	{
		m_fSpeedOffSet = 1.f;
		m_pTransformCom->Go_StraightForPlayer(m_fSpeed * m_fSpeedOffSet * fTimeDelta);
	}

	if(SMASH == m_iState && 4 == m_iSmashNum && m_pMeshCom->Get_AnimationCtrl()->GetTimeAcc() > 0.65 && m_pMeshCom->Get_AnimationCtrl()->GetTimeAcc() < 0.8)
	{
		m_fSpeedOffSet = 1.f;
		m_pTransformCom->Go_StraightForPlayer(m_fSpeed * m_fSpeedOffSet * fTimeDelta);
	}

	if(DOWN == m_iState && 1 == m_iDownState && m_pMeshCom->Get_AnimationCtrl()->GetTimeAcc() < 0.5)
	{
		Engine::CTransform* pBossTransCom = (Engine::CTransform*)Engine::Get_Component(L"Layer_Boss", Engine::CGameObject::TYPE_DYNAMIC, L"Com_Transform");
		D3DXVECTOR3 vBossLook = _vec3(-pBossTransCom->Get_WorldMatrix()->_11, 0.f, -pBossTransCom->Get_WorldMatrix()->_13);

		if(!m_bAttackedFromBack)
		{
			m_pTransformCom->Rotation(Engine::CTransform::ANGLE_Y, D3DXToRadian(180.f));
			m_bAttackedFromBack = true;
		}

		m_fSpeedOffSet = 1.33f;
		m_pTransformCom->Go_Straight(fTimeDelta * m_fSpeed * m_fSpeedOffSet, vBossLook);
	}

	if(DOWN == m_iState && 3 == m_iDownState && m_pMeshCom->Get_AnimationCtrl()->GetTimeAcc() > 0.55 && m_pMeshCom->Get_AnimationCtrl()->GetTimeAcc() < 1.5)
	{
		_matrix matCamWorld;
		_vec3 vCamLook;
		_vec3 vCamRight;
		_vec3 vPlayerLook = _vec3(m_pTransformCom->Get_WorldMatrix()->_11, 0.f, m_pTransformCom->Get_WorldMatrix()->_13);
		_float fRad;
		_float fCos;

		D3DXMatrixInverse(&matCamWorld, NULL, &m_pCamera->Get_ViewMatrix());
		vCamRight = _vec3(matCamWorld._11, 0.f, matCamWorld._13);
		vCamLook = _vec3(matCamWorld._31, 0.f, matCamWorld._33);

		D3DXVec3Normalize(&vPlayerLook, &vPlayerLook);
		D3DXVec3Normalize(&vCamLook, &vCamLook);
		D3DXVec3Normalize(&vCamRight, & vCamRight);

		fCos = D3DXVec3Dot(&vPlayerLook, &vCamLook);
		fRad = acosf(D3DXVec3Dot(&vPlayerLook, &vCamRight));

		if(Engine::Get_DIKeyState(DIK_A))
		{
			if(fCos > 0 && !m_bDownRotate)
			{
				m_pTransformCom->Rotation(Engine::CTransform::ANGLE_Y, -(D3DXToRadian(180.f) - fRad));
				m_bDownRotate = true;
			}
			else if(fCos <= 0 && !m_bDownRotate)
			{
				m_pTransformCom->Rotation(Engine::CTransform::ANGLE_Y, (D3DXToRadian(180.f) - fRad));
				m_bDownRotate = true;
			}
		}
		else if(Engine::Get_DIKeyState(DIK_D))
		{
			if(fCos > 0 && !m_bDownRotate)
			{
				m_pTransformCom->Rotation(Engine::CTransform::ANGLE_Y, fRad);
				m_bDownRotate = true;
			}
			else if(fCos <= 0 && !m_bDownRotate)
			{
				m_pTransformCom->Rotation(Engine::CTransform::ANGLE_Y, -fRad);
				m_bDownRotate = true;
			}
		}

		m_fSpeedOffSet = 1.33f;
		m_pTransformCom->Go_StraightForPlayer(m_fSpeed * m_fSpeedOffSet * fTimeDelta);
	}
	else
		m_bDownRotate = false;

	if(m_bStumbling)
	{
		Engine::CTransform* pBossTransCom = (Engine::CTransform*)Engine::Get_Component(L"Layer_Boss", Engine::CGameObject::TYPE_DYNAMIC, L"Com_Transform");
		D3DXVECTOR3 vBossLook = _vec3(-pBossTransCom->Get_WorldMatrix()->_11, 0.f, -pBossTransCom->Get_WorldMatrix()->_13);
		_vec3 vDir = _vec3(pBossTransCom->Get_WorldMatrix()->_41, pBossTransCom->Get_WorldMatrix()->_42, pBossTransCom->Get_WorldMatrix()->_43) - _vec3(m_pTransformCom->Get_WorldMatrix()->_41, m_pTransformCom->Get_WorldMatrix()->_42, m_pTransformCom->Get_WorldMatrix()->_43);
		_vec3 vPlayerLook = _vec3(m_pTransformCom->Get_WorldMatrix()->_31, 0.f, m_pTransformCom->Get_WorldMatrix()->_33);
		_vec3 vPlayerRight = _vec3(m_pTransformCom->Get_WorldMatrix()->_11, 0.f, m_pTransformCom->Get_WorldMatrix()->_13);
		_vec3 vBossRight = _vec3(pBossTransCom->Get_WorldMatrix()->_11, 0.f, pBossTransCom->Get_WorldMatrix()->_13);

		D3DXVec3Normalize(&vPlayerLook, &vPlayerLook);
		D3DXVec3Normalize(&vDir, &vDir);
		D3DXVec3Normalize(&vPlayerRight, &vPlayerRight);
		D3DXVec3Normalize(&vBossRight, &vBossRight);

		_float fIsFront = D3DXVec3Dot(&vDir, &vPlayerRight);
		_float fRad = D3DXVec3Dot(&vPlayerLook, &vBossRight);

		m_fAttackedRad = D3DXToRadian(90.f) - acosf(fRad);
		m_pTransformCom->Rotation(Engine::CTransform::ANGLE_Y, -m_fAttackedRad);

		if(fIsFront < 0)
			m_bAttackedFromBack = false;
		else
			m_bAttackedFromBack = true;

		if(!m_bAttackedFromBack)
		{
			m_pTransformCom->Rotation(Engine::CTransform::ANGLE_Y, D3DXToRadian(180.f));
			m_bAttackedFromBack = true;
		}

		if(m_pMeshCom->Get_AnimationCtrl()->GetTimeAcc() <= m_dAnimationPeriod * 0.2f)
			m_pTransformCom->Go_Straight(fTimeDelta * 1.5f, vBossLook);
	}

	if(HCOUNTER == m_iState)
	{
		if(m_pMeshCom->Get_AnimationCtrl()->GetTimeAcc() >= m_dAnimationPeriod * 0.08f && m_pMeshCom->Get_AnimationCtrl()->GetTimeAcc() <= m_dAnimationPeriod * 0.25f)
		{
			m_fSpeedOffSet = 2.f;
			m_pTransformCom->Go_StraightForPlayer(fTimeDelta * m_fSpeed * m_fSpeedOffSet);
		}
		else if(m_pMeshCom->Get_AnimationCtrl()->GetTimeAcc() >= m_dAnimationPeriod * 0.25f && m_pMeshCom->Get_AnimationCtrl()->GetTimeAcc() <= m_dAnimationPeriod * 0.27f)
		{
			m_fSpeedOffSet = 0.66f;
			m_pTransformCom->Go_StraightForPlayer(fTimeDelta * m_fSpeed * m_fSpeedOffSet);
		}
	}
}

void CPlayer::CollisionWhileAttack(const _float& fTimeDelta)
{
	Engine::CBoxCollider* pSwordColliderCom = (Engine::CBoxCollider*)Engine::Get_Component(L"Layer_Equipment", Engine::CGameObject::TYPE_STATIC, L"Com_Collider");
	Engine::CBoxCollider* pShieldColliderCom = (Engine::CBoxCollider*)Engine::Get_Component(L"Layer_Equipment", Engine::CGameObject::TYPE_STATIC, L"Com_Collider", 1);
	Engine::CBoxCollider* pBossColliderCom = (Engine::CBoxCollider*)Engine::Get_Component(L"Layer_Boss", Engine::CGameObject::TYPE_STATIC, L"Com_Collider");
	Engine::CBoxCollider* pBossSwordColliderCom = (Engine::CBoxCollider*)Engine::Get_Component(L"Layer_Boss", Engine::CGameObject::TYPE_STATIC, L"Com_Collider", 1);
	Engine::CTransform* pBossTransform = (Engine::CTransform*)Engine::Get_Component(L"Layer_Boss", Engine::CGameObject::TYPE_DYNAMIC, L"Com_Transform");
	CBoss* pBoss = (CBoss*)Engine::Find_Layer(L"Layer_Boss")->Get_GameObject()->front();

	if(m_pColliderCom->Collision_OBB(pBossColliderCom, Engine::CBoxCollider::CHANGE_NO) && DOWN != m_iState)
	{
		_vec3 vDir = _vec3(pBossTransform->Get_WorldMatrix()->_41, pBossTransform->Get_WorldMatrix()->_42, pBossTransform->Get_WorldMatrix()->_43) - _vec3(m_pTransformCom->Get_WorldMatrix()->_41, m_pTransformCom->Get_WorldMatrix()->_42, m_pTransformCom->Get_WorldMatrix()->_43);

		m_pTransformCom->Go_Straight(-m_fSpeed * m_fSpeedOffSet * fTimeDelta, vDir);
	}

	//디버그
	/*g_fDebugTime += m_fTimeDelta;
	if(g_fDebugTime > 0.5f)
	{
		system("cls");
		cout << "m_bGarded : " <<m_bGuarded << endl;
		cout << "m_bIsCol : " << m_bIsCol << endl;
		cout << "m_iState : " << m_iState << endl;
		cout << "m_bDown : " << m_bDown << endl;
		cout << "m_iDownState : " << m_iDownState << endl;
		cout << "m_bPreHit : " << m_bPreHit << endl;
		cout << "m_bPGuard : " << m_bPGuard << endl;

		g_fDebugTime = 0.f;
	}*/

	if(ATTACK == m_iState && 4 != m_iState && m_pMeshCom->Get_AnimationCtrl()->GetTimeAcc() > m_dAnimationPeriod * 0.1f && m_pMeshCom->Get_AnimationCtrl()->GetTimeAcc() < m_dAnimationPeriod * 0.15f)
	{
		if(pBossColliderCom->Collision_OBB(pSwordColliderCom, Engine::CBoxCollider::CHANGE_YES))
			pBoss->SetIsCol(true, m_fDamage, m_fGroggyDamage, fTimeDelta);
	}
	else if(ATTACK == m_iState && 4 == m_iAttackNum && m_pMeshCom->Get_AnimationCtrl()->GetTimeAcc() > m_dAnimationPeriod * 0.2f && m_pMeshCom->Get_AnimationCtrl()->GetTimeAcc() < m_dAnimationPeriod * 0.25f)
	{
		if(pBossColliderCom->Collision_OBB(pSwordColliderCom, Engine::CBoxCollider::CHANGE_YES))
			pBoss->SetIsCol(true, m_fDamage, m_fGroggyDamage, fTimeDelta);
	}
	else if(SMASH == m_iState && 4 == m_iSmashNum && m_pMeshCom->Get_AnimationCtrl()->GetTimeAcc() > m_dAnimationPeriod * 0.2f && m_pMeshCom->Get_AnimationCtrl()->GetTimeAcc() < m_dAnimationPeriod * 0.25f)
	{
		if(pBossColliderCom->Collision_OBB(m_pColliderCom_Foot, Engine::CBoxCollider::CHANGE_YES))
			pBoss->SetIsCol(true, m_fDamage * 5.f, m_fGroggyDamage * 10.f, fTimeDelta);
	}
	else if(SMASH == m_iState && 3 == m_iSmashNum && m_pMeshCom->Get_AnimationCtrl()->GetTimeAcc() > m_dAnimationPeriod * 0.2f && m_pMeshCom->Get_AnimationCtrl()->GetTimeAcc() < m_dAnimationPeriod * 0.25f)
	{
		if(pBossColliderCom->Collision_OBB(pShieldColliderCom, Engine::CBoxCollider::CHANGE_YES))
			pBoss->SetIsCol(true, m_fDamage * 3.f, m_fGroggyDamage * 6.f, fTimeDelta);
	}
	else if(SMASH == m_iState && 3 != m_iSmashNum && 4 != m_iSmashNum && m_pMeshCom->Get_AnimationCtrl()->GetTimeAcc() > m_dAnimationPeriod * 0.1f && m_pMeshCom->Get_AnimationCtrl()->GetTimeAcc() < m_dAnimationPeriod * 0.15f)
	{
		if(pBossColliderCom->Collision_OBB(pSwordColliderCom, Engine::CBoxCollider::CHANGE_YES))
		{
			if(0 == m_iSmashNum)
				pBoss->SetIsCol(true, m_fDamage * 1.5f, m_fGroggyDamage * 3.f, fTimeDelta);
			else if(1 == m_iSmashNum)
				pBoss->SetIsCol(true, m_fDamage * 2.0f, m_fGroggyDamage * 4.f, fTimeDelta);
			else if(2 == m_iSmashNum)
				pBoss->SetIsCol(true, m_fDamage * 2.5f, m_fGroggyDamage * 5.f, fTimeDelta);
		}
	}
	else if(SPRINTATT == m_iState && m_pMeshCom->Get_AnimationCtrl()->GetTimeAcc() > m_dAnimationPeriod * 0.1f && m_pMeshCom->Get_AnimationCtrl()->GetTimeAcc() < m_dAnimationPeriod * 0.15f)
	{
		if(pBossColliderCom->Collision_OBB(pSwordColliderCom, Engine::CBoxCollider::CHANGE_YES))
			pBoss->SetIsCol(true, m_fDamage, m_fGroggyDamage * 2.f, fTimeDelta);
	}
	
	if(GUARD == m_iState)
	{
		if(CBoss::SMASH == pBoss->GetAttState())
		{
			if(m_bPreHit && !m_bPGuard)
			{
				m_bGuarded = true;
				m_bPGuard = true;
				m_bPreHit = false;
				m_bCounterEnable = true;
				m_pMeshCom->Set_AnimationSet(50);
				m_dAnimationPeriod = m_pMeshCom->Get_AnimationCtrl()->Get_Period();
			}
			else
				m_bPGuard = false;

			if(m_bPGuard && m_pMeshCom->Get_AnimationCtrl()->GetTimeAcc() > m_dAnimationPeriod * 0.5f)
			{
				m_bGuarded = false;
				m_bPGuard = false;
				m_iState = IDLE;
				m_pMeshCom->Set_AnimationSet(48);
			}
		}
		else if(m_bIsCol)
		{
			m_bGuarded = true;
			m_bCounterEnable = true;
			m_pMeshCom->Set_AnimationSet(50);
			m_dAnimationPeriod = m_pMeshCom->Get_AnimationCtrl()->Get_Period();
		}

		if(m_bGuarded && m_pMeshCom->Get_AnimationCtrl()->GetTimeAcc() > m_dAnimationPeriod * 0.5f)
		{
			m_bGuarded = false;
			m_pMeshCom->Set_AnimationSet(48);
		}
	}
	else if(!m_bIsCol)
		m_bGuarded = false;

	if(HGUARD == m_iState)
	{
		if(m_bIsCol)
		{
			if(CBoss::SMASH == pBoss->GetAttState())
			{
				m_bStumbling = true;
				m_bCounterEnable = true;
				m_bIsCol = false;
				m_pMeshCom->Set_AnimationSet(54);
				m_dAnimationPeriod = m_pMeshCom->Get_AnimationCtrl()->Get_Period();
			}
			else
				m_pMeshCom->Set_AnimationSet(55);
		}
	}

	if(COUNTER == m_iState && m_pMeshCom->Get_AnimationCtrl()->GetTimeAcc() > m_dAnimationPeriod * 0.1f && m_pMeshCom->Get_AnimationCtrl()->GetTimeAcc() < m_dAnimationPeriod * 0.15f)
	{
		if(pBossColliderCom->Collision_OBB(pShieldColliderCom, Engine::CBoxCollider::CHANGE_YES))
			pBoss->SetIsCol(true, m_fDamage * 4.f, m_fGroggyDamage * 8.f, fTimeDelta);
	}

	if(HCOUNTER == m_iState && m_pMeshCom->Get_AnimationCtrl()->GetTimeAcc() > m_dAnimationPeriod * 0.1f && m_pMeshCom->Get_AnimationCtrl()->GetTimeAcc() < m_dAnimationPeriod * 0.2f)
	{
		if(pBossColliderCom->Collision_OBB(pShieldColliderCom, Engine::CBoxCollider::CHANGE_YES))
			pBoss->SetIsCol(true, m_fDamage * 2.f, m_fGroggyDamage * 4.f, fTimeDelta);
	}
}

CPlayer* CPlayer::Create(LPDIRECT3DDEVICE9 pGraphicDev, CCamera_Stage* pCamera)
{
	CPlayer* pInstance = new CPlayer(pGraphicDev);

	if(FAILED(pInstance->Ready_GameObject(pCamera)))
	{
		MSG_BOX("CPlayer Created Failed");
		::Safe_Release(pInstance);
	}
	return pInstance;
}

void CPlayer::Free(void)
{
	Engine::Safe_Release(m_pRendererCom);
	Engine::Safe_Release(m_pTransformCom);
	Engine::Safe_Release(m_pMeshCom);
	Engine::Safe_Release(m_pColliderCom_Foot);
	Engine::Safe_Release(m_pColliderCom);
	Engine::Safe_Release(m_pPreHitColliderCom);
	Engine::Safe_Release(m_pMtrlCom);
	Engine::Safe_Release(m_pShaderCom);

	Engine::CGameObject::Free();	
}

void CPlayer::SetIsCol(_bool bIsCol, _float iDamage, _uint iType)
{
	if(0 == iType)
	{
		if(bIsCol && !m_bIsCol)
			m_bIsCol = true;
	}
	else
	{
		if(bIsCol && !m_bPreHit && GUARD != m_iState)
			m_bPreHit = true;
	}
}

_uint CPlayer::Get_State(void)
{
	return m_iState;
}

void CPlayer::CancleEnable(void)
{
	if(ATTACK == m_iState && 4 != m_iAttackNum && m_pMeshCom->Get_AnimationCtrl()->GetTimeAcc() >= m_dAnimationPeriod * 0.4)
		m_bCancleEnable = true;
	else if(ATTACK == m_iState && 4 == m_iAttackNum && m_pMeshCom->Get_AnimationCtrl()->GetTimeAcc() >= m_dAnimationPeriod * 0.5)
		m_bCancleEnable = true;
	else if(SMASH == m_iState && m_pMeshCom->Get_AnimationCtrl()->GetTimeAcc() >= m_dAnimationPeriod * 0.2)
		m_bCancleEnable = true;
	else if(SPRINTATT == m_iState && m_pMeshCom->Get_AnimationCtrl()->GetTimeAcc() >= m_dAnimationPeriod * 0.45)
		m_bCancleEnable = true;
	else
		m_bCancleEnable = false;
}

HRESULT CPlayer::SetUp_ConstantTable(LPD3DXEFFECT pEffect)
{
	m_pTransformCom->SetUp_OnShader(pEffect, "g_matWorld");

	_matrix			matView, matProj;
	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
	m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &matProj);

	pEffect->SetMatrix("g_matView", &matView);
	pEffect->SetMatrix("g_matProj", &matProj);

	D3DLIGHT9			LightInfo;
	ZeroMemory(&LightInfo, sizeof(D3DLIGHT9));

	m_pGraphicDev->GetLight(0, &LightInfo);

	pEffect->SetVector("g_vLightDir", &D3DXVECTOR4(LightInfo.Direction, 0.f));
	pEffect->SetVector("g_vLightDiffuse", (_vec4*)&LightInfo.Diffuse);
	pEffect->SetVector("g_vLightAmbient", (_vec4*)&LightInfo.Ambient);
	pEffect->SetVector("g_vLightSpecular", (_vec4*)&LightInfo.Specular);

	D3DMATERIAL9			MtrlInfo;
	ZeroMemory(&MtrlInfo, sizeof(D3DMATERIAL9));

	m_pMtrlCom->Get_Material(&MtrlInfo);

	pEffect->SetVector("g_vMtrlDiffuse", (_vec4*)&MtrlInfo.Diffuse);
	pEffect->SetVector("g_vMtrlAmbient", (_vec4*)&MtrlInfo.Ambient);
	pEffect->SetVector("g_vMtrlSpecular", (_vec4*)&MtrlInfo.Specular);
	pEffect->SetFloat("g_fPower", MtrlInfo.Power);

	D3DXMatrixInverse(&matView, NULL, &matView);

	pEffect->SetVector("g_vCameraPos", (_vec4*)&matView.m[3][0]);	

	return S_OK;
}
