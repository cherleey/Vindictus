#ifndef Attack_Strong_3_h__
#define Attack_Strong_3_h__

#include "Defines.h"
#include "GameObject.h"
#include "Wind_Texture.h"
#include "Blunt_Texture.h"
#include "Ring_Outer_Wind06_Texture.h"

class CAttack_Strong_3 : public Engine::CGameObject
{
private:
	explicit CAttack_Strong_3(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CAttack_Strong_3(void);
private:
	CWind_Texture*						m_pWind_Texture;
	CBlunt_Texture*						m_pBlunt_Texture[4];
	CRing_Outer_Wind06_Texture*			m_pRing_Outer_Wind;
	_float								m_fTimeAcc;
	_bool								m_bRender;
public:
	virtual HRESULT Ready_GameObject(void);
	_int Update_GameObject(const _float& fTimeDelta);
	void SetRenderEnable(_bool bRender);
public:
	static CAttack_Strong_3* Create(LPDIRECT3DDEVICE9 pGraphicDev);
public:
	virtual void Free(void);
};


#endif // Attack_Strong_3_h__
