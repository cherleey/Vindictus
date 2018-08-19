#include "BoxCollider.h"

USING(Engine)

Engine::CBoxCollider::CBoxCollider(LPDIRECT3DDEVICE9 pGraphicDev)
: m_pGraphicDev(pGraphicDev)
, m_pVB(NULL)
, m_pIB(NULL)
, m_bIsCol(false)
{
	m_pGraphicDev->AddRef();
}

CBoxCollider::CBoxCollider(const CBoxCollider& rhs)
: m_pGraphicDev(rhs.m_pGraphicDev)
, m_pVB(rhs.m_pVB)
, m_pIB(rhs.m_pIB)
, m_ColliderMatrix(rhs.m_ColliderMatrix)
, m_bIsCol(false)
{
	m_pGraphicDev->AddRef();
	m_pVB->AddRef();
	m_pIB->AddRef();

	for (_uint i = 0; i < COLL_END; ++i)
	{
		m_pTexture[i] = rhs.m_pTexture[i];
		m_pTexture[i]->AddRef();
	}
}

Engine::CBoxCollider::~CBoxCollider(void)
{

}

HRESULT Engine::CBoxCollider::Ready_Component(void)
{
	D3DXMatrixIdentity(&m_ColliderMatrix);

	for (_uint i = 0; i < COLL_END; ++i)	
		D3DXCreateTexture(m_pGraphicDev, 1, 1, 1, 0, D3DFMT_A8R8G8B8, D3DPOOL_MANAGED, &m_pTexture[i]);

	D3DLOCKED_RECT			LockRect;
	ZeroMemory(&LockRect, sizeof(D3DLOCKED_RECT));

	m_pTexture[COLL_NO]->LockRect(0, &LockRect, NULL, 0);

	((_ulong*)(LockRect.pBits))[0] = D3DXCOLOR(0.f, 1.f, 0.f, 1.f);

	m_pTexture[COLL_NO]->UnlockRect(0);

	ZeroMemory(&LockRect, sizeof(D3DLOCKED_RECT));

	m_pTexture[COLL_YES]->LockRect(0, &LockRect, NULL, 0);

	((_ulong*)(LockRect.pBits))[0] = D3DXCOLOR(1.f, 0.f, 0.f, 1.f);

	m_pTexture[COLL_YES]->UnlockRect(0);



	if(FAILED(m_pGraphicDev->CreateVertexBuffer(sizeof(VTXCUBETEX) * 8, 0, D3DFVF_VTXCUBETEX, D3DPOOL_MANAGED, &m_pVB, NULL)))
		return E_FAIL;

	// 할당된 메모리에 접근을 하자.
	VTXCUBETEX*				pVertex = NULL;

	m_pVB->Lock(0, 0, (void**)&pVertex, 0);

	pVertex[0].vPosition = _vec3(-0.5f, 0.5f, -0.5f);
	pVertex[0].vTexUV = pVertex[0].vPosition;

	pVertex[1].vPosition = _vec3(0.5f, 0.5f, -0.5f);
	pVertex[1].vTexUV = pVertex[1].vPosition;

	pVertex[2].vPosition = _vec3(0.5f, -0.5f, -0.5f);
	pVertex[2].vTexUV = pVertex[2].vPosition;

	pVertex[3].vPosition = _vec3(-0.5f, -0.5f, -0.5f);
	pVertex[3].vTexUV = pVertex[3].vPosition;

	pVertex[4].vPosition = _vec3(-0.5f, 0.5f, 0.5f);
	pVertex[4].vTexUV = pVertex[4].vPosition;

	pVertex[5].vPosition = _vec3(0.5f, 0.5f, 0.5f);
	pVertex[5].vTexUV = pVertex[5].vPosition;

	pVertex[6].vPosition = _vec3(0.5f, -0.5f, 0.5f);
	pVertex[6].vTexUV = pVertex[6].vPosition;

	pVertex[7].vPosition = _vec3(-0.5f, -0.5f, 0.5f);
	pVertex[7].vTexUV = pVertex[7].vPosition;
	m_pVB->Unlock();

	if(FAILED(m_pGraphicDev->CreateIndexBuffer(sizeof(INDEX16) * 12, 0, D3DFMT_INDEX16, D3DPOOL_MANAGED, &m_pIB, NULL)))
		return E_FAIL;

	INDEX16*			pIndex = NULL;

	m_pIB->Lock(0, 0, (void**)&pIndex, 0);

	// +x
	pIndex[0]._0 = 1; pIndex[0]._1 = 6; pIndex[0]._2 = 2;
	pIndex[1]._0 = 1; pIndex[1]._1 = 5; pIndex[1]._2 = 6;

	// -x									   
	pIndex[2]._0 = 4; pIndex[2]._1 = 0; pIndex[2]._2 = 3;
	pIndex[3]._0 = 4; pIndex[3]._1 = 3; pIndex[3]._2 = 7;

	// +y									   
	pIndex[4]._0 = 4; pIndex[4]._1 = 5; pIndex[4]._2 = 1;
	pIndex[5]._0 = 4; pIndex[5]._1 = 1; pIndex[5]._2 = 0;

	// -y									   
	pIndex[6]._0 = 3; pIndex[6]._1 = 2; pIndex[6]._2 = 6;
	pIndex[7]._0 = 3; pIndex[7]._1 = 6; pIndex[7]._2 = 7;

	// +z									   
	pIndex[8]._0 = 7; pIndex[8]._1 = 6; pIndex[8]._2 = 5;
	pIndex[9]._0 = 7; pIndex[9]._1 = 5; pIndex[9]._2 = 4;

	// -z									   
	pIndex[10]._0 = 0; pIndex[10]._1 = 1; pIndex[10]._2 = 2;
	pIndex[11]._0 = 0; pIndex[11]._1 = 2; pIndex[11]._2 = 3;

	m_pIB->Unlock();

	return S_OK;
}

HRESULT Engine::CBoxCollider::Compute_Collider(D3DXVECTOR3 vScale, D3DXVECTOR3 vTrans)
{
	m_vMax.x = vTrans.x + vScale.x * 0.5f;
	m_vMax.y = vTrans.y + vScale.y * 0.5f;
	m_vMax.z = vTrans.z + vScale.z * 0.5f;

	m_vMin.x = vTrans.x - vScale.x * 0.5f;
	m_vMin.y = vTrans.y - vScale.y * 0.5f;
	m_vMin.z = vTrans.z - vScale.z * 0.5f;

	_matrix			matScale, matTrans;

	D3DXMatrixScaling(&matScale, vScale.x, vScale.y, vScale.z);

	D3DXMatrixTranslation(&matTrans, vTrans.x, vTrans.y, vTrans.z);
	
	m_LocalMatrix = matScale * matTrans;

	return S_OK;
}

HRESULT Engine::CBoxCollider::Compute_Collider(Engine::CStaticMesh* pMesh, const _float& fScaleX, const _float& fScaleY, const _float& fScaleZ)
{
	pMesh->Compute_MinMax(&m_vMin, &m_vMax);

	m_vMax.x *= fScaleX;
	m_vMax.y *= fScaleY;
	m_vMax.z *= fScaleZ;

	m_vMin.x *= fScaleX;
	m_vMin.y *= fScaleY;
	m_vMin.z *= fScaleZ;

	_matrix			matScale, matTrans;

	D3DXMatrixScaling(&matScale, m_vMax.x - m_vMin.x, m_vMax.y - m_vMin.y, m_vMax.z - m_vMin.z);

	D3DXMatrixTranslation(&matTrans, (m_vMax.x + m_vMin.x) * 0.5f, (m_vMax.y + m_vMin.y) * 0.5f, (m_vMax.z + m_vMin.z) * 0.5f);

	m_LocalMatrix = matScale * matTrans;

	return S_OK;
}

HRESULT Engine::CBoxCollider::Compute_Collider(Engine::CDynamicMesh* pMesh, const _uint& iMeshContainerIdx)
{
	pMesh->Compute_MinMax(&m_vMin, &m_vMax, iMeshContainerIdx);

	_matrix			matScale, matTrans;

	D3DXMatrixScaling(&matScale, m_vMax.x - m_vMin.x, m_vMax.y - m_vMin.y, m_vMax.z - m_vMin.z);

	D3DXMatrixTranslation(&matTrans, (m_vMax.x + m_vMin.x) * 0.5f, (m_vMax.y + m_vMin.y) * 0.5f, (m_vMax.z + m_vMin.z) * 0.5f);

	m_LocalMatrix = matScale * matTrans;


	return S_OK;
}

HRESULT Engine::CBoxCollider::Compute_Collider(Engine::CDynamicMesh* pMesh, const _uint& iMeshContainerIdx, const _float& fScaleX, const _float& fScaleY, const _float& fScaleZ)
{
	pMesh->Compute_MinMax(&m_vMin, &m_vMax, iMeshContainerIdx);

	m_vMax.x *= fScaleX;
	m_vMax.y *= fScaleY;
	m_vMax.z *= fScaleZ;

	m_vMin.x *= fScaleX;
	m_vMin.y *= fScaleY;
	m_vMin.z *= fScaleZ;

	_matrix			matScale, matTrans;

	D3DXMatrixScaling(&matScale, m_vMax.x - m_vMin.x, m_vMax.y - m_vMin.y, m_vMax.z - m_vMin.z);

	D3DXMatrixTranslation(&matTrans, (m_vMax.x + m_vMin.x) * 0.5f, (m_vMax.y + m_vMin.y) * 0.5f, (m_vMax.z + m_vMin.z) * 0.5f);

	m_LocalMatrix = matScale * matTrans;


	return S_OK;
}

HRESULT Engine::CBoxCollider::Compute_Collider(_matrix* matFrame, _float fSizeX, _float fSizeY)
{
	m_vMin = _vec3(matFrame->_41 - fSizeX * 0.5f, matFrame->_42 - fSizeY * 0.5f, matFrame->_43 - fSizeX * 0.5f);
	m_vMax = _vec3(matFrame->_41 + fSizeX * 0.5f, matFrame->_42 + fSizeY * 0.5f, matFrame->_43 + fSizeX * 0.5f);

	_matrix			matScale, matTrans;

	D3DXMatrixScaling(&matScale, m_vMax.x - m_vMin.x, m_vMax.y - m_vMin.y, m_vMax.z - m_vMin.z);

	D3DXMatrixTranslation(&matTrans, (m_vMax.x + m_vMin.x) * 0.5f, (m_vMax.y + m_vMin.y) * 0.5f, (m_vMax.z + m_vMin.z) * 0.5f);

	m_LocalMatrix = matScale * matTrans;


	return S_OK;
}

void Engine::CBoxCollider::Render_Collider(COLLTYPE eType, const _matrix* pWorldMatrix)
{
	if(TYPE_AABB == eType)
	{
		_matrix			matScale, matTrans;

		D3DXMatrixScaling(&matScale, D3DXVec3Length((_vec3*)&pWorldMatrix->m[0][0])
			, D3DXVec3Length((_vec3*)&pWorldMatrix->m[1][0])
			, D3DXVec3Length((_vec3*)&pWorldMatrix->m[2][0]));

		D3DXMatrixTranslation(&matTrans, pWorldMatrix->m[3][0], pWorldMatrix->m[3][1], pWorldMatrix->m[3][2]);

		m_ColliderMatrix = matScale * matTrans;
	}
	else
		m_ColliderMatrix = *pWorldMatrix;
	

	m_pGraphicDev->SetTransform(D3DTS_WORLD, &(m_LocalMatrix * m_ColliderMatrix));

	if(false == m_bIsCol)
		m_pGraphicDev->SetTexture(0, m_pTexture[COLL_NO]);
	else
		m_pGraphicDev->SetTexture(0, m_pTexture[COLL_YES]);

	//m_pGraphicDev->SetTexture(0, NULL);
	m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, FALSE);
	m_pGraphicDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);

	m_pGraphicDev->SetStreamSource(0, m_pVB, 0, sizeof(VTXCUBETEX));
	m_pGraphicDev->SetFVF(D3DFVF_VTXCUBETEX);
	m_pGraphicDev->SetIndices(m_pIB);
	m_pGraphicDev->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 8, 0, 12);

	m_pGraphicDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
	m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, TRUE);

	m_bIsCol = false;
}

_bool Engine::CBoxCollider::Collision_AABB(const CBoxCollider* pTargetCollider)
{
	_vec3			vWorldMin, vWorldMax;
	_vec3			vTarget_WorldMin, vTarget_WorldMax;

	D3DXVec3TransformCoord(&vWorldMin, &m_vMin, &m_ColliderMatrix);
	D3DXVec3TransformCoord(&vWorldMax, &m_vMax, &m_ColliderMatrix);

	D3DXVec3TransformCoord(&vTarget_WorldMin, &pTargetCollider->m_vMin, &pTargetCollider->m_ColliderMatrix);
	D3DXVec3TransformCoord(&vTarget_WorldMax, &pTargetCollider->m_vMax, &pTargetCollider->m_ColliderMatrix);

	// 가로로 겹쳤냐
	if(max(vWorldMin.x, vTarget_WorldMin.x) > min(vWorldMax.x, vTarget_WorldMax.x))
		return false;

	// 세로로 겹쳤냐
	if(max(vWorldMin.y, vTarget_WorldMin.y) > min(vWorldMax.y, vTarget_WorldMax.y))
		return false;

	// 깊이로 겹쳤냐
	if(max(vWorldMin.z, vTarget_WorldMin.z) > min(vWorldMax.z, vTarget_WorldMax.z))
		return false;


	return true;	
}

_bool Engine::CBoxCollider::Collision_OBB(CBoxCollider* pTargetCollider, COLORCHANGE eColorChange)
{
	Set_PointInfo();
	pTargetCollider->Set_PointInfo();

	// 로컬상의 정점들의 위치를 그려지는 행렬과 곱해서 변환시키자.
	for (_uint i = 0; i < 8; ++i)
	{
		D3DXVec3TransformCoord(&m_tOBB.vPoint[i], &m_tOBB.vPoint[i], &m_ColliderMatrix);
		D3DXVec3TransformCoord(&pTargetCollider->m_tOBB.vPoint[i], &pTargetCollider->m_tOBB.vPoint[i], &pTargetCollider->m_ColliderMatrix);
	}
	D3DXVec3TransformCoord(&m_tOBB.vCenter, &m_tOBB.vCenter, &m_ColliderMatrix);
	D3DXVec3TransformCoord(&pTargetCollider->m_tOBB.vCenter, &pTargetCollider->m_tOBB.vCenter, &pTargetCollider->m_ColliderMatrix);

	Compute_Axis();
	pTargetCollider->Compute_Axis();	

	


	for (_uint i = 0; i < 3; ++i)
	{
		_float		fDistance[3] = {0.0f};

		fDistance[0] = fabs(D3DXVec3Dot(&m_tOBB.vProjAxis[0], &m_tOBB.vAlignAxis[i])) 
			+ fabs(D3DXVec3Dot(&m_tOBB.vProjAxis[1], &m_tOBB.vAlignAxis[i]))
			+ fabs(D3DXVec3Dot(&m_tOBB.vProjAxis[2], &m_tOBB.vAlignAxis[i]));

		fDistance[1] = fabs(D3DXVec3Dot(&pTargetCollider->m_tOBB.vProjAxis[0], &m_tOBB.vAlignAxis[i])) 
			+ fabs(D3DXVec3Dot(&pTargetCollider->m_tOBB.vProjAxis[1], &m_tOBB.vAlignAxis[i]))
			+ fabs(D3DXVec3Dot(&pTargetCollider->m_tOBB.vProjAxis[2], &m_tOBB.vAlignAxis[i]));

		_vec3		vDistance;
		vDistance = pTargetCollider->m_tOBB.vCenter - m_tOBB.vCenter;

		fDistance[2] = fabs(D3DXVec3Dot(&vDistance, &m_tOBB.vAlignAxis[i]));

		if(fDistance[0] + fDistance[1] < fDistance[2])
		{
			m_bIsCol = false;
			return false;
		}
	}

	for (_uint i = 0; i < 3; ++i)
	{
		_float		fDistance[3] = {0.0f};

		fDistance[0] = fabs(D3DXVec3Dot(&m_tOBB.vProjAxis[0], &pTargetCollider->m_tOBB.vAlignAxis[i])) 
			+ fabs(D3DXVec3Dot(&m_tOBB.vProjAxis[1], &pTargetCollider->m_tOBB.vAlignAxis[i]))
			+ fabs(D3DXVec3Dot(&m_tOBB.vProjAxis[2], &pTargetCollider->m_tOBB.vAlignAxis[i]));

		fDistance[1] = fabs(D3DXVec3Dot(&pTargetCollider->m_tOBB.vProjAxis[0], &pTargetCollider->m_tOBB.vAlignAxis[i])) 
			+ fabs(D3DXVec3Dot(&pTargetCollider->m_tOBB.vProjAxis[1], &pTargetCollider->m_tOBB.vAlignAxis[i]))
			+ fabs(D3DXVec3Dot(&pTargetCollider->m_tOBB.vProjAxis[2], &pTargetCollider->m_tOBB.vAlignAxis[i]));

		_vec3		vDistance;
		vDistance = pTargetCollider->m_tOBB.vCenter - m_tOBB.vCenter;

		fDistance[2] = fabs(D3DXVec3Dot(&vDistance, &pTargetCollider->m_tOBB.vAlignAxis[i]));

			if(fDistance[0] + fDistance[1] < fDistance[2])
			{
				m_bIsCol = false;
				return false;
			}
	}

	if(CHANGE_YES == eColorChange)
		m_bIsCol = true;
	else
		m_bIsCol = false;

	return true;
}

void Engine::CBoxCollider::Compute_Axis(void)
{
	m_tOBB.vAlignAxis[0] = m_tOBB.vPoint[5] - m_tOBB.vPoint[4];
	m_tOBB.vAlignAxis[1] = m_tOBB.vPoint[5] - m_tOBB.vPoint[6];
	m_tOBB.vAlignAxis[2] = m_tOBB.vPoint[5] - m_tOBB.vPoint[1];

	for (_uint i = 0; i < 3; ++i)	
		D3DXVec3Normalize(&m_tOBB.vAlignAxis[i], &m_tOBB.vAlignAxis[i]);	

	m_tOBB.vProjAxis[0] = (m_tOBB.vPoint[5] + m_tOBB.vPoint[2]) * 0.5f - m_tOBB.vCenter;
	m_tOBB.vProjAxis[1] = (m_tOBB.vPoint[5] + m_tOBB.vPoint[0]) * 0.5f - m_tOBB.vCenter;
	m_tOBB.vProjAxis[2] = (m_tOBB.vPoint[5] + m_tOBB.vPoint[7]) * 0.5f - m_tOBB.vCenter;
}

void Engine::CBoxCollider::Set_PointInfo(void)
{
	m_tOBB.vPoint[0] = _vec3(m_vMin.x, m_vMax.y, m_vMin.z);
	m_tOBB.vPoint[1] = _vec3(m_vMax.x, m_vMax.y, m_vMin.z);
	m_tOBB.vPoint[2] = _vec3(m_vMax.x, m_vMin.y, m_vMin.z);
	m_tOBB.vPoint[3] = _vec3(m_vMin.x, m_vMin.y, m_vMin.z);

	m_tOBB.vPoint[4] = _vec3(m_vMin.x, m_vMax.y, m_vMax.z);
	m_tOBB.vPoint[5] = _vec3(m_vMax.x, m_vMax.y, m_vMax.z);
	m_tOBB.vPoint[6] = _vec3(m_vMax.x, m_vMin.y, m_vMax.z);
	m_tOBB.vPoint[7] = _vec3(m_vMin.x, m_vMin.y, m_vMax.z);

	m_tOBB.vCenter = (m_vMin + m_vMax) * 0.5f;
}

CComponent* Engine::CBoxCollider::Clone(void)
{
	return new CBoxCollider(*this);
}

CBoxCollider* Engine::CBoxCollider::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CBoxCollider*	pInstance = new CBoxCollider(pGraphicDev);

	if(FAILED(pInstance->Ready_Component()))
	{
		MSG_BOX("CBoxCollider Created Failed");
		Engine::Safe_Release(pInstance);
	}
	return pInstance;
}

void Engine::CBoxCollider::Free(void)
{
	for (_uint i = 0; i < COLL_END; ++i)		
		Engine::Safe_Release(m_pTexture[i]);	

	Engine::Safe_Release(m_pGraphicDev);
	Engine::Safe_Release(m_pVB);
	Engine::Safe_Release(m_pIB);
}
