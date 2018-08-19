#ifndef Navi_h__
#define Navi_h__

#include "Defines.h"
#include "GameObject.h"

namespace Engine
{
	class CRenderer;
	class CTransform;	
	class CStaticMesh;
	class CMaterial;
}

class CNavi : public Engine::CGameObject
{
private:
	explicit CNavi(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CNavi(void);
public:
	virtual HRESULT Ready_GameObject(void);
	_int Update_GameObject(const _float& fTimeDelta);
	_int LastUpdate_GameObject(const _float& fTimeDelta);
	void Render_GameObject(void);
private:
	Engine::CTransform*			m_pTransformCom;
	Engine::CRenderer*			m_pRendererCom;	
	Engine::CStaticMesh*		m_pMeshCom;
	Engine::CMaterial*			m_pMtrlCom;
private:
	HRESULT Ready_Component(void);
public:
	static CNavi* Create(LPDIRECT3DDEVICE9 pGraphicDev);
private:
	virtual void Free(void);
};


#endif
