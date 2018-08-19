#include "Trail_Texture.h"

USING(Engine)

Engine::CTrail_Texture::CTrail_Texture(LPDIRECT3DDEVICE9 pGraphicDev)
: Engine::CVIBuffer(pGraphicDev)
{

}
Engine::CTrail_Texture::CTrail_Texture(const CTrail_Texture& rhs)
: Engine::CVIBuffer(rhs)
{

}

Engine::CTrail_Texture::~CTrail_Texture(void)
{

}

HRESULT Engine::CTrail_Texture::Ready_Buffer(_uint iBufferCount)
{
	m_dwVtxCnt = (iBufferCount + 1) * 2;
	m_dwVtxSize = sizeof(VTXTEX);
	m_dwVtxFVF = D3DFVF_VTXTEX;

	m_dwTriCnt = iBufferCount * 2;
	m_dwIdxSize = sizeof(INDEX16) * m_dwTriCnt;
	m_IndexFmt = D3DFMT_INDEX16;

	if(FAILED(Engine::CVIBuffer::Ready_Buffer()))
		return E_FAIL;

	// 할당된 메모리에 접근을 하자.
	VTXTEX*				pVertex = NULL;

	m_pVB->Lock(0, 0, (void**)&pVertex, 0);

	for(_int i = 0; i < (_int)m_dwVtxCnt; ++i)
	{
		if(i % 2 == 0)
		{
			pVertex[i].vPosition = _vec3(1.f / iBufferCount * i * 0.5f - 0.5f, 0.5f, 0.f);
			pVertex[i].vTexUV = _vec2(1.f / iBufferCount * i * 0.5f, 0.f);
		}
		else
		{
			pVertex[i].vPosition = _vec3((i - 1) * 0.5f * 1 / iBufferCount - 0.5f, -0.5f, 0.f);
			pVertex[i].vTexUV = _vec2(1.f / iBufferCount * (i - 1) * 0.5f, 1.f);
		}
	}

	m_pVB->Unlock();

	INDEX16*			pIndex = NULL;

	m_pIB->Lock(0, 0, (void**)&pIndex, 0);

	for(_int i = 0; i < (_int)m_dwVtxCnt; i += 2)
	{
		pIndex[i]._0 = i;
		pIndex[i]._1 = i + 2;
		pIndex[i]._2 = i + 3;

		pIndex[i + 1]._0 = i;
		pIndex[i + 1]._1 = i + 3;
		pIndex[i + 1]._2 = i + 1;
	}

	m_pIB->Unlock();
	
	return S_OK;
}

void Engine::CTrail_Texture::Render_Buffer(void)
{
	m_pGraphicDev->SetStreamSource(0, m_pVB, 0, m_dwVtxSize);
	m_pGraphicDev->SetFVF(m_dwVtxFVF);
	m_pGraphicDev->SetIndices(m_pIB);
	m_pGraphicDev->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, m_dwVtxCnt, 0, m_dwTriCnt);
}

CTrail_Texture* Engine::CTrail_Texture::Create(LPDIRECT3DDEVICE9 pGraphicDev, _uint iBufferCount)
{
	CTrail_Texture*		pInstance = new CTrail_Texture(pGraphicDev);

	if(FAILED(pInstance->Ready_Buffer(iBufferCount)))
	{
		MSG_BOX("CTrail_Texture Created Failed");
		Engine::Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent* Engine::CTrail_Texture::Clone(void)
{
	return new CTrail_Texture(*this);
}

void Engine::CTrail_Texture::Free(void)
{
	Engine::CVIBuffer::Free();


}

