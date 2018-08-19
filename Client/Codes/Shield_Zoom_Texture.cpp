#include "stdafx.h"
#include "Shield_Zoom_Texture.h"

#include "Export_Engine.h"

CShield_Zoom_Texture::CShield_Zoom_Texture(LPDIRECT3DDEVICE9 pGraphicDev)
: Engine::CGameObject(pGraphicDev)
, m_pRendererCom(NULL)
, m_pTransformCom(NULL)
, m_pTextureCom(NULL)
, m_pBufferCom(NULL)
, m_pShaderCom(NULL)
, m_fFrame(0.f)
, m_bRender(false)
, m_bParentExist(false)
, m_bRotate(false)
, m_fX(0.f)
, m_fY(0.f)
, m_fZ(0.f)
, m_fTimeAcc(0.f)
, m_bStart(false)
, m_fCX(0.f)
, m_fCY(0.f)
, m_fCZ(0.f)
{

}

CShield_Zoom_Texture::~CShield_Zoom_Texture(void)
{

}

HRESULT CShield_Zoom_Texture::Ready_GameObject(_vec3 vAngle)
{
	if(FAILED(Ready_Component()))
		return E_FAIL;

	//m_pTransformCom->Set_Infomation(Engine::CTransform::INFO_SCALE, 30.f, 30.f, 30.f);
	m_pTransformCom->Rotation(Engine::CTransform::ANGLE_X, D3DXToRadian(vAngle.x));
	m_pTransformCom->Rotation(Engine::CTransform::ANGLE_Y, D3DXToRadian(vAngle.y));
	m_pTransformCom->Rotation(Engine::CTransform::ANGLE_Z, D3DXToRadian(vAngle.z));

	m_pWorld = m_pTransformCom->Get_WorldMatrix();

	return S_OK;
}

_int CShield_Zoom_Texture::Update_GameObject(const _float& fTimeDelta)
{	
	Engine::CGameObject::Update_GameObject(fTimeDelta);

	if(m_bRender)
		m_fTimeAcc += fTimeDelta;
	else
		m_fTimeAcc = 0.f;

	Engine::CTransform* pSwordTransformCom = (Engine::CTransform*)Engine::Get_Component(L"Layer_Equipment", Engine::CGameObject::TYPE_DYNAMIC, L"Com_Transform");
	Engine::CTransform* pPlayerTransformCom = (Engine::CTransform*)Engine::Get_Component(L"Layer_Player", Engine::CGameObject::TYPE_DYNAMIC, L"Com_Transform");

	m_pTransformCom->Set_Infomation(Engine::CTransform::INFO_POSITION, &Engine::_vec3(pSwordTransformCom->Get_WorldMatrix()->_41, pSwordTransformCom->Get_WorldMatrix()->_42, pSwordTransformCom->Get_WorldMatrix()->_43 + 0.5f));

	m_pTransformCom->Set_ParentMatrix(pPlayerTransformCom->Get_WorldMatrix());

	m_pTransformCom->Set_Infomation(Engine::CTransform::INFO_SCALE, 50.f + m_fCX, 50.f + m_fCY, 50.f + m_fCZ);
	m_pTransformCom->Set_Infomation(Engine::CTransform::INFO_POSITION, 30.f + m_fX, 35.f + m_fY, -10.f + m_fZ);

	m_pRendererCom->Add_RenderList(Engine::CRenderer::RENDER_ALPHA, this);

	return 0;
}

void CShield_Zoom_Texture::Render_GameObject(void)
{
	if(!m_bRender)
	{
		m_bStart = false;
		m_fTimeAcc = 0.f;
		return;
	}

	LPD3DXEFFECT	pEffect = m_pShaderCom->Get_EffectHandle();

	if(NULL == pEffect)
		return;

	pEffect->AddRef();

	pEffect->SetBool("g_bStart", m_bStart);

	if(!m_bStart)
		m_bStart = true;

	_uint		iNumPass = 0;

	m_pTransformCom->SetUp_OnShader(pEffect, "g_matWorld");

	_matrix		matView, matProj;
	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
	m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &matProj);

	pEffect->SetMatrix("g_matView", &matView);
	pEffect->SetMatrix("g_matProj", &matProj);
	pEffect->SetFloat("g_fTimeAcc", m_fTimeAcc);

	m_pTextureCom->SetUp_OnShader(pEffect, "g_BaseTexture", (_uint)m_fFrame);

	pEffect->Begin(&iNumPass, 0);

	pEffect->BeginPass(0);

	m_pBufferCom->Render_Buffer();

	pEffect->EndPass();

	pEffect->End();

	Engine::Safe_Release(pEffect);		
}

HRESULT CShield_Zoom_Texture::Ready_Component(void)
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

	m_pTextureCom = (Engine::CTexture*)Engine::Clone_Component(SCENE_STAGE, L"Component_Texture_Shield_Zoom");
	if(NULL == m_pTextureCom)
		return E_FAIL;
	if(FAILED(Add_Component(Engine::CGameObject::TYPE_STATIC, L"Com_Texture", m_pTextureCom)))
		return E_FAIL;

	m_pBufferCom = (Engine::CRect_Texture*)Engine::Clone_Component(SCENE_STATIC, L"Component_Buffer_RectTexture");
	if(NULL == m_pBufferCom)
		return E_FAIL;
	if(FAILED(Add_Component(Engine::CGameObject::TYPE_STATIC, L"Com_Buffer", m_pBufferCom)))
		return E_FAIL;

	m_pShaderCom = (Engine::CShader*)Engine::Clone_Component(SCENE_STAGE, L"Component_Shader_Effect");
	if(NULL == m_pShaderCom)
		return E_FAIL;
	if(FAILED(Add_Component(Engine::CGameObject::TYPE_STATIC, L"Com_Shader", m_pShaderCom)))
		return E_FAIL;


	return S_OK;
}

CShield_Zoom_Texture* CShield_Zoom_Texture::Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 vAngle)
{
	CShield_Zoom_Texture* pInstance = new CShield_Zoom_Texture(pGraphicDev);

	if(FAILED(pInstance->Ready_GameObject(vAngle)))
	{
		MSG_BOX("CShield_Zoom_Texture Created Failed");
		::Safe_Release(pInstance);
	}
	return pInstance;
}

void CShield_Zoom_Texture::Free(void)
{	

	Engine::Safe_Release(m_pRendererCom);
	Engine::Safe_Release(m_pTransformCom);
	Engine::Safe_Release(m_pTextureCom);
	Engine::Safe_Release(m_pBufferCom);
	Engine::Safe_Release(m_pShaderCom);

	Engine::CGameObject::Free();	

	delete(this);
}

void CShield_Zoom_Texture::Rotate(_vec3 vAngle)
{
	m_pTransformCom->Rotation(Engine::CTransform::ANGLE_X, D3DXToRadian(vAngle.x));
	m_pTransformCom->Rotation(Engine::CTransform::ANGLE_Y, D3DXToRadian(vAngle.y));
	m_pTransformCom->Rotation(Engine::CTransform::ANGLE_Z, D3DXToRadian(vAngle.z));
}

void CShield_Zoom_Texture::SetPosition(_vec3 vPos)
{
	m_fX = vPos.x;
	m_fY = vPos.y;
	m_fZ = vPos.z;
}

void CShield_Zoom_Texture::SetScale(_vec3 vScale)
{
	m_fCX = vScale.x;
	m_fCY = vScale.y;
	m_fCZ = vScale.z;
}
