#ifndef Phantom_Trail_h__
#define Phantom_Trail_h__

#include "Defines.h"
#include "GameObject.h"
#include "Sword.h"

namespace Engine
{
	class CRenderer;
	class CTransform;
	class CTexture;
	class CTrail_Texture;
	class CShader;
	class CBoxCollider;
}

class CPhantom_Trail : public Engine::CGameObject
{
private:
	explicit CPhantom_Trail(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CPhantom_Trail(void);
public:
	virtual HRESULT Ready_GameObject(void);
	_int Update_GameObject(const _float& fTimeDelta);
	void Render_GameObject(void);
	void SetRenderEnable(_bool bRender){m_bRender = bRender;}
private:
	Engine::CTransform*			m_pTransformCom;
	Engine::CRenderer*			m_pRendererCom;
	Engine::CTexture*			m_pTextureCom;
	Engine::CTrail_Texture*		m_pBufferCom;
	Engine::CShader*			m_pShaderCom;
private:
	_float						m_fFrame;
	_uint						m_iPhantom_TrailNum;
	_bool						m_bRender;
private:
	list<_vec4>					m_Phantom_TrailLIst;
	_vec4						m_TransformedVertices[22];
	typedef list<_vec4>		Phantom_TrailLIST;
private:
	HRESULT Ready_Component(void);
public:
	static CPhantom_Trail* Create(LPDIRECT3DDEVICE9 pGraphicDev);
public:
	virtual void Free(void);
};


#endif // BackGround_Logo_h__
