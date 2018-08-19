#ifndef Counter_h__
#define Counter_h__

#include "Defines.h"
#include "GameObject.h"
#include "Yellow_Blunt_Texture.h"
#include "Ring2_Texture.h"

class CCounter : public Engine::CGameObject
{
private:
	explicit CCounter(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CCounter(void);
private:
	CYellow_Blunt_Texture*				m_pYellow_Blunt_Texture[4];
	CRing2_Texture*						m_pRing2_Texture;
	_float								m_fTimeAcc;
	_bool								m_bRender;
public:
	virtual HRESULT Ready_GameObject(void);
	_int Update_GameObject(const _float& fTimeDelta);
	void SetRenderEnable(_bool bRender);
public:
	static CCounter* Create(LPDIRECT3DDEVICE9 pGraphicDev);
public:
	virtual void Free(void);
};


#endif // Counter_h__
