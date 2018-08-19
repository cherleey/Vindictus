#include "stdafx.h"
#include "Trail.h"

#include "Export_Engine.h"

CTrail::CTrail(LPDIRECT3DDEVICE9 pGraphicDev)
: Engine::CGameObject(pGraphicDev)
, m_pRendererCom(NULL)
, m_pTransformCom(NULL)
, m_pTextureCom(NULL)
, m_pBufferCom(NULL)
, m_pShaderCom(NULL)
, m_fFrame(0.f)
, m_iTrailNum(0)
, m_bRender(NULL)
{

}

CTrail::~CTrail(void)
{

}

HRESULT CTrail::Ready_GameObject(void)
{
	if(FAILED(Ready_Component()))
		return E_FAIL;
	//m_pTransformCom->Set_Infomation(Engine::CTransform::INFO_POSITION, &Engine::_vec3(m_pSwordWorld->_41, m_pSwordWorld->_42, m_pSwordWorld->_43));
	m_pTransformCom->Set_Infomation(Engine::CTransform::INFO_SCALE, 1.f, 1.f, 1.f);

	return S_OK;
}

_int CTrail::Update_GameObject(const _float& fTimeDelta)
{	
	Engine::CBoxCollider* pSwordColliderCom = (Engine::CBoxCollider*)Engine::Get_Component(L"Layer_Equipment", Engine::CGameObject::TYPE_STATIC, L"Com_Collider");

	_vec3 vMax = pSwordColliderCom->GetMax();
	_vec3 vMin = pSwordColliderCom->GetMin();

	Engine::CGameObject::Update_GameObject(fTimeDelta);

	Engine::CTransform* pSwordTransformCom = (Engine::CTransform*)Engine::Get_Component(L"Layer_Equipment", Engine::CGameObject::TYPE_DYNAMIC, L"Com_Transform");

	_matrix* pSwordWorld = pSwordTransformCom->Get_WorldMatrix();

	D3DXVec3TransformCoord(&vMax, &vMax, pSwordWorld);
	D3DXVec3TransformCoord(&vMin, &vMin, pSwordWorld);

	m_pRendererCom->Add_RenderList(Engine::CRenderer::RENDER_ALPHA, this);

	m_TrailLIst.push_front(_vec4(vMax.x, vMax.y, vMax.z, 1.f));
	m_TrailLIst.push_front(_vec4(vMin.x, vMin.y, vMin.z, 1.f));
	m_iTrailNum += 2;

	//µð¹ö±×
	//g_fDebugTime += fTimeDelta;
	//if(g_fDebugTime > 1.f)
	//{
	//	//system("cls");


	//	/*TRAILLIST::iterator iter = m_TrailLIst.begin();
	//	TRAILLIST::iterator iter_end = m_TrailLIst.end();

	//	for(int i = 0;iter != iter_end; ++iter, ++i)
	//	{
	//		cout << i << " : " << (*iter).x << ", " << (*iter).y << ", " << (*iter).z << ", " << (*iter).w << endl;
	//	}*/

	//	for(int i = 0; i < 22 ; ++i)
	//		cout << i << " : " << m_TransformedVertices[i].x << ", " << m_TransformedVertices[i].y << ", " << m_TransformedVertices[i].z << endl;

	//	cout << endl;
	//	cout << endl;
	//	cout << endl;
	//	cout << endl;
	//	cout << endl;

	//	//cout << pPlayerWorld->_41 << " " << pPlayerWorld->_42 << " " << pPlayerWorld->_43 << endl;

	//	g_fDebugTime = 0.f;
	//}

	if(m_iTrailNum > 22)
	{
		m_TrailLIst.pop_back();
		m_TrailLIst.pop_back();
	}

	TRAILLIST::iterator iter = m_TrailLIst.begin();
	TRAILLIST::iterator iter_end = m_TrailLIst.end();

	for(int i = 0;iter != iter_end; ++iter, ++i)
		m_TransformedVertices[i] = (*iter);

	return 0;
}

void CTrail::Render_GameObject(void)
{
	if(!m_bRender)
		return;

	LPD3DXEFFECT pEffect = m_pShaderCom->Get_EffectHandle();

	if(NULL == pEffect)
		return;

	pEffect->AddRef();

	pEffect->SetVectorArray("g_TransformedVertecise", m_TransformedVertices, 22);

	_uint		iNumPass = 0;

	m_pTransformCom->SetUp_OnShader(pEffect, "g_matWorld");

	_matrix		matView, matProj;
	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
	m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &matProj);

	pEffect->SetMatrix("g_matView", &matView);
	pEffect->SetMatrix("g_matProj", &matProj);

	m_pTextureCom->SetUp_OnShader(pEffect, "g_BaseTexture", (_uint)m_fFrame);

	pEffect->Begin(&iNumPass, 0);

	pEffect->BeginPass(0);

	m_pBufferCom->Render_Buffer();

	pEffect->EndPass();

	pEffect->End();
	
	Engine::Safe_Release(pEffect);		
}

HRESULT CTrail::Ready_Component(void)
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

	m_pTextureCom = (Engine::CTexture*)Engine::Clone_Component(SCENE_STAGE, L"Component_Texture_Trail");
	if(NULL == m_pTextureCom)
		return E_FAIL;
	if(FAILED(Add_Component(Engine::CGameObject::TYPE_STATIC, L"Com_Texture", m_pTextureCom)))
		return E_FAIL;

	m_pBufferCom = (Engine::CTrail_Texture*)Engine::Clone_Component(SCENE_STATIC, L"Component_Buffer_TrailTexture");
	if(NULL == m_pBufferCom)
		return E_FAIL;
	if(FAILED(Add_Component(Engine::CGameObject::TYPE_STATIC, L"Com_Buffer", m_pBufferCom)))
		return E_FAIL;

	m_pShaderCom = (Engine::CShader*)Engine::Clone_Component(SCENE_STAGE, L"Component_Shader_Trail");
	if(NULL == m_pShaderCom)
		return E_FAIL;
	if(FAILED(Add_Component(Engine::CGameObject::TYPE_STATIC, L"Com_Shader", m_pShaderCom)))
		return E_FAIL;


	return S_OK;
}

CTrail* CTrail::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CTrail* pInstance = new CTrail(pGraphicDev);

	if(FAILED(pInstance->Ready_GameObject()))
	{
		MSG_BOX("CTrail Created Failed");
		::Safe_Release(pInstance);
	}
	return pInstance;
}

void CTrail::Free(void)
{	
	Engine::Safe_Release(m_pRendererCom);
	Engine::Safe_Release(m_pTransformCom);
	Engine::Safe_Release(m_pTextureCom);
	Engine::Safe_Release(m_pBufferCom);
	Engine::Safe_Release(m_pShaderCom);

	Engine::CGameObject::Free();
}

