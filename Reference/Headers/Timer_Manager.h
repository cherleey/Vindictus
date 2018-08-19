#ifndef Timer_Manager_h__
#define Timer_Manager_h__

#include "Engine_Defines.h"
#include "Base.h"

BEGIN(Engine)

class CTimer;
class ENGINE_DLL CTimer_Manager : public CBase
{
	DECLARE_SINGLETON(CTimer_Manager)
private:
	explicit CTimer_Manager(void);
	virtual ~CTimer_Manager(void);
public:
	_float Get_TimeDelta(const _tchar* pTimerTag);
public:
	HRESULT Ready_Timers(const _tchar* pTimerTag);
	void SetUp_TimeDelta(const _tchar* pTimerTag);
private:
	map<const _tchar*, CTimer*>			m_mapTimers;
	typedef map<const _tchar*, CTimer*>	MAPTIMERS;
private:
	CTimer* Find_Timer(const _tchar* pTimerTag);
private:
	virtual void Free(void);

};


END

#endif // Timer_Manager_h__
