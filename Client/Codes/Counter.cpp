#include "stdafx.h"
#include "Counter.h"

#include "Export_Engine.h"

CCounter::CCounter(LPDIRECT3DDEVICE9 pGraphicDev)
: Engine::CGameObject(pGraphicDev)
, m_pRing2_Texture(NULL)
, m_fTimeAcc(0.f)
, m_bRender(false)
{

}

CCounter::~CCounter(void)
{

}

HRESULT CCounter::Ready_GameObject(void)
{
	m_pYellow_Blunt_Texture[0] = CYellow_Blunt_Texture::Create(m_pGraphicDev, _vec3(0.f, 90.f, 270.f));
	m_pYellow_Blunt_Texture[1] = CYellow_Blunt_Texture::Create(m_pGraphicDev, _vec3(0.f, 0.f, 270.f));
	m_pYellow_Blunt_Texture[2] = CYellow_Blunt_Texture::Create(m_pGraphicDev, _vec3(0.f, 45.f, 270.f));
	m_pYellow_Blunt_Texture[3] = CYellow_Blunt_Texture::Create(m_pGraphicDev, _vec3(0.f, -45.f, 270.f));

	m_pRing2_Texture = CRing2_Texture::Create(m_pGraphicDev, _vec3(0.f, 90.f, 0.f));
	m_pRing2_Texture->SetPosition(_vec3(-20.f, 0.f, 0.f));

	return S_OK;
}

_int CCounter::Update_GameObject(const _float& fTimeDelta)
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
		if(NULL != m_pYellow_Blunt_Texture[i])
		{
			m_pYellow_Blunt_Texture[i]->SetScale(_vec3(0.f, m_fTimeAcc * 500.f, 0.f));
			m_pYellow_Blunt_Texture[i]->SetPosition(_vec3(-40.f, 0.f, 0.f));
			m_pYellow_Blunt_Texture[i]->Update_GameObject(fTimeDelta);
		}
	}

	m_pRing2_Texture->SetPosition(_vec3(20.f, 0.f, 0.f));
	m_pRing2_Texture->Update_GameObject(fTimeDelta);

	return 0;
}

CCounter* CCounter::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CCounter* pInstance = new CCounter(pGraphicDev);

	if(FAILED(pInstance->Ready_GameObject()))
	{
		MSG_BOX("CCounter Created Failed");
		::Safe_Release(pInstance);
	}
	pInstance->SetRenderEnable(false);
	return pInstance;
}

void CCounter::Free(void)
{	
	for(int i = 0; i < 4; ++i)
	{
		m_pYellow_Blunt_Texture[i]->Free();
		m_pYellow_Blunt_Texture[i] = NULL;
	}

	if(NULL != m_pRing2_Texture)
	{
		m_pRing2_Texture->Free();
		m_pRing2_Texture = NULL;
	}

	Engine::CGameObject::Free();	
}

void CCounter::SetRenderEnable(_bool bRender)
{
	m_bRender = bRender;

	for(int i = 0; i < 4; ++i)
		m_pYellow_Blunt_Texture[i]->SetRenderEnable(bRender);

	m_pRing2_Texture->SetRenderEnable(bRender);

	if(!bRender)
		m_fTimeAcc = 0.f;
}

