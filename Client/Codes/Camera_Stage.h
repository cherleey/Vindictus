#ifndef Camera_Stage_h__
#define Camera_Stage_h__

#include "Defines.h"
#include "Camera.h"

class CCamera_Stage : public Engine::CCamera
{
private:
	explicit CCamera_Stage(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CCamera_Stage(void);
public:
	virtual HRESULT Ready_GameObject(const _vec3* pEye, const _vec3* pAt, const _vec3* pUp, _float fFovy, _float fAspect, _float fNear, _float fFar);
	virtual _int Update_GameObject(const _float& fTimeDelta);
	_matrix Get_ViewMatrix(void){
		return m_matView;}
private:
	void Check_InputDev(const _float& fTimeDelta);
private:
	_matrix*					m_pCamBoneMatrix;
	_matrix*					m_pEyeBoneMatrix;
	_matrix*					m_pParentWorldMatrix;
	_matrix						m_matParent;
	Engine::_vec3*				m_pInfomation;
public:
	static CCamera_Stage* Create(LPDIRECT3DDEVICE9 pGraphicDev, const _vec3* pEye, const _vec3* pAt, const _vec3* pUp
		, _float fFovy = D3DXToRadian(60.0f), _float fAspect = _float(WINSIZEX) / WINSIZEY, _float fNear = 0.1f, _float fFar = 1000.f);
private:
	virtual void Free(void);

};


#endif // Dynamic_Camera_h__
