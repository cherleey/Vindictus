#include "stdafx.h"
#include "Attack_Strong_2.h"

#include "Export_Engine.h"

CAttack_Strong_2::CAttack_Strong_2(LPDIRECT3DDEVICE9 pGraphicDev)
: Engine::CGameObject(pGraphicDev)
, m_fTimeAcc(0.f)
, m_bRender(false)
, m_pRing2(NULL)
{
	
}

CAttack_Strong_2::~CAttack_Strong_2(void)
{

}

HRESULT CAttack_Strong_2::Ready_GameObject(void)
{
	for(int i = 0; i < 4; ++i)
		m_pShield_Zoom[i] = CShield_Zoom_Texture::Create(m_pGraphicDev, _vec3(0.f, 90.f, 0.f));

	m_pRing2 = CRing2_Texture::Create(m_pGraphicDev, _vec3(0.f, 90.f, 0.f));
	m_pRing2->SetPosition(_vec3(-10.f, 0.f, 0.f));

	m_pShape_Up_Shot_Texture[0] = CShape_Up_Shot_Texture::Create(m_pGraphicDev, _vec3(0.f, 90.f, 90.f));
	m_pShape_Up_Shot_Texture[1] = CShape_Up_Shot_Texture::Create(m_pGraphicDev, _vec3(0.f, 0.f, 90.f));
	m_pShape_Up_Shot_Texture[2] = CShape_Up_Shot_Texture::Create(m_pGraphicDev, _vec3(0.f, 45.f, 90.f));
	m_pShape_Up_Shot_Texture[3] = CShape_Up_Shot_Texture::Create(m_pGraphicDev, _vec3(0.f, -45.f, 90.f));

	return S_OK;
}

_int CAttack_Strong_2::Update_GameObject(const _float& fTimeDelta)
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
		m_pShield_Zoom[i]->SetScale(_vec3(m_fTimeAcc * 80.f * i, m_fTimeAcc * 80.f * i, 0.f));
		m_pShield_Zoom[i]->SetPosition(_vec3(m_fTimeAcc * 20.f * i, 0.f, 0.f));
		m_pShield_Zoom[i]->Update_GameObject(fTimeDelta);
	}

	m_pRing2->SetPosition(_vec3(10.f, 0.f, 0.f));
	m_pRing2->Update_GameObject(fTimeDelta);

	for(int i = 0; i < 4; ++i)
	{
		if(NULL != m_pShape_Up_Shot_Texture[i])
		{
			m_pShape_Up_Shot_Texture[i]->Update_GameObject(fTimeDelta);
			m_pShape_Up_Shot_Texture[i]->SetScale(_vec3(0.f, m_fTimeAcc * 50.f, 0.f));
		}
	}

	return 0;
}

CAttack_Strong_2* CAttack_Strong_2::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CAttack_Strong_2* pInstance = new CAttack_Strong_2(pGraphicDev);

	if(FAILED(pInstance->Ready_GameObject()))
	{
		MSG_BOX("CAttack_Strong_2 Created Failed");
		::Safe_Release(pInstance);
	}
	return pInstance;
}

void CAttack_Strong_2::Free(void)
{
	for(int i = 0; i < 4; ++i)
	{
		if(NULL != m_pShield_Zoom[i])
		{
			m_pShield_Zoom[i]->Free();
			m_pShield_Zoom[i] = NULL;
		}
	}

	if(NULL != m_pRing2)
	{
		m_pRing2->Free();
		m_pRing2 = NULL;
	}

	for(int i = 0; i < 4; ++i)
	{
		m_pShape_Up_Shot_Texture[i]->Free();
		m_pShape_Up_Shot_Texture[i] = NULL;
	}
	
	Engine::CGameObject::Free();	
}

void CAttack_Strong_2::SetRenderEnable(_bool bRender)
{
	m_bRender = bRender;

	for(int i = 0; i < 4; ++i)
		m_pShape_Up_Shot_Texture[i]->SetRenderEnable(bRender);

	for(int i = 0; i < 4; ++i)
		m_pShield_Zoom[i]->SetRenderEnable(bRender);

	m_pRing2->SetRenderEnable(bRender);

	if(!bRender)
		m_fTimeAcc = 0.f;
}

