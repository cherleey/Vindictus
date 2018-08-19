#include "stdafx.h"
#include "Boss.h"
#include "Player.h"
#include "AnimationCtrl.h"
#include "Map.h"

#include "Export_Engine.h"

float g_fDebugTime = 0.f;

CBoss::CBoss(LPDIRECT3DDEVICE9 pGraphicDev)
: Engine::CGameObject(pGraphicDev)
, m_fTimeDelta(0.f)
, m_pColliderCom(NULL)
, m_fSpeed(0.f)
, m_bBack(false)
, m_bRotating(false)
, m_bRunning(false)
, m_bAttacking(false)
, m_bStop(false)
, m_iSRangePattern(0)
, m_iLRangePattern(0)
, m_fHP(100.f)
, m_bGroggy(false)
, m_fDamageTime(0.f)
, m_fGroggyHP(100.f)
, m_bSwingTripleRotated(false)
, m_iPhase(1)
, m_bRotateAtt(false)
, m_bColWithMap(false)
{
	ZeroMemory(m_vOriginDir, sizeof(_vec3));
}

CBoss::~CBoss(void)
{

}

HRESULT CBoss::Ready_GameObject(void)
{
	if(FAILED(Ready_Component()))
		return E_FAIL;

	m_pTransformCom->Set_Infomation(Engine::CTransform::INFO_POSITION, &Engine::_vec3(0.0f, 1.3f, 6.0f));
	m_pTransformCom->Set_Infomation(Engine::CTransform::INFO_SCALE, &Engine::_vec3(0.012f, 0.012f, 0.012f));	
	m_pTransformCom->Rotation(Engine::CTransform::ANGLE_Y, D3DXToRadian(-90.f));

	m_iState = IDLE;
	m_pMeshCom->Set_AnimationSet(4);

	return S_OK;
}

_int CBoss::Update_GameObject(const _float& fTimeDelta)
{
	m_fTimeDelta = fTimeDelta;

	Pattern(m_fTimeDelta);

	Engine::CGameObject::Update_GameObject(fTimeDelta);
	
	m_pRendererCom->Add_RenderList(Engine::CRenderer::RENDER_NONEALPHA, this);

	CollisionWhileAttack();

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

void CBoss::Render_GameObject(void)
{
	LPD3DXEFFECT pEffect = m_pShaderCom->Get_EffectHandle();
	if(NULL == pEffect)
		return;

	//pEffect->AddRef();

	if(FAILED(SetUp_ConstantTable(pEffect)))
		return;

	pEffect->Begin(NULL, 0);

	m_pMeshCom->Play_AnimationSet(m_fTimeDelta);
	m_pMeshCom->Render_Mesh(pEffect);

	pEffect->End();

	m_pTransformCom->SetUp_OnGraphicDev();
	m_pColliderCom->Render_Collider(Engine::CBoxCollider::TYPE_OBB, m_pTransformCom->Get_WorldMatrix());
}

void CBoss::SetIsCol(_bool bIsCol, _float fDamage, _float fGroggyDamage, const _float& fTimeDelta)
{
	if(bIsCol)
	{
		if(0.f == m_fDamageTime)
		{
			m_fHP -= fDamage;
			m_fGroggyHP -= fGroggyDamage;
		}

		m_fDamageTime += fTimeDelta;

		if(0.16f <= m_fDamageTime)
			m_fDamageTime = 0.f;
	}
}

HRESULT CBoss::Ready_Component(void)
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

	m_pMeshCom = (Engine::CDynamicMesh*)Engine::Clone_Component(SCENE_STAGE, L"Component_Mesh_Ingkells");
	if(NULL == m_pMeshCom)
		return E_FAIL;
	if(FAILED(Add_Component(Engine::CGameObject::TYPE_STATIC, L"Com_Mesh", m_pMeshCom)))
		return E_FAIL;

	m_pColliderCom = (Engine::CBoxCollider*)Engine::Clone_Component(SCENE_STAGE, L"Component_BoxCollider");
	if(NULL == m_pColliderCom)
		return E_FAIL;
	if(FAILED(m_pColliderCom->Compute_Collider(m_pMeshCom,0)))
		return E_FAIL;
	if(FAILED(Add_Component(Engine::CGameObject::TYPE_STATIC, L"Com_Collider", m_pColliderCom)))
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

CBoss* CBoss::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CBoss* pInstance = new CBoss(pGraphicDev);

	if(FAILED(pInstance->Ready_GameObject()))
	{
		MSG_BOX("CBoss Created Failed");
		::Safe_Release(pInstance);
	}
	return pInstance;
}

void CBoss::Free(void)
{
	Engine::Safe_Release(m_pRendererCom);
	Engine::Safe_Release(m_pTransformCom);
	Engine::Safe_Release(m_pMeshCom);
	Engine::Safe_Release(m_pColliderCom);
	Engine::Safe_Release(m_pMtrlCom);
	Engine::Safe_Release(m_pShaderCom);


	Engine::CGameObject::Free();	
}

void CBoss::Pattern(const _float& fTimeDelta)
{
	m_pPlayerTransCom = (Engine::CTransform*)Engine::Get_Component(L"Layer_Player", Engine::CGameObject::TYPE_DYNAMIC, L"Com_Transform");

	_vec3 vPlayerPos = _vec3(m_pPlayerTransCom->Get_WorldMatrix()->_41, m_pPlayerTransCom->Get_WorldMatrix()->_42, m_pPlayerTransCom->Get_WorldMatrix()->_43);
	_vec3 vBossPos = _vec3(m_pTransformCom->Get_WorldMatrix()->_41, m_pTransformCom->Get_WorldMatrix()->_42, m_pTransformCom->Get_WorldMatrix()->_43);
	_vec3 vDir;
	D3DXVECTOR3 vTemp = vPlayerPos - vBossPos;
	_float fDistance = D3DXVec3Length(&vTemp);
	_vec3 vLook = _vec3(m_pTransformCom->Get_WorldMatrix()->_31, m_pTransformCom->Get_WorldMatrix()->_32, m_pTransformCom->Get_WorldMatrix()->_33);
	_vec3 vRight = _vec3(m_pTransformCom->Get_WorldMatrix()->_11, m_pTransformCom->Get_WorldMatrix()->_12, m_pTransformCom->Get_WorldMatrix()->_13);
	float fRad = 0.f;
	float fCosRight = 0.f;
	float fCosLook = 0.f;

	vDir = vTemp;

	D3DXVec3Normalize(&vDir, &vDir);
	D3DXVec3Normalize(&vLook, &vLook);
	D3DXVec3Normalize(&vRight, &vRight);

	fCosRight = D3DXVec3Dot(&vRight, &vDir);
	fCosLook = D3DXVec3Dot(&vLook, &vDir);

	fRad = acosf(D3DXVec3Dot(&vLook, &vDir));

	////디버그
	//g_fDebugTime += m_fTimeDelta;
	//if(g_fDebugTime > 0.5f)
	//{
	//	system("cls");

	//	if(0 == m_iState)
	//		cout << "상태 : IDLE" << endl;
	//	else if(1 == m_iState)
	//		cout << "상태 : RUN" << endl;
	//	else if(2 == m_iState)
	//		cout << "상태 : NORMAL ATTACK" << endl;
	//	else if(3 == m_iState)
	//		cout << "상태 : PIERCE" << endl;
	//	else if(4 == m_iState)
	//		cout << "상태 : PIERCE SPIN" << endl;
	//	else if(5 == m_iState)
	//		cout << "상태 : SLASH SPIN" << endl;
	//	else if(6 == m_iState)
	//		cout << "상태 : SWING TRIPLE" << endl;
	//	else if(7 == m_iState)
	//		cout << "상태 : SWORDWAVE" << endl;
	//	else if(8 == m_iState)
	//		cout << "상태 : DOUBLESMASH" << endl;
	//	else if(9 == m_iState)
	//		cout << "상태 : WHIRLWIND" << endl;
	//	else if(10 == m_iState)
	//		cout << "상태 : AFTERATT" << endl;
	//	else if(11 == m_iState)
	//		cout << "상태 : ROTATE" << endl;
	//	else if(12 == m_iState)
	//		cout << "상태 : GROGGY" << endl;
	//	else if(13 == m_iState)
	//		cout << "상태 : PHASE" << endl;
	//	else if(14 == m_iState)
	//		cout << "상태 : ROTATE_ATT" << endl;
	//	
	//	cout << "페이즈 : " << m_iPhase << endl;

	//	cout << "HP : " << m_fHP << endl;

	//	cout << "그로기 : " << m_fGroggyHP << endl;

	//	cout << "근거리 콤보 : " << m_iSRangePattern << endl;

	//	cout << "원거리 콤보 : " << m_iLRangePattern << endl;

	//	cout << "플레이어와의 거리 : " << fDistance << endl;

	//	cout << "플레이어와의 각도 : " << D3DXToDegree(fRad) << endl;

	//	cout << "회전 공격 : " << m_bRotateAtt << endl;

	//	g_fDebugTime = 0.f;
	//}

	//페이즈 전환
	if(1 == m_iPhase && m_fHP <= 50.f)
	{
		m_iPhase = 2;
		m_iState = PHASE;
		m_iSRangePattern = 0;
		m_iLRangePattern = 0;
		m_pMeshCom->Set_AnimationSet(5);
		m_dAnimationPeriod = m_pMeshCom->Get_AnimationCtrl()->Get_Period();
	}
	else if(PHASE == m_iState && m_pMeshCom->Get_AnimationCtrl()->GetTimeAcc() <= m_dAnimationPeriod * 0.7f)
		return;
	else if(PHASE == m_iState)
	{
		m_bRotating = false;
		m_bRunning = false;
		m_bAttacking = false;
		m_iState = IDLE;
		m_pMeshCom->Set_AnimationSet(4);
	}

	//그로기
	if(0 >= m_fGroggyHP && !m_bGroggy)
	{
		m_bGroggy = true;
		m_iState = GROGGY;
		m_fGroggyHP = 100.f;
		m_pMeshCom->Set_AnimationSet(25);
		m_dAnimationPeriod = m_pMeshCom->Get_AnimationCtrl()->Get_Period();
		return;
	}
	else if(GROGGY == m_iState && m_pMeshCom->Get_AnimationCtrl()->GetTimeAcc() <= m_dAnimationPeriod * 0.7f)
		return;
	else if(GROGGY == m_iState)
	{
		m_bRotating = false;
		m_bRunning = false;
		m_bAttacking = false;
		m_iState = IDLE;
		m_pMeshCom->Set_AnimationSet(4);
	}

	//오른쪽회전
	if(fCosLook > 0 && fCosRight < 0 && D3DXToDegree(fRad) < 80.f && !m_bAttacking)
	{
		if(!m_bRotating && fDistance <= 1.8f && !m_bRotateAtt)
		{
			m_pMeshCom->Set_AnimationSet(23);
			m_bRotating = true;
			m_bRotateAtt = true;
			m_iState = RROTATE_ATT;
			m_eAttState = NORMAL;
		}
		else if(!m_bRotating)
		{
			m_pMeshCom->Set_AnimationSet(33);
			m_bRotating = true;
			m_bRotateAtt = false;
			m_iState = ROTATE;
		}

		if(ROTATE == m_iState)
			m_pTransformCom->Rotation(Engine::CTransform::ANGLE_Y, 2 * (D3DXToRadian(90.f) - fRad) * fTimeDelta);

		m_dAnimationPeriod = m_pMeshCom->Get_AnimationCtrl()->Get_Period();
	}

	if(ROTATE == m_iState && m_pMeshCom->Get_AnimationCtrl()->GetTimeAcc() >= m_dAnimationPeriod * 0.7f)
	{
		m_iState = IDLE;
		m_bRotating = false;
	}

	if(RROTATE_ATT == m_iState && m_pMeshCom->Get_AnimationCtrl()->GetTimeAcc() >= m_dAnimationPeriod * 0.6f)
	{
		m_iState = IDLE;
		m_bRotating = false;
	}

	if(fCosLook > 0 && fCosRight > 0 && !m_bAttacking)
	{
		if(!m_bRotating)
		{
			m_pMeshCom->Set_AnimationSet(33);
			m_bRotating = true;
			m_iState = ROTATE;
		}

		m_pTransformCom->Rotation(Engine::CTransform::ANGLE_Y, 2 * (D3DXToRadian(90.f) + fRad) * fTimeDelta);

		m_dAnimationPeriod = m_pMeshCom->Get_AnimationCtrl()->Get_Period();
	}
	
	if(ROTATE == m_iState && m_pMeshCom->Get_AnimationCtrl()->GetTimeAcc() >= m_dAnimationPeriod * 0.7f)
	{
		m_iState = IDLE;
		m_bRotating = false;
	}

	//왼쪽회전
	if(fCosLook < 0 && fCosRight < 0 && D3DXToDegree(fRad) > 100.f && !m_bAttacking)
	{
		if(!m_bRotating && fDistance <= 1.8f && !m_bRotateAtt)
		{
			m_pMeshCom->Set_AnimationSet(22);
			m_bRotating = true;
			m_bRotateAtt = true;
			m_iState = LROTATE_ATT;
			m_eAttState = NORMAL;
		}
		else if(!m_bRotating)
		{
			m_pMeshCom->Set_AnimationSet(32);
			m_bRotating = true;
			m_bRotateAtt = false;
			m_iState = ROTATE;
		}

		if(ROTATE == m_iState)
			m_pTransformCom->Rotation(Engine::CTransform::ANGLE_Y, -2 * (fRad - D3DXToRadian(90.f)) * fTimeDelta);

		m_dAnimationPeriod = m_pMeshCom->Get_AnimationCtrl()->Get_Period();
	}
	
	if(ROTATE == m_iState && m_pMeshCom->Get_AnimationCtrl()->GetTimeAcc() >= m_dAnimationPeriod * 0.7f)
	{
		m_iState = IDLE;
		m_bRotating = false;
	}

	if(LROTATE_ATT == m_iState && m_pMeshCom->Get_AnimationCtrl()->GetTimeAcc() >= m_dAnimationPeriod * 0.6f)
	{
		m_iState = IDLE;
		m_bRotating = false;
	}

	if(fCosLook < 0 && fCosRight > 0 && !m_bAttacking)
	{
		if(!m_bRotating)
		{
			m_pMeshCom->Set_AnimationSet(32);
			m_bRotating = true;
			m_iState = ROTATE;
		}

		m_pTransformCom->Rotation(Engine::CTransform::ANGLE_Y, -2 * (fRad - D3DXToRadian(90.f)) * fTimeDelta);

		m_dAnimationPeriod = m_pMeshCom->Get_AnimationCtrl()->Get_Period();
	}
	
	if(ROTATE == m_iState && m_pMeshCom->Get_AnimationCtrl()->GetTimeAcc() >= m_dAnimationPeriod * 0.7f)
	{
		m_iState = IDLE;
		m_bRotating = false;
	}

	//전진
	if(fDistance > 2.f && !m_bRotating && !m_bAttacking)
	{
		m_pMeshCom->Set_AnimationSet(29);
		m_iState = RUN;
		m_bRunning = true;
		m_fSpeed = 1.1f;
		m_pTransformCom->Go_Straight(m_fTimeDelta * m_fSpeed, vDir);
	}
	
	if(RUN == m_iState && !m_bRotating)
	{
		m_iState = IDLE;
		m_bRunning = false;
	}

	//Normal Attack
	if(fDistance <= 2.5f && fCosRight < 0 && fDistance != 0.f && !m_bRotating && D3DXToDegree(fRad) <= 100.f && D3DXToDegree(fRad) >= 80.f && m_iSRangePattern < 2)
	{
		if(m_pMeshCom->Get_AnimationCtrl()->GetTimeAcc() >= m_dAnimationPeriod * 0.7f)
			m_bAttacking = false;

		if(!m_bAttacking)
		{
			m_iState = NORMALATTACK;
			m_eAttState = SMASH;
			m_bAttacking = true;
			m_pMeshCom->Set_AnimationSet(10);
			m_vOriginDir = vDir;
			D3DXVec3Normalize(&m_vOriginDir, &m_vOriginDir);
			++m_iSRangePattern;
		}

		m_dAnimationPeriod = m_pMeshCom->Get_AnimationCtrl()->Get_Period();
	}
	
	if(NORMALATTACK == m_iState && m_pMeshCom->Get_AnimationCtrl()->GetTimeAcc() >= m_dAnimationPeriod * 0.7f && !m_bRotating)
	{
		m_iState = IDLE;
		m_bAttacking = false;
	}

	//SwordWave
	if(fDistance <= 2.5f && fCosRight < 0 && fDistance != 0.f && !m_bRotating && !m_bRotating && D3DXToDegree(fRad) <= 100.f && D3DXToDegree(fRad) >= 80.f && 2 == m_iSRangePattern)
	{
		if(m_pMeshCom->Get_AnimationCtrl()->GetTimeAcc() >= m_dAnimationPeriod * 0.7f)
			m_bAttacking = false;

		if(!m_bAttacking)
		{
			m_iState = SWORDWAVE;
			m_eAttState = SMASH;
			m_bAttacking = true;
			m_pMeshCom->Set_AnimationSet(19);
			m_vOriginDir = vDir;
			D3DXVec3Normalize(&m_vOriginDir, &m_vOriginDir);
			++m_iSRangePattern;
		}

		m_dAnimationPeriod = m_pMeshCom->Get_AnimationCtrl()->Get_Period();
	}
	
	if(SWORDWAVE == m_iState && m_pMeshCom->Get_AnimationCtrl()->GetTimeAcc() >= m_dAnimationPeriod * 0.7f && !m_bRotating)
	{
		m_iState = IDLE;
		m_bAttacking = false;
	}

	//Swing_triple
	if(fCosRight < 0 && fDistance != 0.f && !m_bRotating && !m_bRotating && 3 == m_iSRangePattern && D3DXToDegree(fRad) <= 100.f && D3DXToDegree(fRad) >= 80.f)
	{
		if(m_pMeshCom->Get_AnimationCtrl()->GetTimeAcc() >= m_dAnimationPeriod * 0.7f)
			m_bAttacking = false;

		if(!m_bAttacking)
		{
			m_iState = SWING_TRIPLE;
			m_eAttState = SMASH;
			m_bAttacking = true;
			m_pMeshCom->Set_AnimationSet(17, 2.f);
			m_vOriginDir = vDir;
			D3DXVec3Normalize(&m_vOriginDir, &m_vOriginDir);
			if(1 == m_iPhase)
				m_iSRangePattern = 0;
			else
				++m_iSRangePattern;
		}

		m_dAnimationPeriod = m_pMeshCom->Get_AnimationCtrl()->Get_Period();
	}
	
	if(SWING_TRIPLE == m_iState && m_pMeshCom->Get_AnimationCtrl()->GetTimeAcc() >= m_dAnimationPeriod * 0.5f && !m_bRotating)
	{
		m_iState = IDLE;
		m_bAttacking = false;
		m_bSwingTripleRotated = false;
	}

	//Double Smash - Phase 2
	if(fCosRight < 0 && fDistance != 0.f && !m_bRotating && !m_bRotating && 4 == m_iSRangePattern && 2 == m_iPhase && D3DXToDegree(fRad) <= 100.f && D3DXToDegree(fRad) >= 80.f)
	{
		if(m_pMeshCom->Get_AnimationCtrl()->GetTimeAcc() >= m_dAnimationPeriod * 0.7f)
			m_bAttacking = false;

		if(!m_bAttacking)
		{
			m_iState = DOUBLE_SMASH;
			m_eAttState = SMASH;
			m_bAttacking = true;
			m_pMeshCom->Set_AnimationSet(6);
			m_vOriginDir = vDir;
			D3DXVec3Normalize(&m_vOriginDir, &m_vOriginDir);
			m_iSRangePattern = 0;
		}

		m_dAnimationPeriod = m_pMeshCom->Get_AnimationCtrl()->Get_Period();
	}
	
	if(DOUBLE_SMASH == m_iState && m_pMeshCom->Get_AnimationCtrl()->GetTimeAcc() >= m_dAnimationPeriod * 0.7f && !m_bRotating)
	{
		m_iState = IDLE;
		m_bAttacking = false;
		m_bSwingTripleRotated = false;
	}

	//Pierce
	if(1 == m_iPhase && fDistance > 2.5f && fDistance <= 5 && fCosRight < 0 && fDistance != 0.f && !m_bRotating && !m_bRotating && /*m_iLRangePattern < 2 && */D3DXToDegree(fRad) <= 100.f && D3DXToDegree(fRad) >= 80.f)
	{
		if(m_pMeshCom->Get_AnimationCtrl()->GetTimeAcc() >= m_dAnimationPeriod * 0.7f)
			m_bAttacking = false;

		if(!m_bAttacking)
		{
			m_iState = PIERCE;
			m_eAttState = SMASH;
			m_bAttacking = true;
			m_pMeshCom->Set_AnimationSet(13);
			m_vOriginDir = vDir;
			D3DXVec3Normalize(&m_vOriginDir, &m_vOriginDir);
			//++m_iLRangePattern;
		}

		m_dAnimationPeriod = m_pMeshCom->Get_AnimationCtrl()->Get_Period();
	}
	
	if(PIERCE == m_iState && m_pMeshCom->Get_AnimationCtrl()->GetTimeAcc() >= m_dAnimationPeriod * 0.7f && !m_bRotating)
	{
		m_iState = IDLE;
		m_bAttacking = false;
	}

	//Pierce and Spin - Phase 2(Instead of Pierce)
	if(2 == m_iPhase && fDistance > 2.5f && fDistance <= 5 && fCosRight < 0 && fDistance != 0.f && !m_bRotating && !m_bRotating && m_iLRangePattern < 2 && D3DXToDegree(fRad) <= 100.f && D3DXToDegree(fRad) >= 80.f)
	{
		if(m_pMeshCom->Get_AnimationCtrl()->GetTimeAcc() >= m_dAnimationPeriod * 0.7f)
			m_bAttacking = false;

		if(!m_bAttacking)
		{
			m_iState = PIERCE_SPIN;
			m_eAttState = SMASH;
			m_bAttacking = true;
			m_pMeshCom->Set_AnimationSet(14);
			m_vOriginDir = vDir;
			D3DXVec3Normalize(&m_vOriginDir, &m_vOriginDir);
			//++m_iLRangePattern;
		}

		m_dAnimationPeriod = m_pMeshCom->Get_AnimationCtrl()->Get_Period();
	}
	
	if(PIERCE_SPIN == m_iState && m_pMeshCom->Get_AnimationCtrl()->GetTimeAcc() >= m_dAnimationPeriod * 0.6f && !m_bRotating)
	{
		m_iState = IDLE;
		m_bAttacking = false;
	}

	CorrectPosisionWhileAttack(m_fTimeDelta, m_vOriginDir, fDistance, fRad, fCosLook);
}

void CBoss::CollisionWhileAttack(void)
{
	Engine::CBoxCollider* pSwordColliderCom = (Engine::CBoxCollider*)Engine::Get_Component(L"Layer_Boss", Engine::CGameObject::TYPE_STATIC, L"Com_Collider", 1);
	Engine::CBoxCollider* pPlayerColliderCom = (Engine::CBoxCollider*)Engine::Get_Component(L"Layer_Player", Engine::CGameObject::TYPE_STATIC, L"Com_Collider");
	Engine::CBoxCollider* pPlayerPreHitColliderCom = (Engine::CBoxCollider*)Engine::Get_Component(L"Layer_Player", Engine::CGameObject::TYPE_STATIC, L"Com_PreHitCollider");
	Engine::CTransform* pPlayerTransform = (Engine::CTransform*)Engine::Get_Component(L"Layer_Player", Engine::CGameObject::TYPE_DYNAMIC, L"Com_Transform");
	CPlayer* pPlayer = (CPlayer*)Engine::Find_Layer(L"Layer_Player")->Get_GameObject()->front();

	/*if(m_pColliderCom->Collision_OBB(pPlayerColliderCom, Engine::CBoxCollider::CHANGE_NO) && IDLE != m_iState && ROTATE != m_iState)
	{
		_vec3 vDir = _vec3(pPlayerTransform->Get_WorldMatrix()->_41, pPlayerTransform->Get_WorldMatrix()->_42, pPlayerTransform->Get_WorldMatrix()->_43) - _vec3(m_pTransformCom->Get_WorldMatrix()->_41, m_pTransformCom->Get_WorldMatrix()->_42, m_pTransformCom->Get_WorldMatrix()->_43);
		m_pTransformCom->Go_Straight(-m_fSpeed * m_fTimeDelta, vDir);
	}*/

	if(NORMALATTACK == m_iState && m_pMeshCom->Get_AnimationCtrl()->GetTimeAcc() > m_dAnimationPeriod * 0.2f && m_pMeshCom->Get_AnimationCtrl()->GetTimeAcc() < m_dAnimationPeriod * 0.3f)
	{
		if(pPlayerColliderCom->Collision_OBB(pSwordColliderCom, Engine::CBoxCollider::CHANGE_YES))
			pPlayer->SetIsCol(true, 0.f, 0);

		if(pPlayerPreHitColliderCom->Collision_OBB(pSwordColliderCom, Engine::CBoxCollider::CHANGE_YES))
			pPlayer->SetIsCol(true, 0.f, 1);
	}
	
	if(PIERCE == m_iState && m_pMeshCom->Get_AnimationCtrl()->GetTimeAcc() > m_dAnimationPeriod * 0.45f && m_pMeshCom->Get_AnimationCtrl()->GetTimeAcc() < m_dAnimationPeriod * 0.6f)
	{
		if(pPlayerColliderCom->Collision_OBB(pSwordColliderCom, Engine::CBoxCollider::CHANGE_YES))
			pPlayer->SetIsCol(true, 0.f, 0);

		if(pPlayerPreHitColliderCom->Collision_OBB(pSwordColliderCom, Engine::CBoxCollider::CHANGE_YES))
			pPlayer->SetIsCol(true, 0.f, 1);
	}
	
	if(PIERCE_SPIN == m_iState)
	{
		if(m_pMeshCom->Get_AnimationCtrl()->GetTimeAcc() > m_dAnimationPeriod * 0.25f && m_pMeshCom->Get_AnimationCtrl()->GetTimeAcc() < m_dAnimationPeriod * 0.5f)
		{
			if(pPlayerColliderCom->Collision_OBB(pSwordColliderCom, Engine::CBoxCollider::CHANGE_YES))
				pPlayer->SetIsCol(true, 0.f, 0);

			if(pPlayerPreHitColliderCom->Collision_OBB(pSwordColliderCom, Engine::CBoxCollider::CHANGE_YES))
				pPlayer->SetIsCol(true, 0.f, 1);
		}
	}

	if(DOUBLE_SMASH == m_iState)
	{
		if(m_pMeshCom->Get_AnimationCtrl()->GetTimeAcc() > m_dAnimationPeriod * 0.2f && m_pMeshCom->Get_AnimationCtrl()->GetTimeAcc() < m_dAnimationPeriod * 0.25f)
		{
			if(pPlayerColliderCom->Collision_OBB(pSwordColliderCom, Engine::CBoxCollider::CHANGE_YES))
				pPlayer->SetIsCol(true, 0.f, 0);

			if(pPlayerPreHitColliderCom->Collision_OBB(pSwordColliderCom, Engine::CBoxCollider::CHANGE_YES))
				pPlayer->SetIsCol(true, 0.f, 1);
		}

		if(m_pMeshCom->Get_AnimationCtrl()->GetTimeAcc() > m_dAnimationPeriod * 0.37f && m_pMeshCom->Get_AnimationCtrl()->GetTimeAcc() < m_dAnimationPeriod * 0.42f)
		{
			if(pPlayerColliderCom->Collision_OBB(pSwordColliderCom, Engine::CBoxCollider::CHANGE_YES))
				pPlayer->SetIsCol(true, 0.f, 0);

			if(pPlayerPreHitColliderCom->Collision_OBB(pSwordColliderCom, Engine::CBoxCollider::CHANGE_YES))
				pPlayer->SetIsCol(true, 0.f, 1);
		}
	}

	//if(SLASH_SPIN == m_iState/* && m_pMeshCom->Get_AnimationCtrl()->GetTimeAcc() > m_dAnimationPeriod * 0.1f && m_pMeshCom->Get_AnimationCtrl()->GetTimeAcc() < m_dAnimationPeriod * 0.2f*/)
	//{
	//	if(pPlayerColliderCom->Collision_OBB(pSwordColliderCom))
	//		pPlayer->SetIsCol(true, 0.f, m_dAnimationPeriod, m_pMeshCom->Get_AnimationCtrl()->GetTimeAcc());
	//}
	
	if(SWING_TRIPLE == m_iState)
	{
		if(m_pMeshCom->Get_AnimationCtrl()->GetTimeAcc() >= m_dAnimationPeriod * 0.1f && m_pMeshCom->Get_AnimationCtrl()->GetTimeAcc() <= m_dAnimationPeriod * 0.14f)
		{
			if(pPlayerColliderCom->Collision_OBB(pSwordColliderCom, Engine::CBoxCollider::CHANGE_YES))
				pPlayer->SetIsCol(true, 0.f, 0);

			if(pPlayerPreHitColliderCom->Collision_OBB(pSwordColliderCom, Engine::CBoxCollider::CHANGE_YES))
				pPlayer->SetIsCol(true, 0.f, 1);
		}

		if(m_pMeshCom->Get_AnimationCtrl()->GetTimeAcc() >= m_dAnimationPeriod * 0.18f && m_pMeshCom->Get_AnimationCtrl()->GetTimeAcc() <= m_dAnimationPeriod * 0.22f)
		{
			if(pPlayerColliderCom->Collision_OBB(pSwordColliderCom, Engine::CBoxCollider::CHANGE_YES))
				pPlayer->SetIsCol(true, 0.f, 0);

			if(pPlayerPreHitColliderCom->Collision_OBB(pSwordColliderCom, Engine::CBoxCollider::CHANGE_YES))
				pPlayer->SetIsCol(true, 0.f, 1);
		}

		if(m_pMeshCom->Get_AnimationCtrl()->GetTimeAcc() >= m_dAnimationPeriod * 0.3f && m_pMeshCom->Get_AnimationCtrl()->GetTimeAcc() <= m_dAnimationPeriod * 0.33f)
		{
			if(pPlayerColliderCom->Collision_OBB(pSwordColliderCom, Engine::CBoxCollider::CHANGE_YES))
				pPlayer->SetIsCol(true, 0.f, 0);

			if(pPlayerPreHitColliderCom->Collision_OBB(pSwordColliderCom, Engine::CBoxCollider::CHANGE_YES))
				pPlayer->SetIsCol(true, 0.f, 1);
		}
	}

	if(SWORDWAVE == m_iState && m_pMeshCom->Get_AnimationCtrl()->GetTimeAcc() > m_dAnimationPeriod * 0.35f && m_pMeshCom->Get_AnimationCtrl()->GetTimeAcc() < m_dAnimationPeriod * 0.4f)
	{
		if(pPlayerColliderCom->Collision_OBB(pSwordColliderCom, Engine::CBoxCollider::CHANGE_YES))
			pPlayer->SetIsCol(true, 0.f, 0);

		if(pPlayerPreHitColliderCom->Collision_OBB(pSwordColliderCom, Engine::CBoxCollider::CHANGE_YES))
			pPlayer->SetIsCol(true, 0.f, 1);
	}

	//if(WHIRLWIND == m_iState/* && m_pMeshCom->Get_AnimationCtrl()->GetTimeAcc() > m_dAnimationPeriod * 0.1f && m_pMeshCom->Get_AnimationCtrl()->GetTimeAcc() < m_dAnimationPeriod * 0.2f*/)
	//{
	//	if(pPlayerColliderCom->Collision_OBB(pSwordColliderCom))
	//		pPlayer->SetIsCol(true, 0.f, m_dAnimationPeriod, m_pMeshCom->Get_AnimationCtrl()->GetTimeAcc());
	//}

	if(RROTATE_ATT == m_iState)
	{
		if(m_pMeshCom->Get_AnimationCtrl()->GetTimeAcc() >= m_dAnimationPeriod * 0.3f && m_pMeshCom->Get_AnimationCtrl()->GetTimeAcc() <= m_dAnimationPeriod * 0.35f)
		{
			if(pPlayerColliderCom->Collision_OBB(pSwordColliderCom, Engine::CBoxCollider::CHANGE_YES))
				pPlayer->SetIsCol(true, 0.f, 0);

			if(pPlayerPreHitColliderCom->Collision_OBB(pSwordColliderCom, Engine::CBoxCollider::CHANGE_YES))
				pPlayer->SetIsCol(true, 0.f, 1);
		}
	}

	if(LROTATE_ATT == m_iState)
	{
		if(m_pMeshCom->Get_AnimationCtrl()->GetTimeAcc() >= m_dAnimationPeriod * 0.25f && m_pMeshCom->Get_AnimationCtrl()->GetTimeAcc() <= m_dAnimationPeriod * 0.3f)
		{
			if(pPlayerColliderCom->Collision_OBB(pSwordColliderCom, Engine::CBoxCollider::CHANGE_YES))
				pPlayer->SetIsCol(true, 0.f, 0);

			if(pPlayerPreHitColliderCom->Collision_OBB(pSwordColliderCom, Engine::CBoxCollider::CHANGE_YES))
				pPlayer->SetIsCol(true, 0.f, 1);
		}
	}
}

void CBoss::CorrectPosisionWhileAttack(const _float& fTimeDelta, const _vec3& vDir, _float fDistance, const _float& fRad, const _float& fCos)
{
	if(NORMALATTACK == m_iState)
	{
		if(m_pMeshCom->Get_AnimationCtrl()->GetTimeAcc() >= m_dAnimationPeriod * 0.1f && m_pMeshCom->Get_AnimationCtrl()->GetTimeAcc() <= m_dAnimationPeriod * 0.2f)
		{
			if(fDistance <= 1.5f)
				m_fSpeed = 0.f;
			else
				m_fSpeed = 1.1f;

			m_pTransformCom->Get_Infomation()[Engine::CTransform::INFO_POSITION] += vDir * fTimeDelta * m_fSpeed;
		}

		if(m_pMeshCom->Get_AnimationCtrl()->GetTimeAcc() >= m_dAnimationPeriod * 0.25f && m_pMeshCom->Get_AnimationCtrl()->GetTimeAcc() <= m_dAnimationPeriod * 0.3f)
		{
			if(fDistance <= 1.5f)
				m_fSpeed = 0.f;
			else
				m_fSpeed = 5.f;

			m_pTransformCom->Get_Infomation()[Engine::CTransform::INFO_POSITION] += vDir * fTimeDelta * m_fSpeed;
		}

		if(m_pMeshCom->Get_AnimationCtrl()->GetTimeAcc() >= m_dAnimationPeriod * 0.3f && m_pMeshCom->Get_AnimationCtrl()->GetTimeAcc() <= m_dAnimationPeriod * 0.37f)
		{
			if(fDistance <= 1.5f)
				m_fSpeed = 0.f;
			else
				m_fSpeed = 0.5f;

			m_pTransformCom->Get_Infomation()[Engine::CTransform::INFO_POSITION] += vDir * fTimeDelta * m_fSpeed;
		}

		if(m_pMeshCom->Get_AnimationCtrl()->GetTimeAcc() >= m_dAnimationPeriod * 0.5f && m_pMeshCom->Get_AnimationCtrl()->GetTimeAcc() <= m_dAnimationPeriod * 0.6f)
		{
			if(fDistance <= 1.5f)
				m_fSpeed = 0.f;
			else
				m_fSpeed = 0.5f;

			m_pTransformCom->Get_Infomation()[Engine::CTransform::INFO_POSITION] += vDir * fTimeDelta * m_fSpeed;
		}
	}

	if(PIERCE == m_iState)
	{
		_vec3 vPlayerPos = _vec3(m_pPlayerTransCom->Get_WorldMatrix()->_41, m_pPlayerTransCom->Get_WorldMatrix()->_42, m_pPlayerTransCom->Get_WorldMatrix()->_43);
		_vec3 vBossPos = _vec3(m_pTransformCom->Get_WorldMatrix()->_41, m_pTransformCom->Get_WorldMatrix()->_42, m_pTransformCom->Get_WorldMatrix()->_43);
		_vec3 vDirToPlayer = vPlayerPos - vBossPos;
		_float fDis = D3DXVec3Length(&vDirToPlayer);

		if(m_pMeshCom->Get_AnimationCtrl()->GetTimeAcc() >= m_dAnimationPeriod * 0.3f && m_pMeshCom->Get_AnimationCtrl()->GetTimeAcc() <= m_dAnimationPeriod * 0.4f)
		{
			if(fDistance <= 1.5f)
				m_fSpeed = 0.f;
			else
				m_fSpeed = 2.5f;

			m_pTransformCom->Get_Infomation()[Engine::CTransform::INFO_POSITION] += vDir * fTimeDelta * m_fSpeed;
		}

		if(m_pMeshCom->Get_AnimationCtrl()->GetTimeAcc() >= m_dAnimationPeriod * 0.45f && m_pMeshCom->Get_AnimationCtrl()->GetTimeAcc() <= m_dAnimationPeriod * 0.47f)
		{
			if(fDistance <= 1.5f)
				m_fSpeed = 0.f;
			else
				m_fSpeed = 5.f;

			m_pTransformCom->Get_Infomation()[Engine::CTransform::INFO_POSITION] += vDir * fTimeDelta * m_fSpeed;
		}

		if(m_pMeshCom->Get_AnimationCtrl()->GetTimeAcc() >= m_dAnimationPeriod * 0.47f && m_pMeshCom->Get_AnimationCtrl()->GetTimeAcc() <= m_dAnimationPeriod * 0.49f)
		{
			if(fDistance <= 1.5f)
				m_fSpeed = 0.f;
			else
				m_fSpeed = 5.f;

			m_pTransformCom->Get_Infomation()[Engine::CTransform::INFO_POSITION] += vDir * fTimeDelta * m_fSpeed;
		}

		if(m_pMeshCom->Get_AnimationCtrl()->GetTimeAcc() >= m_dAnimationPeriod * 0.49f && m_pMeshCom->Get_AnimationCtrl()->GetTimeAcc() <= m_dAnimationPeriod * 0.51f)
		{
			if(fDistance <= 1.5f)
				m_fSpeed = 0.f;
			else
				m_fSpeed = 4.f;

			m_pTransformCom->Get_Infomation()[Engine::CTransform::INFO_POSITION] += vDir * fTimeDelta * m_fSpeed;
		}

		if(m_pMeshCom->Get_AnimationCtrl()->GetTimeAcc() >= m_dAnimationPeriod * 0.51f && m_pMeshCom->Get_AnimationCtrl()->GetTimeAcc() <= m_dAnimationPeriod * 0.53f)
		{
			if(fDistance <= 1.5f)
				m_fSpeed = 0.f;
			else
				m_fSpeed = 3.f;

			m_pTransformCom->Get_Infomation()[Engine::CTransform::INFO_POSITION] += vDir * fTimeDelta * m_fSpeed;
		}

		if(m_pMeshCom->Get_AnimationCtrl()->GetTimeAcc() >= m_dAnimationPeriod * 0.53f && m_pMeshCom->Get_AnimationCtrl()->GetTimeAcc() <= m_dAnimationPeriod * 0.55f)
		{
			if(fDistance <= 1.5f)
				m_fSpeed = 0.f;
			else
				m_fSpeed = 2.f;

			m_pTransformCom->Get_Infomation()[Engine::CTransform::INFO_POSITION] += vDir * fTimeDelta * m_fSpeed;
		}

		if(m_pMeshCom->Get_AnimationCtrl()->GetTimeAcc() >= m_dAnimationPeriod * 0.55f && m_pMeshCom->Get_AnimationCtrl()->GetTimeAcc() <= m_dAnimationPeriod * 0.57f)
		{
			if(fDistance <= 1.5f)
				m_fSpeed = 0.f;
			else
				m_fSpeed = 1.f;

			m_pTransformCom->Get_Infomation()[Engine::CTransform::INFO_POSITION] += vDir * fTimeDelta * m_fSpeed;
		}
	}

	if(PIERCE_SPIN == m_iState)
	{
		if(m_pMeshCom->Get_AnimationCtrl()->GetTimeAcc() >= m_dAnimationPeriod * 0.15f && m_pMeshCom->Get_AnimationCtrl()->GetTimeAcc() <= m_dAnimationPeriod * 0.25f)
		{
			if(fDistance <= 1.5f)
				m_fSpeed = 0.f;
			else
				m_fSpeed = 3.f;

			m_pTransformCom->Get_Infomation()[Engine::CTransform::INFO_POSITION] += vDir * fTimeDelta * m_fSpeed;
		}

		if(m_pMeshCom->Get_AnimationCtrl()->GetTimeAcc() >= m_dAnimationPeriod * 0.25f && m_pMeshCom->Get_AnimationCtrl()->GetTimeAcc() <= m_dAnimationPeriod * 0.27f)
		{
			if(fDistance <= 1.5f)
				m_fSpeed = 0.f;
			else
				m_fSpeed = 6.f;

			m_pTransformCom->Get_Infomation()[Engine::CTransform::INFO_POSITION] += vDir * fTimeDelta * m_fSpeed;
		}

		if(m_pMeshCom->Get_AnimationCtrl()->GetTimeAcc() >= m_dAnimationPeriod * 0.27f && m_pMeshCom->Get_AnimationCtrl()->GetTimeAcc() <= m_dAnimationPeriod * 0.29f)
		{
			if(fDistance <= 1.5f)
				m_fSpeed = 0.f;
			else
				m_fSpeed = 4.f;

			m_pTransformCom->Get_Infomation()[Engine::CTransform::INFO_POSITION] += vDir * fTimeDelta * m_fSpeed;
		}

		if(m_pMeshCom->Get_AnimationCtrl()->GetTimeAcc() >= m_dAnimationPeriod * 0.29f && m_pMeshCom->Get_AnimationCtrl()->GetTimeAcc() <= m_dAnimationPeriod * 0.31f)
		{
			if(fDistance <= 1.5f)
				m_fSpeed = 0.f;
			else
				m_fSpeed = 2.f;

			m_pTransformCom->Get_Infomation()[Engine::CTransform::INFO_POSITION] += vDir * fTimeDelta * m_fSpeed;
		}

		if(m_pMeshCom->Get_AnimationCtrl()->GetTimeAcc() >= m_dAnimationPeriod * 0.31f && m_pMeshCom->Get_AnimationCtrl()->GetTimeAcc() <= m_dAnimationPeriod * 0.33f)
		{
			if(fDistance <= 1.5f)
				m_fSpeed = 0.f;
			else
				m_fSpeed = 1.f;

			m_pTransformCom->Get_Infomation()[Engine::CTransform::INFO_POSITION] += vDir * fTimeDelta * m_fSpeed;
		}

		if(m_pMeshCom->Get_AnimationCtrl()->GetTimeAcc() >= m_dAnimationPeriod * 0.35f && m_pMeshCom->Get_AnimationCtrl()->GetTimeAcc() <= m_dAnimationPeriod * 0.38f)
		{
			if(fDistance <= 1.5f)
				m_fSpeed = 0.f;
			else
				m_fSpeed = 5.f;

			m_pTransformCom->Get_Infomation()[Engine::CTransform::INFO_POSITION] += vDir * fTimeDelta * m_fSpeed;
		}

		if(m_pMeshCom->Get_AnimationCtrl()->GetTimeAcc() >= m_dAnimationPeriod * 0.38f && m_pMeshCom->Get_AnimationCtrl()->GetTimeAcc() <= m_dAnimationPeriod * 0.4f)
		{
			if(fDistance <= 1.5f)
				m_fSpeed = 0.f;
			else
				m_fSpeed = 3.f;

			m_pTransformCom->Get_Infomation()[Engine::CTransform::INFO_POSITION] += vDir * fTimeDelta * m_fSpeed;
		}

		if(m_pMeshCom->Get_AnimationCtrl()->GetTimeAcc() >= m_dAnimationPeriod * 0.4f && m_pMeshCom->Get_AnimationCtrl()->GetTimeAcc() <= m_dAnimationPeriod * 0.46f)
		{
			if(fDistance <= 1.5f)
				m_fSpeed = 0.f;
			else
				m_fSpeed = 1.f;

			m_pTransformCom->Get_Infomation()[Engine::CTransform::INFO_POSITION] += vDir * fTimeDelta * m_fSpeed;
		}
	}

	if(SWORDWAVE == m_iState)
	{
		if(m_pMeshCom->Get_AnimationCtrl()->GetTimeAcc() >= m_dAnimationPeriod * 0.05f && m_pMeshCom->Get_AnimationCtrl()->GetTimeAcc() <= m_dAnimationPeriod * 0.07f)
		{
			if(fDistance <= 1.5f)
				m_fSpeed = 0.f;
			else
				m_fSpeed = 1.f;
			m_pTransformCom->Get_Infomation()[Engine::CTransform::INFO_POSITION] += vDir * fTimeDelta * m_fSpeed;
		}

		if(m_pMeshCom->Get_AnimationCtrl()->GetTimeAcc() >= m_dAnimationPeriod * 0.22f && m_pMeshCom->Get_AnimationCtrl()->GetTimeAcc() <= m_dAnimationPeriod * 0.26f)
		{
			if(fDistance <= 1.5f)
				m_fSpeed = 0.f;
			else
				m_fSpeed = 1.f;
			m_pTransformCom->Get_Infomation()[Engine::CTransform::INFO_POSITION] += vDir * fTimeDelta * m_fSpeed;
		}

		if(m_pMeshCom->Get_AnimationCtrl()->GetTimeAcc() >= m_dAnimationPeriod * 0.29f && m_pMeshCom->Get_AnimationCtrl()->GetTimeAcc() <= m_dAnimationPeriod * 0.34f)
		{
			if(fDistance <= 1.5f)
				m_fSpeed = 0.f;
			else
				m_fSpeed = 2.f;
			m_pTransformCom->Get_Infomation()[Engine::CTransform::INFO_POSITION] += vDir * fTimeDelta * m_fSpeed;
		}
	}

	if(SWING_TRIPLE == m_iState)
	{
		if(m_pMeshCom->Get_AnimationCtrl()->GetTimeAcc() >= m_dAnimationPeriod * 0.18f && m_pMeshCom->Get_AnimationCtrl()->GetTimeAcc() <= m_dAnimationPeriod * 0.22f)
		{
			m_fSpeed = 15.f;
			m_pTransformCom->Go_Straight(fTimeDelta * m_fSpeed, vDir);
		}

		if(m_pMeshCom->Get_AnimationCtrl()->GetTimeAcc() >= m_dAnimationPeriod * 0.3f && m_pMeshCom->Get_AnimationCtrl()->GetTimeAcc() <= m_dAnimationPeriod * 0.33f)
		{
			_vec3 vPlayerPos = _vec3(m_pPlayerTransCom->Get_WorldMatrix()->_41, m_pPlayerTransCom->Get_WorldMatrix()->_42, m_pPlayerTransCom->Get_WorldMatrix()->_43);
			_vec3 vBossPos = _vec3(m_pTransformCom->Get_WorldMatrix()->_41, m_pTransformCom->Get_WorldMatrix()->_42, m_pTransformCom->Get_WorldMatrix()->_43);
			_vec3 vDirToPlayer = vPlayerPos - vBossPos;
			_vec3 vLook = _vec3(m_pTransformCom->Get_WorldMatrix()->_31, m_pTransformCom->Get_WorldMatrix()->_32, m_pTransformCom->Get_WorldMatrix()->_33);
			_float fDis = D3DXVec3Length(&vDirToPlayer);
			float fRad = 0.f;

			D3DXVec3Normalize(&vDirToPlayer, &vDirToPlayer);
			D3DXVec3Normalize(&vLook, &vLook);

			if(!m_bSwingTripleRotated)
			{
				fRad = D3DXVec3Dot(&vLook, &vDirToPlayer);

				if(fRad > 0.f)
					m_pTransformCom->Rotation(Engine::CTransform::ANGLE_Y, D3DXToRadian(90.f) + acosf(fRad));
				else
					m_pTransformCom->Rotation(Engine::CTransform::ANGLE_Y, D3DXToRadian(90.f) - acosf(fRad));

				m_bSwingTripleRotated = true;
			}

			if(fDis <= 1.5f)
				m_fSpeed = 0.f;
			else
				m_fSpeed = 20.f;

			m_pTransformCom->Go_Straight(fTimeDelta * m_fSpeed, vDirToPlayer);
		}
	}

	if(DOUBLE_SMASH == m_iState)
	{
		if(m_pMeshCom->Get_AnimationCtrl()->GetTimeAcc() >= m_dAnimationPeriod * 0.1f && m_pMeshCom->Get_AnimationCtrl()->GetTimeAcc() <= m_dAnimationPeriod * 0.2f)
		{
			if(fDistance <= 1.5f)
				m_fSpeed = 0.f;
			else
				m_fSpeed = 1.5f;

			m_pTransformCom->Go_Straight(fTimeDelta * m_fSpeed, vDir);
		}

		if(m_pMeshCom->Get_AnimationCtrl()->GetTimeAcc() >= m_dAnimationPeriod * 0.3f && m_pMeshCom->Get_AnimationCtrl()->GetTimeAcc() <= m_dAnimationPeriod * 0.37f)
		{
			if(fDistance <= 1.5f)
				m_fSpeed = 0.f;
			else
				m_fSpeed = 1.5f;

			m_pTransformCom->Go_Straight(fTimeDelta * m_fSpeed, vDir);
		}
	}

	if(RROTATE_ATT == m_iState)
	{
		if(m_pMeshCom->Get_AnimationCtrl()->GetTimeAcc() >= m_dAnimationPeriod * 0.2f && m_pMeshCom->Get_AnimationCtrl()->GetTimeAcc() <= m_dAnimationPeriod * 0.4f)
			m_pTransformCom->Rotation(Engine::CTransform::ANGLE_Y, 2.f * (D3DXToRadian(90.f) - fRad) * fTimeDelta);
	}

	if(LROTATE_ATT == m_iState)
	{
		if(m_pMeshCom->Get_AnimationCtrl()->GetTimeAcc() >= m_dAnimationPeriod * 0.2f && m_pMeshCom->Get_AnimationCtrl()->GetTimeAcc() <= m_dAnimationPeriod * 0.4f)
			m_pTransformCom->Rotation(Engine::CTransform::ANGLE_Y, -2.f * (fRad - D3DXToRadian(90.f)) * fTimeDelta);
	}
}

HRESULT CBoss::SetUp_ConstantTable(LPD3DXEFFECT pEffect)
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
