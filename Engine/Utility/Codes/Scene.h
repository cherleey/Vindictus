#ifndef Scene_h__
#define Scene_h__

#include "Engine_Defines.h"
#include "Base.h"
#include "GameObject.h"

BEGIN(Engine)

class CLayer;
class CComponent;
class ENGINE_DLL CScene : public CBase
{
protected:
	explicit CScene(LPDIRECT3DDEVICE9	pGraphicDev);
	virtual ~CScene(void);
public:
	Engine::CComponent* Get_Component(const _tchar* pLayerTag, Engine::CGameObject::COMTYPE eType, const _tchar* pComponentTag, const _uint& iObjectIdx = 0);
public:
	HRESULT Add_Layer(const _tchar* pLayerTag, CLayer* pLayer);
public:
	virtual HRESULT Ready_Scene(void);
	virtual _int Update_Scene(const _float& fTimeDelta);
	virtual void Render_Scene(void) PURE;
protected:
	LPDIRECT3DDEVICE9		m_pGraphicDev;
private:
	map<const _tchar*, CLayer*>			m_mapLayers;
public:
	typedef map<const _tchar*, CLayer*>	MAPLAYERS;
public:
	CLayer* Find_Layer(const _tchar* pLayerTag);
protected:
	virtual void Free(void);
};

END

#endif // Scene_h__
