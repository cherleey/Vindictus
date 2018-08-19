#ifndef Scene_Logo_h__
#define Scene_Logo_h__

#include "Defines.h"
#include "Scene.h"


class CLoading;
class CScene_Logo : public Engine::CScene
{
private:	
	explicit CScene_Logo(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CScene_Logo(void);
public:
	virtual HRESULT Ready_Scene(void);
	virtual _int Update_Scene(const _float& fTimeDelta);
	virtual void Render_Scene(void);
private:
	CLoading*			m_pLoading;
private:
	HRESULT Ready_RenderState(void);
	HRESULT Ready_LightInfo(void);
	HRESULT Ready_LayerForBackGround(const _tchar* pLayerTag);
	HRESULT Ready_Component(void);
public:
	static CScene_Logo* Create(LPDIRECT3DDEVICE9 pGraphicDev);
private:
	virtual void Free(void);


};


#endif // Scene_Logo_h__
