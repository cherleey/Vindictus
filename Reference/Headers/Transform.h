#ifndef Transform_h__
#define Transform_h__

#include "Engine_Defines.h"
#include "Component.h"

// 이친구를 포함하고 있는 객체의 월드행렬을 장치에 셋팅한다.

BEGIN(Engine)

class ENGINE_DLL CTransform : public CComponent
{
public:
	enum ANGLE {ANGLE_X, ANGLE_Y, ANGLE_Z};
	enum INFOMATION {INFO_SCALE, INFO_ANGLE, INFO_POSITION, INFO_END};
private:
	explicit CTransform(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CTransform(const CTransform& rhs);
	virtual ~CTransform(void);
public:
	_matrix* Get_WorldMatrix(void) {
		return &m_matWorld;}
	_vec3* Get_Infomation(void) {
		return m_vInfomation;}
public:
	void Set_Infomation(INFOMATION eInfo, const _float& fX, const _float& fY, const _float& fZ );
	void Set_Infomation(INFOMATION eInfo, const _vec3* pVector);
	void Set_ParentMatrix(_matrix* pParentMatrix);
	void SetUp_OnGraphicDev(void);
	void SetUp_OnShader(LPD3DXEFFECT pEffect, const char* pConstantName);
public:
	HRESULT Ready_Component(void);
	void Update_Component(void);
public:
	void Go_StraightForPlayer(const _float& fSpeedTimeDelta);
	void Go_Straight(const _float& fSpeedTimeDelta);
	void Go_Straight(const _float& fSpeedTimeDelta, D3DXVECTOR3 vDir);
	void Rotation(ANGLE eType, const _float& fAngleTimeDelta);
private:
	LPDIRECT3DDEVICE9	m_pGraphicDev;
private:	
	_vec3				m_vInfomation[INFO_END];
	_matrix				m_matWorld;
	_matrix*			m_pParentMatrix;
public:
	virtual CComponent* Clone(void);
	static CTransform* Create(LPDIRECT3DDEVICE9 pGraphicDev);
private:
	virtual void Free(void);
};

END

#endif // Transform_h__
