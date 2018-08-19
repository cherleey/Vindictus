#ifndef Ingkells_Sword_h__
#define Ingkells_Sword_h__

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

class CIngkells_Sword : public Engine::CGameObject
{
private:
	explicit CIngkells_Sword(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CIngkells_Sword(void);
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
	_matrix*					m_pParentBoneMatrix; // ���Ⱑ �پ���� ���� ���
	_matrix*					m_pParentWorldMatrix;
	_matrix						m_matParent;
	bool						m_bIsCol;
private:
	HRESULT Ready_Component(void);
public:
	static CIngkells_Sword* Create(LPDIRECT3DDEVICE9 pGraphicDev);
private:
	virtual void Free(void);
};


#endif // Terrain_h__
