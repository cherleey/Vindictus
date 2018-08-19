#ifndef Attack_Strong_1_h__
#define Attack_Strong_1_h__

#include "Defines.h"
#include "GameObject.h"
#include "Shield_Zoom_Texture.h"
#include "Ring2_Texture.h"
#include "Shape_Up_Shot_Texture.h"

class CAttack_Strong_2 : public Engine::CGameObject
{
private:
	explicit CAttack_Strong_2(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CAttack_Strong_2(void);
private:
	CShield_Zoom_Texture*				m_pShield_Zoom[4];
	CRing2_Texture*						m_pRing2;
	CShape_Up_Shot_Texture*				m_pShape_Up_Shot_Texture[4];
	_float								m_fTimeAcc;
	_bool								m_bRender;
public:
	virtual HRESULT Ready_GameObject(void);
	_int Update_GameObject(const _float& fTimeDelta);
	void SetRenderEnable(_bool bRender);
public:
	static CAttack_Strong_2* Create(LPDIRECT3DDEVICE9 pGraphicDev);
public:
	virtual void Free(void);
};


#endif // Attack_Strong_0_h__
