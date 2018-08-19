#ifndef HeavyStanderCounter_h__
#define HeavyStanderCounter_h__

#include "Defines.h"
#include "GameObject.h"
#include "Ring_Disk_Texture.h"
#include "Ring2_Texture.h"
#include "Holy_Shield_Texture.h"

class CHeavyStanderCounter : public Engine::CGameObject
{
private:
	explicit CHeavyStanderCounter(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CHeavyStanderCounter(void);
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
	static CHeavyStanderCounter* Create(LPDIRECT3DDEVICE9 pGraphicDev);
public:
	virtual void Free(void);
};


#endif // HeavyStanderCounter_h__
