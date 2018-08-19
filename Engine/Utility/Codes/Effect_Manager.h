#ifndef Effect_Manager_h__
#define Effect_Manager_h__

#include "Engine_Defines.h"
#include "Base.h"

class CEffect;
class CEffect_Manager : public CBase
{
	DECLARE_SINGLETON(CEffect_Manager)
private:
	explicit CEffect_Manager(void);
	virtual ~CEffect_Manager(void);
	virtual void Free(void);
//private:
//	map<const _tchar*, CEffect*>* m_pMapEffect;
//	typedef map<const _tchar*, CEffect*> MAPEFFECT;
//public:
//	HRESULT CreateEffect(LPDIRECT3DDEVICE9 pGraphicDev);
};

#endif
