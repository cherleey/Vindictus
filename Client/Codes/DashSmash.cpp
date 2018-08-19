#include "stdafx.h"
#include "DashSmash.h"

#include "Export_Engine.h"

CDashSmash::CDashSmash(LPDIRECT3DDEVICE9 pGraphicDev)
: Engine::CGameObject(pGraphicDev)
, m_pRing2_Texture(NULL)
, m_fTimeAcc(0.f)
, m_bRender(false)
{

}

CDashSmash::~CDashSmash(void)
{

}

HRESULT CDashSmash::Ready_GameObject(void)
{
	for(int i = 0; i < 5; ++i)
	{
		m_pRing_Disk_Texture[i] = CRing_Disk_Texture::Create(m_pGraphicDev, _vec3(0.f, 90.f, 0.f));
		m_pRing_Disk_Texture[i]->SetPosition(_vec3(-20.f, 0.f, 0.f));
	}

	m_pRing2_Texture = CRing2_Texture::Create(m_pGraphicDev, _vec3(0.f, 90.f, 0.f));
	m_pRing2_Texture->SetPosition(_vec3(-20.f, 0.f, 0.f));

	m_pHoly_Shield_Texture = CHoly_Shield_Texture::Create(m_pGraphicDev, _vec3(0.f, 90.f, 0.f));
	m_pHoly_Shield_Texture->SetPosition(_vec3(-20.f, 0.f, 0.f));

	return S_OK;
}

_int CDashSmash::Update_GameObject(const _float& fTimeDelta)
{	
	if(!m_bRender)
	{
		m_fTimeAcc = 0.f;
		return 0;
	}

	Engine::CGameObject::Update_GameObject(fTimeDelta);

	if(m_bRender)
		m_fTimeAcc += fTimeDelta;

	for(int i = 0; i < 5; ++i)
	{
		m_pRing_Disk_Texture[i]->SetScale(_vec3(-10.f, -10.f, 0.f));
		m_pRing_Disk_Texture[i]->SetPosition(_vec3(0.f, 0.f, 0.f));
		m_pRing_Disk_Texture[i]->Update_GameObject(fTimeDelta);
	}

	m_pRing2_Texture->SetPosition(_vec3(10.f, 0.f, 0.f));
	m_pRing2_Texture->Update_GameObject(fTimeDelta);

	m_pHoly_Shield_Texture->SetPosition(_vec3(20.f, 0.f, 0.f));
	m_pHoly_Shield_Texture->Update_GameObject(fTimeDelta);

	return 0;
}

CDashSmash* CDashSmash::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CDashSmash* pInstance = new CDashSmash(pGraphicDev);

	if(FAILED(pInstance->Ready_GameObject()))
	{
		MSG_BOX("CDashSmash Created Failed");
		::Safe_Release(pInstance);
	}
	pInstance->SetRenderEnable(false);
	return pInstance;
}

void CDashSmash::Free(void)
{	
	for(int i = 0; i < 5; ++i)
	{
		if(NULL != m_pRing_Disk_Texture[i])
		{
			m_pRing_Disk_Texture[i]->Free();
			m_pRing_Disk_Texture[i] = NULL;
		}
	}

	if(NULL != m_pRing2_Texture)
	{
		m_pRing2_Texture->Free();
		m_pRing2_Texture = NULL;
	}

	if(NULL != m_pHoly_Shield_Texture)
	{
		m_pHoly_Shield_Texture->Free();
		m_pHoly_Shield_Texture = NULL;
	}

	Engine::CGameObject::Free();	
}

void CDashSmash::SetRenderEnable(_bool bRender)
{
	m_bRender = bRender;

	for(int i = 0; i < 5; ++i)
		m_pRing_Disk_Texture[i]->SetRenderEnable(bRender);

	m_pRing2_Texture->SetRenderEnable(bRender);

	m_pHoly_Shield_Texture->SetRenderEnable(bRender);

	if(!bRender)
		m_fTimeAcc = 0.f;
}

