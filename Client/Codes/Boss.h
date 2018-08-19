#ifndef Boss_h__
#define Boss_h__

#include "Defines.h"
#include "GameObject.h"

namespace Engine
{
	class CRenderer;
	class CTransform;	
	class CDynamicMesh;
	class CMaterial;
	class CBoxCollider;
	class CShader;
}

class CBoss : public Engine::CGameObject
{
public:
	enum STATE {IDLE, RUN, NORMALATTACK, PIERCE, PIERCE_SPIN, SLASH_SPIN, SWING_TRIPLE, SWORDWAVE, DOUBLE_SMASH, WHIRLWIND, AFTERATT, ROTATE, GROGGY, PHASE, RROTATE_ATT, LROTATE_ATT, END_STATE};
	enum ATT_STATE {NORMAL, SMASH};
	bool m_bBack;
private:
	explicit CBoss(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CBoss(void);
public:
	virtual HRESULT Ready_GameObject();
	_int Update_GameObject(const _float& fTimeDelta);
	void Render_GameObject(void);
	void SetIsCol(_bool bIsCol, _float fDamage, _float fGroggyDamage, const _float& fTimeDelta);
	ATT_STATE GetAttState(void){return m_eAttState;}
private:
	Engine::CTransform*			m_pTransformCom;
	Engine::CRenderer*			m_pRendererCom;	
	Engine::CDynamicMesh*		m_pMeshCom;
	Engine::CMaterial*			m_pMtrlCom;
	Engine::CBoxCollider*		m_pColliderCom;
	Engine::CTransform*			m_pPlayerTransCom;
	Engine::CShader*			m_pShaderCom;
private:
	_uint						m_iState;
	_float						m_fTimeDelta;
	_float						m_fSpeed;
	_double						m_dAnimationPeriod;
	LPD3DXTRACK_DESC			m_pTrackInfo;
	_uint						m_iSRangePattern;
	_uint						m_iLRangePattern;
	_bool						m_bRotating;
	_bool						m_bRunning;
	_bool						m_bAttacking;
	_bool						m_bStop;
	_vec3						m_vOriginDir;
	_float						m_fHP;
	_float						m_fGroggyHP;
	_bool						m_bGroggy;
	_float						m_fDamageTime;
	ATT_STATE					m_eAttState;
	_bool						m_bSwingTripleRotated;
	_uint						m_iPhase;
	_bool						m_bRotateAtt;
	_bool						m_bColWithMap;
private:
	HRESULT Ready_Component(void);
	void CorrectPosisionWhileAttack(const _float& fTimeDelta, const _vec3& vDir, _float fDistance = 0.f, const _float& fRad = 0.f, const _float& fCos = 0.f);
	void Pattern(const _float& fTimeDelta);
	void CollisionWhileAttack(void);
	HRESULT SetUp_ConstantTable(LPD3DXEFFECT pEffect);
public:
	static CBoss* Create(LPDIRECT3DDEVICE9 pGraphicDev);
private:
	virtual void Free(void);
};


#endif // Boss_h__
