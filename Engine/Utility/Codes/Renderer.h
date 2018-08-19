#ifndef Renderer_h__
#define Renderer_h__

#include "Component.h"

// 객체들을 그린다.(순서대로)
// 그려져야할 객체들응ㄹ 모아놓을것이다.


BEGIN(Engine)

class CGameObject;
class ENGINE_DLL CRenderer : public CComponent
{
public:
	enum RENDER {RENDER_PRIORITY, RENDER_NONEALPHA, RENDER_ALPHA, RENDER_UI, RENDER_END};
private:
	explicit CRenderer(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CRenderer(void);
public:
	HRESULT Add_RenderList(RENDER eType, CGameObject* pGameObject);
public:
	virtual HRESULT Ready_Component(void);
	void Render_GameObject(void);
private:
	LPDIRECT3DDEVICE9				m_pGraphicDev;
private:
	list<CGameObject*>				m_RenderList[RENDER_END];
	typedef list<CGameObject*>		RENDERLIST;
private:
	void Render_Priority(void);
	void Render_NoneAlpha(void);
	void Render_Alpha(void);
	void Render_UI(void);
	void Clear_RenderList(void);
public:
	virtual CComponent* Clone(void);
	static CRenderer* Create(LPDIRECT3DDEVICE9 pGraphicDev);
private:
	virtual void Free(void);
};

END

#endif // Renderer_h__
