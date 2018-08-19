#include "Frame.h"

USING(Engine)

Engine::CFrame::CFrame(void)
: m_fCallPerSec(0.f)
, m_fTimeAcc(0.f)
{
	
}

Engine::CFrame::~CFrame(void)
{

}

HRESULT Engine::CFrame::Ready_Frame(const _float& fCallCnt)
{
	m_fCallPerSec = 1.f / fCallCnt;

	return S_OK;
}

bool Engine::CFrame::Permit_Call(const _float& fTimeDelta)
{
	m_fTimeAcc += fTimeDelta;

	if(m_fTimeAcc > m_fCallPerSec) // 일분에 백초가 지나면
	{
		m_fTimeAcc = 0.f;
		return true;
	}

	// 백분의 일초가 지나면
	return false;
}

CFrame* Engine::CFrame::Create(const _float& fCallCnt)
{
	CFrame*		pInstance = new CFrame;

	if(FAILED(pInstance->Ready_Frame(fCallCnt)))
	{
		MSG_BOX("CFrame Created Failed");
		Engine::Safe_Release(pInstance);
	}
	return pInstance;
}

void Engine::CFrame::Free(void)
{

}

