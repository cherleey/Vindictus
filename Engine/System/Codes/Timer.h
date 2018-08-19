#ifndef Engine_Timer_h__
#define Engine_Timer_h__

#include "Engine_Defines.h"
#include "Base.h"

// TimeDelta를 셋팅한다.

BEGIN(Engine)

class ENGINE_DLL CTimer : public CBase
{
private:
	explicit CTimer(void);
	virtual ~CTimer(void);
public:
	_float Get_TimeDelta(void) const {
		return m_fTimeDelta;}
public:
	HRESULT Ready_Timer(void);
	void SetUp_TimeDelta(void);
private:
	LARGE_INTEGER			m_FrameTime;
	LARGE_INTEGER			m_FixTime;
	LARGE_INTEGER			m_LastTime;
	LARGE_INTEGER			m_CpuTick;
private:
	_float					m_fTimeDelta;
public:
	static CTimer* Create(void);
private:
	virtual void Free(void);
};

END

#endif // Timer_h__
