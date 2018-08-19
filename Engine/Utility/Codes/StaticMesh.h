#ifndef StaticMesh_h__
#define StaticMesh_h__

#include "Mesh.h"

BEGIN(Engine)

class ENGINE_DLL CStaticMesh : public CMesh
{
private:
	explicit CStaticMesh(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CStaticMesh(const CStaticMesh& rhs);
	virtual ~CStaticMesh(void);	
public:
	HRESULT Ready_Meshes(const _tchar* pFilePath, const _tchar* pFileName);
	void Render_Mesh();	
public:
	void Compute_MinMax(_vec3* pMin, _vec3* pMax);
private:
	LPD3DXMESH					m_pOriMesh;	
	LPD3DXMESH					m_pMesh;
	LPD3DXBUFFER				m_pAdjacency;
	LPD3DXBUFFER				m_pSubSetBuffer;
	D3DXMATERIAL*				m_pSubSets;
	_ulong						m_dwSubSetCnt;	
	LPDIRECT3DTEXTURE9*			m_ppTextures;
private:
	_bool Find_Alpha(const char* pFileName);
public:
	static CStaticMesh* Create(LPDIRECT3DDEVICE9 pGraphicDev, const _tchar* pFilePath, const _tchar* pFileName);

	
public:
	virtual CComponent* Clone(void);
public:
	virtual void Free(void);

};

END

#endif // StaticMesh_h__
