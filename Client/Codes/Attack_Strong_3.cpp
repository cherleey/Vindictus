#include "stdafx.h"
#include "Attack_Strong_3.h"

#include "Export_Engine.h"

CAttack_Strong_3::CAttack_Strong_3(LPDIRECT3DDEVICE9 pGraphicDev)
: Engine::CGameObject(pGraphicDev)
, m_fTimeAcc(0.f)
, m_bRender(false)
, m_pWind_Texture(NULL)
, m_pRing_Outer_Wind(NULL)
{
	
}

CAttack_Strong_3::~CAttack_Strong_3(void)
{

}

HRESULT CAttack_Strong_3::Ready_GameObject(void)
{
	m_pBlunt_Texture[0] = CBlunt_Texture::Create(m_pGraphicDev, _vec3(0.f, 90.f, 90.f));
	m_pBlunt_Texture[1] = CBlunt_Texture::Create(m_pGraphicDev, _vec3(0.f, 0.f, 90.f));
	m_pBlunt_Texture[2] = CBlunt_Texture::Create(m_pGraphicDev, _vec3(0.f, 45.f, 90.f));
	m_pBlunt_Texture[3] = CBlunt_Texture::Create(m_pGraphicDev, _vec3(0.f, -45.f, 90.f));

	m_pWind_Texture = CWind_Texture::Create(m_pGraphicDev, _vec3(0.f, 90.f, 0.f));
	m_pWind_Texture->SetPosition(_vec3(-20.f, 0.f, 0.f));

	m_pRing_Outer_Wind = CRing_Outer_Wind06_Texture::Create(m_pGraphicDev, _vec3(0.f, 90.f, 0.f));
	m_pRing_Outer_Wind->SetPosition(_vec3(-20.f, 0.f, 0.f));

	return S_OK;
}

_int CAttack_Strong_3::Update_GameObject(const _float& fTimeDelta)
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
		if(NULL != m_pBlunt_Texture[i])
		{
			m_pBlunt_Texture[i]->SetScale(_vec3(0.f, m_fTimeAcc * 200.f, 0.f));
			m_pBlunt_Texture[i]->SetPosition(_vec3(50.f, 5.f, 10.f));
			m_pBlunt_Texture[i]->Update_GameObject(fTimeDelta);
		}
	}

	m_pWind_Texture->SetScale(_vec3(m_fTimeAcc * 50.f, m_fTimeAcc * 50.f, 0.f));
	m_pWind_Texture->SetPosition(_vec3(10.f, 5.f, 10.f));
	m_pWind_Texture->Update_GameObject(fTimeDelta);

	m_pRing_Outer_Wind->SetScale(_vec3(m_fTimeAcc * 70.f, m_fTimeAcc * 70.f, 0.f));
	m_pRing_Outer_Wind->SetPosition(_vec3(0.f, 5.f, 10.f));
	m_pRing_Outer_Wind->Update_GameObject(fTimeDelta);

	return 0;
}

CAttack_Strong_3* CAttack_Strong_3::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CAttack_Strong_3* pInstance = new CAttack_Strong_3(pGraphicDev);

	if(FAILED(pInstance->Ready_GameObject()))
	{
		MSG_BOX("CAttack_Strong_3 Created Failed");
		::Safe_Release(pInstance);
	}
	return pInstance;
}

void CAttack_Strong_3::Free(void)
{
	for(int i = 0; i < 4; ++i)
	{
		m_pBlunt_Texture[i]->Free();
		m_pBlunt_Texture[i] = NULL;
	}

	if(NULL != m_pWind_Texture)
	{
		m_pWind_Texture->Free();
		m_pWind_Texture = NULL;
	}

	if(NULL != m_pRing_Outer_Wind)
	{
		m_pRing_Outer_Wind->Free();
		m_pRing_Outer_Wind = NULL;
	}
	
	Engine::CGameObject::Free();	
}

void CAttack_Strong_3::SetRenderEnable(_bool bRender)
{
	m_bRender = bRender;

	for(int i = 0; i < 4; ++i)
		m_pBlunt_Texture[i]->SetRenderEnable(bRender);

	m_pWind_Texture->SetRenderEnable(bRender);

	m_pRing_Outer_Wind->SetRenderEnable(bRender);

	if(!bRender)
		m_fTimeAcc = 0.f;
}

