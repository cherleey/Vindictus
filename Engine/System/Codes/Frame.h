#ifndef Engine_Frame_h__
#define Engine_Frame_h__

#include "Engine_Defines.h"
#include "Base.h"

// 초당 렌더링할수 있는 횟수(FPS)를 제어한다.
BEGIN(Engine)

class ENGINE_DLL CFrame : public CBase
{
private:
	explicit CFrame(void);
	virtual ~CFrame(void);
public:
	HRESULT Ready_Frame(const _float& fCallCnt);
	bool Permit_Call(const _float& fTimeDelta);
private:
	_float		m_fCallPerSec;
	_float		m_fTimeAcc;
public:
	static CFrame* Create(const _float& fCallCnt);
private:
	virtual void Free(void);
};

END

#endif // Timer_h__
