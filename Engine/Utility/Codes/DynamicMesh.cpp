#include "DynamicMesh.h"
#include "HierarchyLoader.h"
#include "AnimationCtrl.h"

USING(Engine)

Engine::CDynamicMesh::CDynamicMesh(LPDIRECT3DDEVICE9 pGraphicDev)
: CMesh(pGraphicDev)
, m_pLoader(NULL)
, m_pRootFrame(NULL)
, m_pAnimationCtrl(NULL)
{

}

Engine::CDynamicMesh::CDynamicMesh(const CDynamicMesh& rhs)
: CMesh(rhs)
, m_pLoader(rhs.m_pLoader)
, m_pRootFrame(rhs.m_pRootFrame)
, m_MeshContainerList(rhs.m_MeshContainerList)
, m_pAnimationCtrl(CAnimationCtrl::Create(*rhs.m_pAnimationCtrl))
, m_matParent(rhs.m_matParent)
{
	m_pLoader->AddRef();
}

Engine::CDynamicMesh::~CDynamicMesh(void)
{

}

_matrix* Engine::CDynamicMesh::Get_FrameMatrix(const char* pFrameName) const
{
	D3DXFRAME_DERIVED* pFindFrame = (D3DXFRAME_DERIVED*)D3DXFrameFind(m_pRootFrame, pFrameName);

	if(NULL == pFindFrame)
		return NULL;

	return &pFindFrame->CombinedTransformationMatrix;
}

HRESULT Engine::CDynamicMesh::Ready_Meshes(const _tchar* pFilePath, const _tchar* pFileName)
{
	_tchar			szFullPath[128] = L"";

	lstrcpy(szFullPath, pFilePath);
	lstrcat(szFullPath, pFileName);

	m_pLoader = CHierarchyLoader::Create(m_pGraphicDev, pFilePath);
	if(NULL == m_pLoader)
		return E_FAIL;		

	m_pAnimationCtrl = CAnimationCtrl::Create(m_pGraphicDev);
	if(NULL == m_pAnimationCtrl)
		return E_FAIL;	

	

	if(FAILED(D3DXLoadMeshHierarchyFromX(szFullPath, D3DXMESH_MANAGED, m_pGraphicDev, m_pLoader, NULL, &m_pRootFrame, m_pAnimationCtrl->Get_AnimationCtrl())))
		return E_FAIL;


	D3DXMatrixRotationY(&m_matParent, D3DXToRadian(180.0f));

	Update_CombinedMatrices((D3DXFRAME_DERIVED*)m_pRootFrame, &m_matParent);

	SetUp_MatrixPointer((D3DXFRAME_DERIVED*)m_pRootFrame);

	return S_OK;
}


// 뼈들이 가지고 있는 CombinedTransformationMatrix에 값을 채우자.
void Engine::CDynamicMesh::Update_CombinedMatrices(D3DXFRAME_DERIVED* pFrame, const _matrix* pMatrix)
{
	pFrame->CombinedTransformationMatrix = pFrame->TransformationMatrix * *pMatrix;

	if(NULL != pFrame->pFrameSibling)
		Update_CombinedMatrices((D3DXFRAME_DERIVED*)pFrame->pFrameSibling, pMatrix);

	if(NULL != pFrame->pFrameFirstChild)
		Update_CombinedMatrices((D3DXFRAME_DERIVED*)pFrame->pFrameFirstChild, &pFrame->CombinedTransformationMatrix);		
}

void Engine::CDynamicMesh::SetUp_MatrixPointer(D3DXFRAME_DERIVED* pFrame)
{
	if(NULL != pFrame->pMeshContainer)
	{
		D3DXMESHCONTAINER_DERIVED*		pMeshContainer = (D3DXMESHCONTAINER_DERIVED*)pFrame->pMeshContainer;

		for (_ulong i = 0; i < pMeshContainer->dwNumBone; ++i)
		{
			const char* pFrameName = pMeshContainer->pSkinInfo->GetBoneName(i);

			D3DXFRAME_DERIVED* pFrameFind = (D3DXFRAME_DERIVED*)D3DXFrameFind(m_pRootFrame, pFrameName);

			pMeshContainer->ppCombinedTransformationMatrices[i] = &pFrameFind->CombinedTransformationMatrix;		
		}

		m_MeshContainerList.push_back(pMeshContainer);
	}	

	if(NULL != pFrame->pFrameSibling)
		SetUp_MatrixPointer((D3DXFRAME_DERIVED*)pFrame->pFrameSibling);

	if(NULL != pFrame->pFrameFirstChild)
		SetUp_MatrixPointer((D3DXFRAME_DERIVED*)pFrame->pFrameFirstChild);
}

void Engine::CDynamicMesh::Render_Mesh(void)
{
	MESHCONTAINERLIST::iterator	iter = m_MeshContainerList.begin();
	MESHCONTAINERLIST::iterator	iter_end = m_MeshContainerList.end();	

	for (; iter != iter_end; ++iter)
	{
		for (_ulong i = 0; i < (*iter)->dwNumBone; ++i)
			(*iter)->pRenderingMatrices[i] = (*iter)->pOffsetMatrices[i] * *(*iter)->ppCombinedTransformationMatrices[i];

		void*		pSrcVertices = NULL, *pDestVertices = NULL;

		(*iter)->pOriMesh->LockVertexBuffer(0, &pSrcVertices);
		(*iter)->MeshData.pMesh->LockVertexBuffer(0, &pDestVertices);

		(*iter)->pSkinInfo->UpdateSkinnedMesh((*iter)->pRenderingMatrices, NULL, pSrcVertices, pDestVertices);

		(*iter)->pOriMesh->UnlockVertexBuffer();
		(*iter)->MeshData.pMesh->UnlockVertexBuffer();


		for (_ulong i = 0; i < (*iter)->NumMaterials; ++i)
		{
			m_pGraphicDev->SetTexture(0, (*iter)->ppTextures[i]);
			(*iter)->MeshData.pMesh->DrawSubset(i);
		}	
	}	
}

void Engine::CDynamicMesh::Render_Mesh(LPD3DXEFFECT pEffect)
{
	MESHCONTAINERLIST::iterator	iter = m_MeshContainerList.begin();
	MESHCONTAINERLIST::iterator	iter_end = m_MeshContainerList.end();	

	for (; iter != iter_end; ++iter)
	{
		for (_ulong i = 0; i < (*iter)->dwNumBone; ++i)
			(*iter)->pRenderingMatrices[i] = (*iter)->pOffsetMatrices[i] * *(*iter)->ppCombinedTransformationMatrices[i];

		void*		pSrcVertices = NULL, *pDestVertices = NULL;

		(*iter)->pOriMesh->LockVertexBuffer(0, &pSrcVertices);
		(*iter)->MeshData.pMesh->LockVertexBuffer(0, &pDestVertices);

		(*iter)->pSkinInfo->UpdateSkinnedMesh((*iter)->pRenderingMatrices, NULL, pSrcVertices, pDestVertices);

		(*iter)->pOriMesh->UnlockVertexBuffer();
		(*iter)->MeshData.pMesh->UnlockVertexBuffer();


		for (_ulong i = 0; i < (*iter)->NumMaterials; ++i)
		{
			pEffect->SetTexture("g_BaseTexture", (*iter)->ppTextures[i]);

			pEffect->CommitChanges();

			pEffect->BeginPass(0);

			(*iter)->MeshData.pMesh->DrawSubset(i);

			pEffect->EndPass();
		}
	}	
}

void Engine::CDynamicMesh::Set_AnimationSet(const _uint& iIndex, const _float& fSpeed)
{
	m_pAnimationCtrl->Set_AnimationSet(iIndex, fSpeed);
}

void Engine::CDynamicMesh::Play_AnimationSet(const _float& fTimeDelta)
{
	m_pAnimationCtrl->Play_AnimationSet(fTimeDelta);

	Update_CombinedMatrices((D3DXFRAME_DERIVED*)m_pRootFrame, &m_matParent);	
}


void Engine::CDynamicMesh::Compute_MinMax(_vec3* pMin, _vec3* pMax, const _uint& iContainerIdx)
{
	D3DVERTEXELEMENT9			Element[MAX_FVF_DECL_SIZE];
	ZeroMemory(Element, sizeof(D3DVERTEXELEMENT9) * MAX_FVF_DECL_SIZE);

	MESHCONTAINERLIST::iterator	iter = m_MeshContainerList.begin();

	for (_uint i = 0; i < iContainerIdx; ++i)
		++iter;

	(*iter)->MeshData.pMesh->GetDeclaration(Element);
	
	

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

	(*iter)->MeshData.pMesh->LockVertexBuffer(0, (void**)&pVertex);

	_ulong dwFVF = (*iter)->MeshData.pMesh->GetFVF();

	D3DXComputeBoundingBox((_vec3*)(pVertex + byOffset), (*iter)->MeshData.pMesh->GetNumVertices(), D3DXGetFVFVertexSize(dwFVF), pMin, pMax);

	(*iter)->MeshData.pMesh->UnlockVertexBuffer();	
}

CComponent* Engine::CDynamicMesh::Clone(void)
{
	return new CDynamicMesh(*this);
}

CDynamicMesh* Engine::CDynamicMesh::Create(LPDIRECT3DDEVICE9 pGraphicDev, const _tchar* pFilePath, const _tchar* pFileName)
{
	CDynamicMesh*		pInstance = new CDynamicMesh(pGraphicDev);

	if(FAILED(pInstance->Ready_Meshes(pFilePath, pFileName)))
	{
		MSG_BOX("CDynamicMesh Created Failed");
		Engine::Safe_Release(pInstance);
	}
	return pInstance;
}

void Engine::CDynamicMesh::Free(void)
{
	Engine::CMesh::Free();

	if(false == m_isClone)// 원형객체의 경우에만.
	{
		m_pLoader->DestroyFrame(m_pRootFrame);
		
	}

	Engine::Safe_Release(m_pAnimationCtrl);

	m_MeshContainerList.clear();

	
	Engine::Safe_Release(m_pLoader);	
}

