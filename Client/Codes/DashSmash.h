#ifndef DashSmash_h__
#define DashSmash_h__

#include "Defines.h"
#include "GameObject.h"
#include "Ring_Disk_Texture.h"
#include "Ring2_Texture.h"
#include "Holy_Shield_Texture.h"

class CDashSmash : public Engine::CGameObject
{
private:
	explicit CDashSmash(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CDashSmash(void);
private:
	CRing_Disk_Texture*					m_pRing_Disk_Texture[5];
	CRing2_Texture*						m_pRing2_Texture;
	CHoly_Shield_Texture*				m_pHoly_Shield_Texture;
	_float								m_fTimeAcc;
	_bool								m_bRender;
public:
	virtual HRESULT Ready_GameObject(void);
	_int Update_GameObject(const _float& fTimeDelta);
	void SetRenderEnable(_bool bRender);
public:
	static CDashSmash* Create(LPDIRECT3DDEVICE9 pGraphicDev);
public:
	virtual void Free(void);
};


#endif // DashSmash_h__
