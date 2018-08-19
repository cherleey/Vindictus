#ifndef Attack_Strong_0_h__
#define Attack_Strong_0_h__

#include "Defines.h"
#include "GameObject.h"
#include "Sword_Sharp_Line_Texture.h"
#include "Sword_Sharp_Plane_Texture.h"
#include "Sword_Radial_Blur_Texture.h"

class CAttack_Strong_0 : public Engine::CGameObject
{
private:
	explicit CAttack_Strong_0(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CAttack_Strong_0(void);
private:
	CSword_Sharp_Line_Texture*			m_pSword_Sharp_Line_Texture[4];
	CSword_Sharp_Plane_Texture*			m_pSword_Sharp_Plane_Texture;
	CSword_Radial_Blur_Texture*			m_pSword_Radial_Blur_Texture;
	_float								m_fTimeAcc;
	_bool								m_bRender;
public:
	virtual HRESULT Ready_GameObject(void);
	_int Update_GameObject(const _float& fTimeDelta);
	void SetRenderEnable(_bool bRender);
public:
	static CAttack_Strong_0* Create(LPDIRECT3DDEVICE9 pGraphicDev);
public:
	virtual void Free(void);
};


#endif // Attack_Strong_0_h__
