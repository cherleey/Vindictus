#ifndef Shield_h__
#define Shield_h__

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

class CShield : public Engine::CGameObject
{
private:
	explicit CShield(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CShield(void);
public:
	virtual HRESULT Ready_GameObject(void);
	_int Update_GameObject(const _float& fTimeDelta);
	void Render_GameObject(void);
private:
	Engine::CTransform*			m_pTransformCom;
	Engine::CRenderer*			m_pRendererCom;	
	Engine::CStaticMesh*		m_pMeshCom;
	Engine::CBoxCollider*		m_pColliderCom;
	Engine::CMaterial*			m_pMtrlCom;
private:
	_matrix*					m_pParentBoneMatrix; // 무기가 붙어야할 뼈의 행렬
	_matrix*					m_pParentWorldMatrix;
	_matrix						m_matParent;
private:
	HRESULT Ready_Component(void);
public:
	static CShield* Create(LPDIRECT3DDEVICE9 pGraphicDev);
private:
	virtual void Free(void);
};


#endif // Terrain_h__
