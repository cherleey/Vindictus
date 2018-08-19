#ifndef Layer_h__
#define Layer_h__

#include "Engine_Defines.h"
#include "GameObject.h"
#include "Base.h"

BEGIN(Engine)


class CComponent;
class ENGINE_DLL CLayer : public CBase
{
private:
	explicit CLayer(void);
	virtual ~CLayer(void);
public:
	Engine::CComponent* Get_Component(Engine::CGameObject::COMTYPE eType, const _tchar* pComponentTag, const _uint& iObjectIdx = 0);
public:
	inline void Add_GameObject(CGameObject* pGameObject) {
		m_ObjectList.push_back(pGameObject);}
	list<CGameObject*>* Get_GameObject(void){
		return &m_ObjectList;}
public:
	HRESULT Ready_Layer(void);
	_int Update_Layer(const _float& fTimeDelta);	
	_int LastUpdate_Layer(const _float& fTimeDelta);	
private:
	list<CGameObject*>			m_ObjectList;
public:
	typedef list<CGameObject*>	OBJECTLIST;
public:
	static CLayer* Create(void);
private:
	virtual void Free(void);
};

END

#endif // Layer_h__
