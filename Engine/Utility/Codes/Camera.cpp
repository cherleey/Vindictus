#include "Camera.h"

USING(Engine)

Engine::CCamera::CCamera(LPDIRECT3DDEVICE9 pGraphicDev)
: Engine::CGameObject(pGraphicDev)
{

}

Engine::CCamera::~CCamera(void)
{

}


HRESULT Engine::CCamera::Ready_GameObject(void)
{
	// For.View Matrix
	D3DXMatrixLookAtLH(&m_matView, &m_vEye, &m_vAt, &m_vUp);

	m_pGraphicDev->SetTransform(D3DTS_VIEW, &m_matView);

	// For.Projection Matrix
	D3DXMatrixPerspectiveFovLH(&m_matProj, m_fFovy, m_fAspect, m_fNear, m_fFar);

	m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &m_matProj);


	return S_OK;
}

_int Engine::CCamera::Update_GameObject(const _float& fTimeDelta)
{	
	D3DXMatrixLookAtLH(&m_matView, &m_vEye, &m_vAt, &m_vUp);

	m_pGraphicDev->SetTransform(D3DTS_VIEW, &m_matView);

	return 0;
}


void Engine::CCamera::Free(void)
{
	Engine::CGameObject::Free();

}


