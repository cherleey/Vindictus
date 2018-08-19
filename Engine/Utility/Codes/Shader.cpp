#include "Shader.h"

USING(Engine)

Engine::CShader::CShader(LPDIRECT3DDEVICE9 pGraphicDev)
: m_pGraphicDev(pGraphicDev)
, m_pEffect(NULL)
, m_pErrBuffer(NULL)
{
	m_pGraphicDev->AddRef();
}

Engine::CShader::CShader(const CShader& rhs)
: m_pGraphicDev(rhs.m_pGraphicDev)
, m_pEffect(rhs.m_pEffect)
, m_pErrBuffer(rhs.m_pErrBuffer)
{
	m_pGraphicDev->AddRef();
	m_pEffect->AddRef();

	if(NULL != m_pErrBuffer)
		m_pErrBuffer->AddRef();
}

Engine::CShader::~CShader(void)
{

}

HRESULT Engine::CShader::Ready_Shader(const _tchar* pFilePath)
{
	
	// hlsl�� ������ ������ �����Ͽ� LPD3DXEFFCT��ü�� �����Ѵ�.
	if(FAILED(D3DXCreateEffectFromFile(m_pGraphicDev, pFilePath, NULL, NULL, D3DXSHADER_DEBUG, NULL, &m_pEffect, &m_pErrBuffer)))
	{
		// ���� § hlsl�ǵ忡 ������ ���� ���
		MessageBoxA(NULL, (char*)m_pErrBuffer->GetBufferPointer(), NULL, MB_OK);

		return E_FAIL;		
	}

	if(NULL != m_pErrBuffer)
	{
		MessageBoxA(NULL, (char*)m_pErrBuffer->GetBufferPointer(), NULL, MB_OK);	
	}

/*
	m_pEffect->SetMatrix();
	m_pEffect->SetFloat();
	m_pEffect->SetVector();
	m_pEffect->SetTexture();*/

	return S_OK;
}

CComponent* Engine::CShader::Clone(void)
{
	return new CShader(*this);
}

CShader* Engine::CShader::Create(LPDIRECT3DDEVICE9 pGraphicDev, const _tchar* pFilePath)
{
	CShader*		pInstance = new CShader(pGraphicDev);

	if(FAILED(pInstance->Ready_Shader(pFilePath)))
	{
		MSG_BOX("CShader Created Failed");
		Engine::Safe_Release(pInstance);
	}
	return pInstance;
}

void Engine::CShader::Free(void)
{
	Engine::Safe_Release(m_pGraphicDev);
	Engine::Safe_Release(m_pEffect);
	Engine::Safe_Release(m_pErrBuffer);

}

