#ifndef GraphicDev_h__
#define GraphicDev_h__

#include "Engine_Defines.h"
#include "Base.h"

// �뵵 : �� ��ǻ�Ϳ� �ִ� �׷���ī����ġ�� �ʱ�ȭ�Ѵ�.
// 1. �׷���ī���� ������ �����ϳ�
// 2. ������ ������ ��Ÿ���� ��ġ�� ����� �� �ֵ��� �غ��Ű��.

BEGIN(Engine)

class ENGINE_DLL CGraphicDev : public CBase
{
	DECLARE_SINGLETON(CGraphicDev)
public:
	enum WINMODE {MODE_FULL, MODE_WIN};	
private:
	explicit CGraphicDev(void);
	virtual ~CGraphicDev(void);
public:
	LPDIRECT3DDEVICE9 Get_GraphicDev(void) const {
		return m_pGraphicDev;}
	void Set_RenderState(D3DRENDERSTATETYPE eType, _ulong dwFlag) {
		m_pGraphicDev->SetRenderState(eType, dwFlag);}
public:
	HRESULT Ready_GraphicDev(HWND hWnd, WINMODE eMode, const _uint& iSizeX, const _uint& iSizeY, Engine::CGraphicDev** ppGraphicDev);
	void Render_Begin(D3DXCOLOR Color);
	void Render_End(void);
private:
	// m_pGrpahicDev��ġ�� �������ִ� ��Ȱ
	// ��ġ�� ������ �����ϴ� ��Ȱ
	LPDIRECT3D9						m_pSDK;

	//��ġ�� ��Ʈ�� �� �� �ִ�(��ġ�� ����� ���� �� �ִ�) ��ü�� �������ش�.	
	LPDIRECT3DDEVICE9				m_pGraphicDev;
public:
	inline virtual void Free(void);
};

END

#endif // GraphicDev_h__
