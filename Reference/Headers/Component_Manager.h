#ifndef Component_Manager_h__
#define Component_Manager_h__

#include "Engine_Defines.h"
#include "Base.h"

BEGIN(Engine)

class CComponent;
class ENGINE_DLL CComponent_Manager : public CBase
{
	DECLARE_SINGLETON(CComponent_Manager)	
private:
	explicit CComponent_Manager(void);
	virtual ~CComponent_Manager(void);
public:
	HRESULT Reserve_ComponentMgr(const _uint& iSize);
	HRESULT Add_Component(const _uint& iIndex, const _tchar* pComponentTag, CComponent* pComponent);
	CComponent* Clone_Component(const _uint& iIndex, const _tchar* pComponentTag);
	void Release_Component(const _uint& iIndex);
private:
	map<const _tchar*, CComponent*>*		m_pMapComponent; // 동적할당할려고.
	typedef map<const _tchar*, CComponent*>	MAPCOMPONENT;
private:
	_uint									m_iContainerSize;
private:
	CComponent* Find_Component(const _uint& iIndex, const _tchar* pComponentTag);

private:
	virtual void Free(void);

};

END

#endif // Component_Manager_h__
