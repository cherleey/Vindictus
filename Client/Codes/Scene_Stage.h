#ifndef Scene_Stage_h__
#define Scene_Stage_h__

#include "Defines.h"
#include "Scene.h"

class CScene_Stage : public Engine::CScene
{
private:	
	explicit CScene_Stage(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CScene_Stage(void);
public:
	virtual HRESULT Ready_Scene(void);
	virtual _int Update_Scene(const _float& fTimeDelta);
	virtual void Render_Scene(void);
private:
	HRESULT Ready_RenderState(void);
	HRESULT Ready_LightInfo(void);
	HRESULT Ready_LayerForCamera(const _tchar* pLayerTag);
	HRESULT Ready_LayerForEnvironment(const _tchar* pLayerTag);
	HRESULT Ready_LayerForBackGround(const _tchar* pLayerTag);
	HRESULT Ready_LayerForEquipment(const _tchar* pLayerTag);
	HRESULT Ready_LayerForPlayer(const _tchar* pLayerTag);
	HRESULT Ready_LayerForBoss(const _tchar* pLayerTag);
	HRESULT Ready_LayerForEffect(const _tchar* pLayerTag);
public:
	static CScene_Stage* Create(LPDIRECT3DDEVICE9 pGraphicDev);
private:
	virtual void Free(void);


};


#endif // Scene_Logo_h__
