#include "Timer.h"

USING(Engine)

Engine::CTimer::CTimer(void)
: m_fTimeDelta(0.f)
{
	ZeroMemory(&m_FrameTime, sizeof(LARGE_INTEGER));
	ZeroMemory(&m_FixTime, sizeof(LARGE_INTEGER));
	ZeroMemory(&m_LastTime, sizeof(LARGE_INTEGER));
	ZeroMemory(&m_CpuTick, sizeof(LARGE_INTEGER));
}

Engine::CTimer::~CTimer(void)
{

}

HRESULT Engine::CTimer::Ready_Timer(void)
{
	QueryPerformanceCounter(&m_FrameTime); // ���� cpu�� ī������ ���ڸ� ����ִ���.
	QueryPerformanceCounter(&m_FixTime);
	QueryPerformanceCounter(&m_LastTime);
	QueryPerformanceFrequency(&m_CpuTick); // cpu�� 1�ʿ� ī�����Ҽ� �ִ� ���ڸ� ��������.

	return S_OK;
}

void Engine::CTimer::SetUp_TimeDelta(void)
{
	QueryPerformanceCounter(&m_FrameTime);

	if(m_FrameTime.QuadPart - m_LastTime.QuadPart >= m_CpuTick.QuadPart)
	{
		// cpu�� 1�ʿ� ī�����Ҽ� �ִ� ���ڸ� ��������.
		QueryPerformanceFrequency(&m_CpuTick);
		m_LastTime = m_FrameTime;
	}	 

	m_fTimeDelta = _float(m_FrameTime.QuadPart - m_FixTime.QuadPart) / m_CpuTick.QuadPart;

	m_FixTime = m_FrameTime;	
}

CTimer* Engine::CTimer::Create(void)
{
	CTimer*		pInstance = new CTimer;

	if(FAILED(pInstance->Ready_Timer()))
	{
		MSG_BOX("CTimer Created Failed");
		Engine::Safe_Release(pInstance);
	}
	return pInstance;
}

void Engine::CTimer::Free(void)
{

}

