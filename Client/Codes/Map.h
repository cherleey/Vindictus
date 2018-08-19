#ifndef Map_h__
#define Map_h__

#include "Defines.h"
#include "GameObject.h"

namespace Engine
{
	class CRenderer;
	class CTransform;	
	class CStaticMesh;
	class CMaterial;
	class CBoxCollider;
}

class CMap : public Engine::CGameObject
{
private:
	explicit CMap(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CMap(void);
public:
	virtual HRESULT Ready_GameObject(void);
	_int Update_GameObject(const _float& fTimeDelta);
	_int LastUpdate_GameObject(const _float& fTimeDelta);
	void Render_GameObject(void);
	list<Engine::CBoxCollider*>* GetCollider(void);
private:
	Engine::CTransform*			m_pTransformCom;
	Engine::CRenderer*			m_pRendererCom;	
	Engine::CStaticMesh*		m_pMeshCom;
	Engine::CMaterial*			m_pMtrlCom;
	Engine::CBoxCollider*		m_pColliderCom;
private:
	list<Engine::CBoxCollider*> m_ColliderList;
private:
	HRESULT Ready_Component(void);
public:
	typedef list<Engine::CBoxCollider*> COLLIDER;
public:
	static CMap* Create(LPDIRECT3DDEVICE9 pGraphicDev);
private:
	virtual void Free(void);
};


#endif
