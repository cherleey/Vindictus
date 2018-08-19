#ifndef Stone_h__
#define Stone_h__

#include "Defines.h"
#include "GameObject.h"

namespace Engine
{
	class CRenderer;
	class CTransform;	
	class CStaticMesh;
	class CBoxCollider;
	class CMaterial;
}

class CStone : public Engine::CGameObject
{
private:
	explicit CStone(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CStone(void);
public:
	virtual HRESULT Ready_GameObject(void);
	_int Update_GameObject(const _float& fTimeDelta);
	_int LastUpdate_GameObject(const _float& fTimeDelta);
	void Render_GameObject(void);
private:
	Engine::CTransform*			m_pTransformCom;
	Engine::CRenderer*			m_pRendererCom;	
	Engine::CStaticMesh*		m_pMeshCom;
	Engine::CBoxCollider*		m_pColliderCom;
	Engine::CMaterial*			m_pMtrlCom;
private:
	_bool						m_isColl;
private:
	HRESULT Ready_Component(void);
public:
	static CStone* Create(LPDIRECT3DDEVICE9 pGraphicDev);
private:
	virtual void Free(void);
};


#endif // Terrain_h__
