#ifndef Sword_Radial_Blur_Texture_h__
#define Sword_Radial_Blur_Texture_h__

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

class CSword_Radial_Blur_Texture : public Engine::CGameObject
{
private:
	explicit CSword_Radial_Blur_Texture(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CSword_Radial_Blur_Texture(void);
public:
	virtual HRESULT Ready_GameObject(_vec3 vAngle);
	_int Update_GameObject(const _float& fTimeDelta);
	void Render_GameObject(void);
	void SetRenderEnable(_bool bRender){m_bRender = bRender;}
	Engine::CTransform* GetTransform(void){return m_pTransformCom;}
	void Rotate(_vec3 vAngle);
	void SetPosition(_vec3 vPos);
private:
	Engine::CTransform*			m_pTransformCom;
	Engine::CRenderer*			m_pRendererCom;
	Engine::CTexture*			m_pTextureCom;
	Engine::CRect_Texture*		m_pBufferCom;
	Engine::CShader*			m_pShaderCom;
private:
	_float						m_fFrame;
	_bool						m_bRender;
	_bool						m_bParentExist;
	_matrix*					m_pWorld;
	_bool						m_bRotate;
	_float						m_fX, m_fY, m_fZ;
	_float						m_fTimeAcc;
	_bool						m_bStart;
private:
	HRESULT Ready_Component(void);
public:
	static CSword_Radial_Blur_Texture* Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 vAngle);
	virtual void Free(void);
};


#endif // Sword_Radial_Blur_Texture_h__
