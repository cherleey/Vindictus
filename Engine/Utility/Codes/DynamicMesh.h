#ifndef DynamicMesh_h__
#define DynamicMesh_h__

#include "Mesh.h"

BEGIN(Engine)

class CHierarchyLoader;
class CAnimationCtrl;
class ENGINE_DLL CDynamicMesh : public CMesh
{
private:
	explicit CDynamicMesh(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CDynamicMesh(const CDynamicMesh& rhs);
	virtual ~CDynamicMesh(void);
public:
	_matrix* Get_FrameMatrix(const char* pFrameName) const;
	CAnimationCtrl* Get_AnimationCtrl(void){
	return m_pAnimationCtrl;}
public:
	HRESULT Ready_Meshes(const _tchar* pFilePath, const _tchar* pFileName);
	void Update_CombinedMatrices(D3DXFRAME_DERIVED* pFrame, const _matrix* pMatrix);
	void SetUp_MatrixPointer(D3DXFRAME_DERIVED* pFrame);
	void Render_Mesh(void);
	void Render_Mesh(LPD3DXEFFECT pEffect);
	void Set_AnimationSet(const _uint& iIndex, const _float& fSpeed = 1.5f);
	void Play_AnimationSet(const _float& fTimeDelta);
public:
	void Compute_MinMax(_vec3* pMin, _vec3* pMax, const _uint& iContainerIdx);
private:
	CHierarchyLoader*							m_pLoader;
	CAnimationCtrl*								m_pAnimationCtrl;
	LPD3DXFRAME									m_pRootFrame;
	_matrix										m_matParent;
private:
	list<D3DXMESHCONTAINER_DERIVED*>			m_MeshContainerList;
	typedef list<D3DXMESHCONTAINER_DERIVED*>	MESHCONTAINERLIST;
public:
	virtual CComponent* Clone(void);
	static CDynamicMesh* Create(LPDIRECT3DDEVICE9 pGraphicDev, const _tchar* pFilePath, const _tchar* pFileName);
private:
	virtual void Free(void);

};

END

#endif // DynamicMesh_h__
