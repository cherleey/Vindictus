#ifndef Player_h__
#define Player_h__

#include "Defines.h"
#include "GameObject.h"
#include "Camera_Stage.h"
#include "Trail.h"
#include "Attack_Strong_0.h"
#include "Attack_Strong_2.h"
#include "Attack_Strong_3.h"
#include "Phantom_Trail.h"
#include "Counter.h"
#include "HeavyStanderCounter.h"
#include "DashSmash.h"

namespace Engine
{
	class CRenderer;
	class CTransform;	
	class CDynamicMesh;
	class CMaterial;
	class CBoxCollider;
	class CShader;
}

class CPlayer : public Engine::CGameObject
{
public:
	enum STATE {IDLE, RUN, SPRINT, ATTACK, SMASH, SPRINTATT, AFTERATT, COUNTER, HCOUNTER, GUARD, HGUARD, DOWN, LAYBACK, END_STATE};
	bool m_bBack;
private:
	explicit CPlayer(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CPlayer(void);
public:
	virtual HRESULT Ready_GameObject(CCamera_Stage* pCamera);
	_int Update_GameObject(const _float& fTimeDelta);
	void Render_GameObject(void);
	void SetIsCol(_bool bIsCol, _float iDamage, _uint iType);
	_uint Get_State(void);
private:
	Engine::CTransform*			m_pTransformCom;
	Engine::CRenderer*			m_pRendererCom;	
	Engine::CDynamicMesh*		m_pMeshCom;
	Engine::CMaterial*			m_pMtrlCom;
	Engine::CBoxCollider*		m_pColliderCom;
	Engine::CBoxCollider*		m_pPreHitColliderCom;
	Engine::CBoxCollider*		m_pColliderCom_Foot;
	Engine::CShader*			m_pShaderCom;
private:
	_uint						m_iState;
	_float						m_fTimeDelta;
	_float						m_fSpeed;
	_double						m_dAnimationPeriod;
	_uint						m_iAttackNum;
	LPD3DXTRACK_DESC			m_pTrackInfo;
	CCamera_Stage*				m_pCamera;
	_bool						m_bRunning;
	_bool						m_bSprinting;
	_bool						m_bAttacking;
	_bool						m_bSmashing;
	_bool						m_bGuarding;
	_bool						m_bCancleEnable;
	_bool						m_bIsCol;
	_bool						m_bDown;
	_bool						m_bAttackedFromBack;
	_bool						m_bDownRotate;
	_bool						m_bAttackRotate;
	_bool						m_bGuarded;
	_bool						m_bPreHit;
	_bool						m_bPGuard;
	_bool						m_bStumbling;
	_bool						m_bCounterEnable;
	_bool						m_bColWithMap;
	_float						m_fDamage;
	_float						m_fGroggyDamage;
	_uint						m_iDownState;
	_uint						m_iSmashNum;
	_float						m_fAttackedRad;
	_double						m_dBossTimeAcc;
	_double						m_dBossAnimPeriod;
	_float						m_fBossOffSet;
	_float						m_fPGuardTiming;
	_uint						m_iCounterNum;
	_float						m_fSpeedOffSet;
	CTrail*						m_pTrail;
	CAttack_Strong_0*			m_pAttackStrong0;
	CAttack_Strong_2*			m_pAttackStrong2;
	CAttack_Strong_3*			m_pAttackStrong3;
	CPhantom_Trail*				m_pPhantom_Trail;
	CCounter*					m_pCounter;
	CHeavyStanderCounter*		m_pHeavyStanderCounter;
	CDashSmash*					m_pDashSmash;
private:
	HRESULT Ready_Component(void);
	void Check_InputDev(const _float& fTimeDelta);
	void CorrectPosisionWhileAttack(const _float& fTimeDelta);
	void CollisionWhileAttack(const _float& fTimeDelta);
	void CancleEnable(void);
	HRESULT SetUp_ConstantTable(LPD3DXEFFECT pEffect);
public:
	static CPlayer* Create(LPDIRECT3DDEVICE9 pGraphicDev, CCamera_Stage* pCamera);
private:
	virtual void Free(void);
};


#endif // Terrain_h__
