#ifndef Loading_h__
#define Loading_h__

#include "Defines.h"
#include "Base.h"

class CLoading : public CBase
{
public:
	enum LOADINGID {LOADING_STAGE, LOADING_BOSS, LOADING_END};
private:
	explicit CLoading(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CLoading(void);
public:
	LOADINGID Get_LoadingID(void) const {
		return m_LoadingID;}
	CRITICAL_SECTION* Get_CriticalSection(void) {
		return &m_Crt;}
	_bool Get_Finish(void) const {
		return m_isFinish;}
	const _tchar* Get_String(void) const {
		return m_szloadingString;}
public:
	HRESULT Ready_Loading(LOADINGID eLoadingID);
	_uint Loading_ForStage(void);
public:
	static unsigned int APIENTRY Thread_Main(void* pArg);


	
private:
	HANDLE						m_hThread;
	CRITICAL_SECTION			m_Crt;
	LOADINGID					m_LoadingID;
private:
	LPDIRECT3DDEVICE9			m_pGraphicDev;
private:
	_tchar						m_szloadingString[128];
	_bool						m_isFinish;
public:
	static CLoading* Create(LPDIRECT3DDEVICE9 pGraphicDev, LOADINGID eLoadingID);
private:
	virtual void Free(void);

};


#endif // Loading_h__
