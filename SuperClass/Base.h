#ifndef Base_h__
#define Base_h__

// CBase�� ���� ����? : dll���� ���ǵ� Ŭ�������� _declspec(dllexport)�� ���ǵǾ��ְ�
// _declspec(dllexport)�� ���ǵǾ��ִ� ũ������ �θ�� _declspec(dllexport)�̾�߸� �Ѵ�.

// _declspec(dllimport)�� ���� ó���k ���� ���� ������ dll���� �Ҽӵ� Ŭ������ �ƴϴ�.
// dllimport�� ����� �� ����.

class _declspec(dllexport) CBase
{
protected:
	inline explicit CBase(void) : m_dwRefCnt(0) {}
	inline virtual ~CBase(void) {}
public:
	inline unsigned long Add_Ref(void); // ���۷���ī��Ʈ�� ������Ų��.
	inline unsigned long Release(void); // ���۷���ī��Ʈ�� ���ҽ�Ų��. or �����Ѵ�.
protected:
	unsigned long m_dwRefCnt;
protected:
	virtual void Free(void)PURE;
};

#include "Base.inl"

#endif // Base_h__
