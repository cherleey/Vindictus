#ifndef Component_h__
#define Component_h__

#include "Engine_Defines.h"
#include "Base.h"

BEGIN(Engine)

class ENGINE_DLL CComponent : public CBase
{
protected:
	explicit CComponent(void);
	virtual ~CComponent(void);
public:
	virtual HRESULT Ready_Component(void);
	virtual void Update_Component(void) {}
public:
	virtual CComponent* Clone(void)PURE;
protected:
	virtual void Free(void);


};


END

#endif // Component_h__
