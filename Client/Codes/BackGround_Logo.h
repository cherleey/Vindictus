#ifndef BackGround_Logo_h__
#define BackGround_Logo_h__

#include "Defines.h"
#include "GameObject.h"

namespace Engine
{
	class CRenderer;
	class CTransform;
	class CTexture;
	class CRect_Texture;
	class CShader;
}

class CBackGround_Logo : public Engine::CGameObject
{
private:
	explicit CBackGround_Logo(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CBackGround_Logo(void);
public:
	virtual HRESULT Ready_GameObject(void);
	_int Update_GameObject(const _float& fTimeDelta);
	void Render_GameObject(void);
private:
	Engine::CTransform*			m_pTransformCom;
	Engine::CRenderer*			m_pRendererCom;
	Engine::CTexture*			m_pTextureCom;
	Engine::CRect_Texture*		m_pBufferCom;
	Engine::CShader*			m_pShaderCom;
private:
	HRESULT Ready_Component(void);
public:
	static CBackGround_Logo* Create(LPDIRECT3DDEVICE9 pGraphicDev);
private:
	virtual void Free(void);
};


#endif // BackGround_Logo_h__
