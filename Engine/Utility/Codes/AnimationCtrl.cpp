#include "AnimationCtrl.h"

USING(Engine)

Engine::CAnimationCtrl::CAnimationCtrl(LPDIRECT3DDEVICE9 pGraphicDev)
: m_pGraphicDev(pGraphicDev)
, m_pAnimationCtrl(NULL)
, m_dwCurrentTrack(0)
, m_dwNewTrack(1)
, m_dTimeAcc(0.0)
, m_iOldIndex(999)
, m_iMaxNumAniSet(0)
{
	m_pGraphicDev->AddRef();
}

Engine::CAnimationCtrl::CAnimationCtrl(const CAnimationCtrl& rhs)
: m_pGraphicDev(rhs.m_pGraphicDev)
, m_dwCurrentTrack(rhs.m_dwCurrentTrack)
, m_dwNewTrack(rhs.m_dwNewTrack)
, m_dTimeAcc(rhs.m_dTimeAcc)
, m_iOldIndex(rhs.m_iOldIndex)
, m_iMaxNumAniSet(rhs.m_iMaxNumAniSet)
{
	rhs.m_pAnimationCtrl->CloneAnimationController(rhs.m_pAnimationCtrl->GetMaxNumAnimationOutputs()
		, m_iMaxNumAniSet = rhs.m_pAnimationCtrl->GetMaxNumAnimationSets(), rhs.m_pAnimationCtrl->GetMaxNumTracks()
		, rhs.m_pAnimationCtrl->GetMaxNumEvents(), &m_pAnimationCtrl);	

	m_pGraphicDev->AddRef();
}

Engine::CAnimationCtrl::~CAnimationCtrl(void)
{

}

HRESULT Engine::CAnimationCtrl::Ready_AnimationCtrl(void)
{

	return S_OK;
}

void Engine::CAnimationCtrl::Set_AnimationSet(const _uint& iIndex, const _float& fSpeed)
{
	if(m_iMaxNumAniSet <= iIndex)
		return;

	if(m_iOldIndex == iIndex)
		return;

	LPD3DXANIMATIONSET	pAS = NULL;
	
	m_pAnimationCtrl->GetAnimationSet(iIndex, &pAS);

	if(NULL == pAS)
		return;

	m_dPeriod = pAS->GetPeriod();

	m_dwNewTrack = m_dwCurrentTrack == 0 ? 1 : 0;

	m_pAnimationCtrl->SetTrackAnimationSet(m_dwNewTrack, pAS);

	Engine::Safe_Release(pAS);

	m_pAnimationCtrl->UnkeyAllTrackEvents(m_dwCurrentTrack);
	m_pAnimationCtrl->UnkeyAllTrackEvents(m_dwNewTrack);

	m_pAnimationCtrl->KeyTrackEnable(m_dwCurrentTrack, FALSE, m_dTimeAcc + 0.2);
	m_pAnimationCtrl->KeyTrackWeight(m_dwCurrentTrack, 0.1f, m_dTimeAcc, 0.2, D3DXTRANSITION_LINEAR);
	m_pAnimationCtrl->KeyTrackSpeed(m_dwCurrentTrack, fSpeed, m_dTimeAcc, 0.2, D3DXTRANSITION_LINEAR);

	m_pAnimationCtrl->SetTrackEnable(m_dwNewTrack, TRUE);
	m_pAnimationCtrl->KeyTrackWeight(m_dwNewTrack, 0.9f, m_dTimeAcc, 0.2, D3DXTRANSITION_LINEAR);
	m_pAnimationCtrl->KeyTrackSpeed(m_dwNewTrack, fSpeed, m_dTimeAcc, 0.2, D3DXTRANSITION_LINEAR);

	m_pAnimationCtrl->ResetTime();

	m_dTimeAcc = 0.0;

	m_pAnimationCtrl->SetTrackPosition(m_dwNewTrack, 0.0);

	m_iOldIndex = iIndex;

	m_dwCurrentTrack = m_dwNewTrack;
}

void Engine::CAnimationCtrl::Play_AnimationSet(const _float& fTimeDelta)
{
	m_pAnimationCtrl->AdvanceTime(fTimeDelta, NULL);

	m_dTimeAcc += fTimeDelta;
}



CAnimationCtrl* Engine::CAnimationCtrl::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CAnimationCtrl*		pInstance = new CAnimationCtrl(pGraphicDev);

	if(FAILED(pInstance->Ready_AnimationCtrl()))
	{
		MSG_BOX("CAnimationCtrl Created Failed");
		Engine::Safe_Release(pInstance);
	}
	return pInstance;
}

CAnimationCtrl* Engine::CAnimationCtrl::Create(const CAnimationCtrl& rhs)
{
	CAnimationCtrl*		pInstance = new CAnimationCtrl(rhs);

	return pInstance;
}

void Engine::CAnimationCtrl::Free(void)
{
	Engine::Safe_Release(m_pAnimationCtrl);
	Engine::Safe_Release(m_pGraphicDev);
}

LPD3DXTRACK_DESC Engine::CAnimationCtrl::Get_TrackInfo(void)
{
	if(FAILED(m_pAnimationCtrl->GetTrackDesc(m_dwCurrentTrack, m_pTrackInfo)))
		return NULL;

	return m_pTrackInfo;
}

