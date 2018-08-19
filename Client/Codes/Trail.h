#ifndef Trail_h__
#define Trail_h__

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

class CTrail : public Engine::CGameObject
{
private:
	explicit CTrail(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CTrail(void);
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
	_uint						m_iTrailNum;
	_bool						m_bRender;
private:
	list<_vec4>					m_TrailLIst;
	_vec4						m_TransformedVertices[22];
	typedef list<_vec4>		TRAILLIST;
private:
	HRESULT Ready_Component(void);
public:
	static CTrail* Create(LPDIRECT3DDEVICE9 pGraphicDev);
public:
	virtual void Free(void);
};


#endif // BackGround_Logo_h__
