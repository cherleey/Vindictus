#include "Timer_Manager.h"
#include "Timer.h"

USING(Engine)
IMPLEMENT_SINGLETON(CTimer_Manager)

Engine::CTimer_Manager::CTimer_Manager(void)
{

}

Engine::CTimer_Manager::~CTimer_Manager(void)
{

}

_float CTimer_Manager::Get_TimeDelta(const _tchar* pTimerTag)
{
	CTimer*	pTimer = Find_Timer(pTimerTag);

	if(NULL == pTimer)
		return 0.f;

	return pTimer->Get_TimeDelta();
}

HRESULT Engine::CTimer_Manager::Ready_Timers(const _tchar* pTimerTag)
{
	CTimer*	pTimer = Find_Timer(pTimerTag);

	if(NULL != pTimer)
		return E_FAIL;

	pTimer = CTimer::Create();
	if(NULL == pTimer)
		return E_FAIL;

	m_mapTimers.insert(MAPTIMERS::value_type(pTimerTag, pTimer));	

	return S_OK;
}

void Engine::CTimer_Manager::SetUp_TimeDelta(const _tchar* pTimerTag)
{
	CTimer*	pTimer = Find_Timer(pTimerTag);

	if(NULL == pTimer)
		return;

	pTimer->SetUp_TimeDelta();	
}

/*
_bool Compare(map<const _tchar*, CTimer*>::value_type pair)
{

	return true;
}*/

CTimer* Engine::CTimer_Manager::Find_Timer(const _tchar* pTimerTag)
{
	CTag_Finder		TagFinder(pTimerTag);

	MAPTIMERS::iterator iter = find_if(m_mapTimers.begin(), m_mapTimers.end(), TagFinder);

	if(iter == m_mapTimers.end())
		return NULL;

	return iter->second;

}

void Engine::CTimer_Manager::Free(void)
{
	for_each(m_mapTimers.begin(), m_mapTimers.end(), CRelease_Pair());
	m_mapTimers.clear();

}

