#ifndef Base_h__
#define Base_h__

// CBase에 붙인 이유? : dll에서 정의된 클래스들이 _declspec(dllexport)로 정의되어있고
// _declspec(dllexport)로 정의되어있는 크랠스는 부모두 _declspec(dllexport)이어야만 한다.

// _declspec(dllimport)에 대한 처리릏 하지 않은 이유는 dll내에 소속된 클래스가 아니다.
// dllimport를 사용할 수 없다.

class _declspec(dllexport) CBase
{
protected:
	inline explicit CBase(void) : m_dwRefCnt(0) {}
	inline virtual ~CBase(void) {}
public:
	inline unsigned long Add_Ref(void); // 레퍼런스카운트를 증가시킨다.
	inline unsigned long Release(void); // 레퍼런스카운트를 감소시킨다. or 삭제한다.
protected:
	unsigned long m_dwRefCnt;
protected:
	virtual void Free(void)PURE;
};

#include "Base.inl"

#endif // Base_h__
