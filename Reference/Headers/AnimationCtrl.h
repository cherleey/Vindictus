#ifndef AnimationCtrl_h__
#define AnimationCtrl_h__

#include "Engine_Defines.h"
#include "Base.h"

BEGIN(Engine)

class ENGINE_DLL CAnimationCtrl : public CBase
{
private:
	explicit CAnimationCtrl(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CAnimationCtrl(const CAnimationCtrl& rhs);
	virtual ~CAnimationCtrl(void);
public:
	LPD3DXANIMATIONCONTROLLER* Get_AnimationCtrl(void) {return &m_pAnimationCtrl;}
	double Get_Period(void) {return m_dPeriod;}
	LPD3DXTRACK_DESC Get_TrackInfo(void);
public:
	HRESULT Ready_AnimationCtrl(void);
	void Set_AnimationSet(const _uint& iIndex, const _float& fSpeed = 1.5f);
	void Play_AnimationSet(const _float& fTimeDelta);
	_double GetTimeAcc(void){return m_dTimeAcc;}
private:
	LPDIRECT3DDEVICE9				m_pGraphicDev;
	LPD3DXANIMATIONCONTROLLER		m_pAnimationCtrl;
	_uint							m_iMaxNumAniSet;
	_uint							m_iOldIndex;
	_ulong							m_dwCurrentTrack;
	_ulong							m_dwNewTrack;
	_double							m_dTimeAcc;
	_double							m_dPeriod;
	LPD3DXTRACK_DESC				m_pTrackInfo;
public:
	static CAnimationCtrl* Create(LPDIRECT3DDEVICE9 pGraphicDev);
	static CAnimationCtrl* Create(const CAnimationCtrl& rhs);
private:
	virtual void Free(void);

};

END

#endif // AnimationCtrl_h__
