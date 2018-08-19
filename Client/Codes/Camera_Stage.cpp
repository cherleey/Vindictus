#include "stdafx.h"
#include "Camera_Stage.h"
#include "Export_Engine.h"

CCamera_Stage::CCamera_Stage(LPDIRECT3DDEVICE9 pGraphicDev)
: Engine::CCamera(pGraphicDev)
, m_pCamBoneMatrix(NULL)
, m_pEyeBoneMatrix(NULL)
, m_pParentWorldMatrix(NULL)
{

}

CCamera_Stage::~CCamera_Stage(void)
{

}

HRESULT CCamera_Stage::Ready_GameObject(const _vec3* pEye, const _vec3* pAt, const _vec3* pUp
									  , _float fFovy, _float fAspect, _float fNear, _float fFar)
{
	m_vEye = *pEye;
	m_vAt = *pAt;
	m_vUp = *pUp;

	m_fFovy = fFovy;
	m_fAspect = fAspect;
	m_fNear = fNear;
	m_fFar = fFar;

	if(FAILED(Engine::CCamera::Ready_GameObject()))
		return E_FAIL;

	return S_OK;
}

_int CCamera_Stage::Update_GameObject(const _float& fTimeDelta)
{
	POINT			ptCenter = {WINSIZEX >> 1, WINSIZEY >> 1};

	ClientToScreen(g_hWnd, &ptCenter);

	SetCursorPos(ptCenter.x, ptCenter.y);

	Check_InputDev(fTimeDelta);

	if(NULL == m_pCamBoneMatrix	|| NULL == m_pParentWorldMatrix || NULL == m_pEyeBoneMatrix)
	{
		Engine::CDynamicMesh*	pPlayerMeshCom = (Engine::CDynamicMesh*)Engine::Get_Component(L"Layer_Player", Engine::CGameObject::TYPE_STATIC, L"Com_Mesh");
		if(NULL == pPlayerMeshCom)
			return -1;
		m_pCamBoneMatrix = pPlayerMeshCom->Get_FrameMatrix("ValveBiped_Anim_Attachment_CAM");
		m_pEyeBoneMatrix = pPlayerMeshCom->Get_FrameMatrix("ValveBiped_Anim_Attachment_EyeTarget");

		Engine::CTransform*	pPlayerTransCom = (Engine::CTransform*)Engine::Get_Component(L"Layer_Player", Engine::CGameObject::TYPE_DYNAMIC, L"Com_Transform");
		if(NULL == pPlayerTransCom)
			return -1;
		m_pParentWorldMatrix = pPlayerTransCom->Get_WorldMatrix();
	}	

	_matrix			matWorld;
	D3DXMatrixInverse(&matWorld, NULL, &m_matView);

	m_matParent = (*m_pCamBoneMatrix * *m_pParentWorldMatrix);


	matWorld *= m_matParent;

	
	m_vUp = _vec3(matWorld._21, matWorld._22 + 1.f, matWorld._23);

	_vec3			vRight;
	memcpy(&vRight, &matWorld.m[0][0], sizeof(_vec3));

	_vec3			vLook = m_vEye - m_vAt;	

	m_vAt = _vec3(matWorld._41, matWorld._42, matWorld._43);

	D3DXVec3Normalize(&vLook, &vLook);

	m_vEye = m_vAt + vLook * 2;

	/*_vec3 vLook;
	_vec3 vRight;

	vRight = _vec3(matWorld._21, matWorld._22, matWorld._23);
	D3DXVec3Normalize(&vRight, &vRight);
	D3DXVec3Cross(&vLook, &m_vUp, &vRight);
	D3DXVec3Normalize(&vLook, &vLook);

	m_vEye = m_vAt - vLook;*/

	//_matrix			matWorld;
	//D3DXMatrixInverse(&matWorld, NULL, &m_matView);

	//m_matParent = (*m_pCamBoneMatrix * *m_pParentWorldMatrix);

	//matWorld *= m_matParent;

	//m_vAt = _vec3(matWorld._41, matWorld._42, matWorld._43);
	//m_vUp = _vec3(matWorld._21, matWorld._22 + 1.f, matWorld._23);

	//_vec3 vLook;
	//_vec3 vRight;

	//vRight = _vec3(matWorld._21, matWorld._22, matWorld._23);
	//D3DXVec3Normalize(&vRight, &vRight);
	//D3DXVec3Cross(&vLook, &m_vUp, &vRight);
	//D3DXVec3Normalize(&vLook, &vLook);

	//m_vEye = m_vAt - vLook;

	_int iExit = Engine::CCamera::Update_GameObject(fTimeDelta);

	return iExit;	
}

void CCamera_Stage::Check_InputDev(const _float& fTimeDelta)
{
	_matrix			matWorld;
	D3DXMatrixInverse(&matWorld, NULL, &m_matView);

	/*if(Engine::Get_DIKeyState(DIK_W) & 0x80)
	{
		_vec3			vLook;
		memcpy(&vLook, &matWorld.m[2][0], sizeof(_vec3));

		_vec3			vLength = *D3DXVec3Normalize(&vLook, &vLook) * 5.0f * fTimeDelta;

		m_vEye += vLength;
		m_vAt += vLength;
	}

	if(Engine::Get_DIKeyState(DIK_S) & 0x80)
	{
		_vec3			vLook;
		memcpy(&vLook, &matWorld.m[2][0], sizeof(_vec3));

		_vec3			vLength = *D3DXVec3Normalize(&vLook, &vLook) * 5.0f * fTimeDelta;

		m_vEye -= vLength;
		m_vAt -= vLength;
	}

	if(Engine::Get_DIKeyState(DIK_A) & 0x80)
	{
		_vec3			vLook;
		memcpy(&vLook, &matWorld.m[0][0], sizeof(_vec3));

		_vec3			vLength = *D3DXVec3Normalize(&vLook, &vLook) * 5.0f * fTimeDelta;

		m_vEye -= vLength;
		m_vAt -= vLength;
	}

	if(Engine::Get_DIKeyState(DIK_D) & 0x80)
	{
		_vec3			vLook;
		memcpy(&vLook, &matWorld.m[0][0], sizeof(_vec3));

		_vec3			vLength = *D3DXVec3Normalize(&vLook, &vLook) * 5.0f * fTimeDelta;

		m_vEye += vLength;
		m_vAt += vLength;
	}*/

	_long	dwMouseMove = 0;

	if(dwMouseMove = Engine::Get_DIMouseMove(Engine::CInput_Device::DIMS_Y))
	{
		_vec3			vRight;
		memcpy(&vRight, &matWorld.m[0][0], sizeof(_vec3));

		_vec3			vLook = m_vEye - m_vAt;	

		D3DXVec3Normalize(&vLook, &vLook);

		_matrix			matRot;
		D3DXMatrixRotationAxis(&matRot, &vRight, D3DXToRadian(dwMouseMove / 10.0f));		

		D3DXVec3TransformNormal(&vLook, &vLook, &matRot);

		m_vEye = m_vAt + vLook * 2;
	}

	if(dwMouseMove = Engine::Get_DIMouseMove(Engine::CInput_Device::DIMS_X))
	{
		_vec3			vAxis = _vec3(0.f, 1.f, 0.f);

		_vec3			vLook = m_vEye - m_vAt;

		D3DXVec3Normalize(&vLook, &vLook);

		_matrix			matRot;

		D3DXMatrixRotationAxis(&matRot, &vAxis, D3DXToRadian(dwMouseMove / 10.0f));		

		D3DXVec3TransformNormal(&vLook, &vLook, &matRot);

		m_vEye = m_vAt + vLook * 2;
	}
}

CCamera_Stage* CCamera_Stage::Create(LPDIRECT3DDEVICE9 pGraphicDev, const _vec3* pEye, const _vec3* pAt, const _vec3* pUp
										 , _float fFovy, _float fAspect, _float fNear, _float fFar)
{
	CCamera_Stage*	pInstance = new CCamera_Stage(pGraphicDev);

	if(FAILED(pInstance->Ready_GameObject(pEye, pAt, pUp, fFovy, fAspect, fNear, fFar)))
	{
		MSG_BOX("CDynamicCamera Created Failed");
		::Safe_Release(pInstance);
	}
	return pInstance;
}

void CCamera_Stage::Free(void)
{
	Engine::CCamera::Free();
}
