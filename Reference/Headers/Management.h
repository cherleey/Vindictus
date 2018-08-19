#ifndef Management_h__
#define Management_h__

#include "Engine_Defines.h"
#include "Base.h"
#include "GameObject.h"
#include "Layer.h"

// ���� �������� ���� �����͸� ������ ������
// �� ���� ���� �Ǵ� �׸��� �۾��� �����Ѵ�.
// Renderer

BEGIN(Engine)

class CScene;
class CRenderer;
class CComponent;
class ENGINE_DLL CManagement : public CBase
{
	DECLARE_SINGLETON(CManagement)
private:
	explicit CManagement(void);
	virtual ~CManagement(void);	
public:
	Engine::CComponent* Get_Component(const _tchar* pLayerTag, Engine::CGameObject::COMTYPE eType, const _tchar* pComponentTag, const _uint& iObjectIdx = 0);
	Engine::CLayer* Find_Layer(const _tchar* pLayerTag);

public:
	HRESULT Ready_Management(const _uint& iIndex, const _tchar* pRendererTag);
	_int Update_Management(const _float& fTimeDelta);
	void Render_Management(void);
	HRESULT SetUp_CurrentScene(CScene* pCurrentScene);
private:
	CScene*			m_pCurrentScene;
	CRenderer*		m_pRenderer;
private:
	virtual void Free(void);
};

END

#endif // Management_h__
