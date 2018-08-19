#ifndef Frame_Manager_h__
#define Frame_Manager_h__

#include "Engine_Defines.h"
#include "Base.h"

BEGIN(Engine)

class CFrame;
class ENGINE_DLL CFrame_Manager : public CBase
{
	DECLARE_SINGLETON(CFrame_Manager)
private:
	explicit CFrame_Manager(void);
	virtual ~CFrame_Manager(void);
public:
	HRESULT Ready_Frames(const _tchar* pFrameTag, const _float& fCallCnt);	
	bool Permit_Call(const _tchar* pFrameTag, const _float& fTimeDelta);
private:
	map<const _tchar*, CFrame*>			m_mapFrames;
	typedef map<const _tchar*, CFrame*>	MAPFRAMES;
private:
	CFrame* Find_Frame(const _tchar* pFrameTag);
private:
	virtual void Free(void);

};


END

#endif // Frame_Manager_h__
