#include "stdafx.h"
#include "SkyBox.h"

#include "Export_Engine.h"

CSkyBox::CSkyBox(LPDIRECT3DDEVICE9 pGraphicDev)
: Engine::CGameObject(pGraphicDev)
{

}

CSkyBox::~CSkyBox(void)
{

}

HRESULT CSkyBox::Ready_GameObject(void)
{
	if(FAILED(Ready_Component()))
		return E_FAIL;

	m_pTransformCom->Set_Infomation(Engine::CTransform::INFO_SCALE, 20.f, 20.f, 20.f);


	return S_OK;
}

_int CSkyBox::Update_GameObject(const _float& fTimeDelta)
{
	
	Engine::CGameObject::Update_GameObject(fTimeDelta);	
	
	m_pRendererCom->Add_RenderList(Engine::CRenderer::RENDER_PRIORITY, this);

	return 0;
}

_int CSkyBox::LastUpdate_GameObject(const _float& fTimeDelta)
{
	// 카메라의 위치를 얻어온다.
	_matrix				matView;
	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);

	// matView : 카메라의 월드행렬
	D3DXMatrixInverse(&matView, NULL, &matView);

	m_pTransformCom->Set_Infomation(Engine::CTransform::INFO_POSITION, (Engine::_vec3*)&matView.m[3][0]);

	m_pTransformCom->Update_Component();

	return 0;
}

void CSkyBox::Render_GameObject(void)
{
	Engine::Set_RenderState(D3DRS_ZWRITEENABLE, FALSE);
	Engine::Set_RenderState(D3DRS_LIGHTING, FALSE);
	Engine::Set_RenderState(D3DRS_CULLMODE, D3DCULL_CW);

	m_pTransformCom->SetUp_OnGraphicDev();
	m_pTextureCom->SetUp_OnGraphicDev(3);	
	m_pBufferCom->Render_Buffer();

	Engine::Set_RenderState(D3DRS_ZWRITEENABLE, TRUE);
	Engine::Set_RenderState(D3DRS_LIGHTING, TRUE);
	Engine::Set_RenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

HRESULT CSkyBox::Ready_Component(void)
{
	m_pRendererCom = (Engine::CRenderer*)Engine::Clone_Component(SCENE_STATIC, L"Component_Renderer");
	if(NULL == m_pRendererCom)
		return E_FAIL;
	if(FAILED(Add_Component(Engine::CGameObject::TYPE_STATIC, L"Com_Renderer", m_pRendererCom)))
		return E_FAIL;

	m_pTransformCom = (Engine::CTransform*)Engine::Clone_Component(SCENE_STATIC, L"Component_Transform");
	if(NULL == m_pTransformCom)
		return E_FAIL;
	if(FAILED(Add_Component(Engine::CGameObject::TYPE_DYNAMIC, L"Com_Transform", m_pTransformCom)))
		return E_FAIL;

	m_pTextureCom = (Engine::CTexture*)Engine::Clone_Component(SCENE_STAGE, L"Component_Texture_SkyBox");
	if(NULL == m_pTextureCom)
		return E_FAIL;
	if(FAILED(Add_Component(Engine::CGameObject::TYPE_STATIC, L"Com_Texture", m_pTextureCom)))
		return E_FAIL;

	m_pBufferCom = (Engine::CCube_Texture*)Engine::Clone_Component(SCENE_STAGE, L"Component_Buffer_Cube");
	if(NULL == m_pBufferCom)
		return E_FAIL;
	if(FAILED(Add_Component(Engine::CGameObject::TYPE_STATIC, L"Com_Buffer", m_pBufferCom)))
		return E_FAIL;	

	return S_OK;
}

CSkyBox* CSkyBox::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CSkyBox* pInstance = new CSkyBox(pGraphicDev);

	if(FAILED(pInstance->Ready_GameObject()))
	{
		MSG_BOX("CSkyBox Created Failed");
		::Safe_Release(pInstance);
	}
	return pInstance;
}

void CSkyBox::Free(void)
{
	Engine::Safe_Release(m_pRendererCom);
	Engine::Safe_Release(m_pTransformCom);
	Engine::Safe_Release(m_pTextureCom);
	Engine::Safe_Release(m_pBufferCom);

	Engine::CGameObject::Free();	
}

