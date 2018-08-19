#ifndef Camera_h__
#define Camera_h__

#include "GameObject.h"

BEGIN(Engine)

class ENGINE_DLL CCamera : public CGameObject
{
protected:
	explicit CCamera(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CCamera(void);
public:
	virtual HRESULT Ready_GameObject(void);
	virtual _int Update_GameObject(const _float& fTimeDelta);

protected:
	_vec3				m_vEye, m_vAt, m_vUp;
	_matrix				m_matView;

	_float				m_fFovy, m_fAspect, m_fNear, m_fFar;
	_matrix				m_matProj;
protected:
	virtual void Free(void);
};

END

#endif // Camera_h__
