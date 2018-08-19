#ifndef Sword_h__
#define Sword_h__

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

class CSword : public Engine::CGameObject
{
private:
	explicit CSword(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CSword(void);
public:
	virtual HRESULT Ready_GameObject(void);
	_int Update_GameObject(const _float& fTimeDelta);
	void Render_GameObject(void);
	_vec3 GetMax(void){return m_vMax;}
	_vec3 GetMin(void){return m_vMin;}
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
	bool						m_bIsCol;
	_vec3						m_vMax;
	_vec3						m_vMin;
private:
	HRESULT Ready_Component(void);
public:
	static CSword* Create(LPDIRECT3DDEVICE9 pGraphicDev);
private:
	virtual void Free(void);
};


#endif // Terrain_h__
