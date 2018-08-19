#include "StaticMesh.h"

USING(Engine)

Engine::CStaticMesh::CStaticMesh(LPDIRECT3DDEVICE9 pGraphicDev)
: Engine::CMesh(pGraphicDev)
{

}

Engine::CStaticMesh::CStaticMesh(const CStaticMesh& rhs)
: Engine::CMesh(rhs)
, m_pAdjacency(rhs.m_pAdjacency)
, m_pSubSetBuffer(rhs.m_pSubSetBuffer)
, m_pSubSets(rhs.m_pSubSets)
, m_dwSubSetCnt(rhs.m_dwSubSetCnt)
, m_pMesh(rhs.m_pMesh)
, m_pOriMesh(rhs.m_pOriMesh)
, m_ppTextures(rhs.m_ppTextures)
{
	/*m_ppTextures = new LPDIRECT3DTEXTURE9[rhs.m_dwSubSetCnt];*/

	for (_ulong i = 0; i < rhs.m_dwSubSetCnt; ++i)
	{
		m_ppTextures[i] = rhs.m_ppTextures[i];
		m_ppTextures[i]->AddRef();
	}
	m_pAdjacency->AddRef();
	m_pSubSetBuffer->AddRef();
	m_pOriMesh->AddRef();
	m_pMesh->AddRef();

}

Engine::CStaticMesh::~CStaticMesh(void)
{

}

HRESULT Engine::CStaticMesh::Ready_Meshes(const _tchar* pFilePath, const _tchar* pFileName)
{
	_tchar			szFullPath[128] = L"";

	lstrcpy(szFullPath, pFilePath);
	lstrcat(szFullPath, pFileName);

	_ulong	dwSubSetCnt = 0;

	if(FAILED(D3DXLoadMeshFromX(szFullPath, D3DXMESH_MANAGED, m_pGraphicDev, &m_pAdjacency, &m_pSubSetBuffer, NULL, &dwSubSetCnt, &m_pOriMesh)))
		return E_FAIL;	

	// �޽ø� �����ϰ� �ִ� ������ fvf������ �����Ѵ�.
	_ulong dwFVF = m_pOriMesh->GetFVF();

	if(dwFVF & D3DFVF_NORMAL)
	{
		m_pOriMesh->CloneMeshFVF(m_pOriMesh->GetOptions(), dwFVF, m_pGraphicDev, &m_pMesh);
	}
	else
	{
		// ���� ������ fvf������ �޽ø� �����ϰڴ�.
		m_pOriMesh->CloneMeshFVF(m_pOriMesh->GetOptions(), dwFVF | D3DFVF_NORMAL, m_pGraphicDev, &m_pMesh);

		// ����� ������ش�.
		D3DXComputeNormals(m_pMesh, (_ulong*)m_pAdjacency->GetBufferPointer());
	}

	// D3DXBuffer�� ����ִ� ������ ���� �� �ּҸ� �����ϳ�.
	m_pSubSets = (D3DXMATERIAL*)m_pSubSetBuffer->GetBufferPointer();

	m_dwSubSetCnt = dwSubSetCnt == 0 ? 1 : dwSubSetCnt;

	m_ppTextures = new LPDIRECT3DTEXTURE9[m_dwSubSetCnt];

	if(0 != dwSubSetCnt)
	{
		for (_ulong i = 0; i < m_dwSubSetCnt; ++i)
		{
			_tchar			szFileName[128] = L"";

			lstrcpy(szFullPath, pFilePath);

			MultiByteToWideChar(CP_ACP, 0, m_pSubSets[i].pTextureFilename, strlen(m_pSubSets[i].pTextureFilename)
				, szFileName, 128);

			lstrcat(szFullPath, szFileName);

			if(FAILED(D3DXCreateTextureFromFile(m_pGraphicDev, szFullPath, &m_ppTextures[i])))
				return E_FAIL;
		}
	}

	else
	{
		m_pSubSets[0].MatD3D.Diffuse = D3DXCOLOR(1.f, 0.f, 0.f, 1.f);
		m_pSubSets[0].MatD3D.Specular = D3DXCOLOR(1.f, 0.f, 0.f, 1.f);
		m_pSubSets[0].MatD3D.Emissive = D3DXCOLOR(1.f, 0.f, 0.f, 1.f);
		m_pSubSets[0].MatD3D.Ambient = D3DXCOLOR(1.f, 0.f, 0.f, 1.f);
		m_pSubSets[0].MatD3D.Power = 1.0f;

		m_ppTextures[0] = NULL;				
	}

	return S_OK;
}

void Engine::CStaticMesh::Render_Mesh()
{
	for (_ulong i = 0; i < m_dwSubSetCnt; ++i)
	{
		if(NULL != m_ppTextures[i])
			m_pGraphicDev->SetTexture(0, m_ppTextures[i]);
		m_pMesh->DrawSubset(i);
	}
	
}

void Engine::CStaticMesh::Compute_MinMax(_vec3* pMin, _vec3* pMax)
{
	D3DVERTEXELEMENT9			Element[MAX_FVF_DECL_SIZE];
	ZeroMemory(Element, sizeof(D3DVERTEXELEMENT9) * MAX_FVF_DECL_SIZE);

	m_pMesh->GetDeclaration(Element);
	
	_byte	byOffset = 0;

	for (_ulong i = 0; i < MAX_FVF_DECL_SIZE; ++i)
	{
		if(D3DDECLUSAGE_POSITION == Element[i].Usage)
		{
			byOffset = (_byte)Element[i].Offset;
			break;
		}
	}

	_byte*		pVertex = NULL;

	m_pMesh->LockVertexBuffer(0, (void**)&pVertex);

	_ulong dwFVF = m_pMesh->GetFVF();

	D3DXComputeBoundingBox((_vec3*)(pVertex + byOffset), m_pMesh->GetNumVertices(), D3DXGetFVFVertexSize(dwFVF), pMin, pMax);

	m_pMesh->UnlockVertexBuffer();	
}


_bool Engine::CStaticMesh::Find_Alpha(const char* pFileName)
{
	_uint iLength = strlen(pFileName);

	for (_uint i = 0; i < iLength + 1; ++i)
	{
		if(pFileName[i] == '.')
		{
			if(pFileName[i - 1] == 'A')
				return true;
		}
	}
	return false;
}

CStaticMesh* Engine::CStaticMesh::Create(LPDIRECT3DDEVICE9 pGraphicDev, const _tchar* pFilePath, const _tchar* pFileName)
{
	CStaticMesh*		pInstance = new CStaticMesh(pGraphicDev);

	if(FAILED(pInstance->Ready_Meshes(pFilePath, pFileName)))
	{
		MSG_BOX("CStaticMesh Created Failed");
		Engine::Safe_Release(pInstance);
	}
	return pInstance;
}

CComponent* Engine::CStaticMesh::Clone(void)
{
	return new CStaticMesh(*this);
}

void Engine::CStaticMesh::Free(void)
{	
	Engine::CMesh::Free();

	for (_ulong i = 0; i < m_dwSubSetCnt; ++i)
	{
		Engine::Safe_Release(m_ppTextures[i]);
	}

	_ulong dwRefCnt = 0;
	dwRefCnt = Safe_Release(m_pOriMesh);
	dwRefCnt = Safe_Release(m_pMesh);
	Safe_Release(m_pAdjacency);
	Safe_Release(m_pSubSetBuffer);

	if(false == m_isClone)	
		Safe_Delete_Array(m_ppTextures);
	
}

