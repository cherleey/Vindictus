#include "stdafx.h"
#include "Attack_Strong_0.h"

#include "Export_Engine.h"

CAttack_Strong_0::CAttack_Strong_0(LPDIRECT3DDEVICE9 pGraphicDev)
: Engine::CGameObject(pGraphicDev)
, m_pSword_Sharp_Plane_Texture(NULL)
, m_pSword_Radial_Blur_Texture(NULL)
, m_fTimeAcc(0.f)
, m_bRender(false)
{

}

CAttack_Strong_0::~CAttack_Strong_0(void)
{

}

HRESULT CAttack_Strong_0::Ready_GameObject(void)
{
	m_pSword_Sharp_Line_Texture[0] = CSword_Sharp_Line_Texture::Create(m_pGraphicDev, _vec3(0.f, 90.f, 90.f));
	m_pSword_Sharp_Line_Texture[1] = CSword_Sharp_Line_Texture::Create(m_pGraphicDev, _vec3(0.f, 0.f, 90.f));
	m_pSword_Sharp_Line_Texture[2] = CSword_Sharp_Line_Texture::Create(m_pGraphicDev, _vec3(0.f, 45.f, 90.f));
	m_pSword_Sharp_Line_Texture[3] = CSword_Sharp_Line_Texture::Create(m_pGraphicDev, _vec3(0.f, -45.f, 90.f));

	m_pSword_Sharp_Plane_Texture = CSword_Sharp_Plane_Texture::Create(m_pGraphicDev, _vec3(0.f, 90.f, 0.f));
	m_pSword_Sharp_Plane_Texture->SetPosition(_vec3(-20.f, 0.f, 0.f));

	m_pSword_Radial_Blur_Texture = CSword_Radial_Blur_Texture::Create(m_pGraphicDev, _vec3(0.f, 90.f, 0.f));
	m_pSword_Sharp_Plane_Texture->SetPosition(_vec3(-10.f, 0.f, 0.f));

	return S_OK;
}

_int CAttack_Strong_0::Update_GameObject(const _float& fTimeDelta)
{	
	if(!m_bRender)
	{
		m_fTimeAcc = 0.f;
		return 0;
	}

	Engine::CGameObject::Update_GameObject(fTimeDelta);

	if(m_bRender)
		m_fTimeAcc += fTimeDelta;

	for(int i = 0; i < 4; ++i)
	{
		if(NULL != m_pSword_Sharp_Line_Texture[i])
		{
			m_pSword_Sharp_Line_Texture[i]->Update_GameObject(fTimeDelta);
			m_pSword_Sharp_Line_Texture[i]->SetScale(_vec3(0.f, m_fTimeAcc * 500.f, 0.f));
		}
	}

	m_pSword_Sharp_Plane_Texture->Update_GameObject(fTimeDelta);

	m_pSword_Radial_Blur_Texture->Update_GameObject(fTimeDelta);

	return 0;
}

CAttack_Strong_0* CAttack_Strong_0::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CAttack_Strong_0* pInstance = new CAttack_Strong_0(pGraphicDev);

	if(FAILED(pInstance->Ready_GameObject()))
	{
		MSG_BOX("CAttack_Strong_0 Created Failed");
		::Safe_Release(pInstance);
	}
	return pInstance;
}

void CAttack_Strong_0::Free(void)
{	
	for(int i = 0; i < 4; ++i)
	{
		m_pSword_Sharp_Line_Texture[i]->Free();
		m_pSword_Sharp_Line_Texture[i] = NULL;
	}

	if(NULL != m_pSword_Sharp_Plane_Texture)
	{
		m_pSword_Sharp_Plane_Texture->Free();
		m_pSword_Sharp_Plane_Texture = NULL;
	}

	if(NULL != m_pSword_Radial_Blur_Texture)
	{
		m_pSword_Radial_Blur_Texture->Free();
		m_pSword_Radial_Blur_Texture = NULL;
	}

	Engine::CGameObject::Free();	
}

void CAttack_Strong_0::SetRenderEnable(_bool bRender)
{
	m_bRender = bRender;

	for(int i = 0; i < 4; ++i)
		m_pSword_Sharp_Line_Texture[i]->SetRenderEnable(bRender);

	m_pSword_Sharp_Plane_Texture->SetRenderEnable(bRender);
	m_pSword_Radial_Blur_Texture->SetRenderEnable(bRender);

	if(!bRender)
		m_fTimeAcc = 0.f;
}

