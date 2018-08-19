#ifndef Engine_Functor_h__
#define Engine_Functor_h__

// Functor : �Լ���ü
// �Լ���ä : ��ü�� �Լ�ó�� ���� �ִ°�->��ü()

namespace Engine
{
	// stl�����̳��� ���Ұ� 
	class CRelease_Single
	{
	public:
		explicit CRelease_Single(void) {}
		virtual ~CRelease_Single(void) {}
	public:
		template <typename T>
		void operator () (T& Instance)
		{
			Engine::Safe_Release(Instance);
		}	
	};

	// stl�����̳��� ���Ұ� pairŸ���϶�
	class CRelease_Pair
	{
	public:
		explicit CRelease_Pair(void) {}
		virtual ~CRelease_Pair(void) {}
	public:
		template <typename T>
		void operator () (T& Pair)
		{	
			Engine::Safe_Release(Pair.second);
		}
	};

	class CTag_Finder
	{
	public:
		explicit CTag_Finder(const TCHAR* pString) : m_pString(pString) {}
		virtual ~CTag_Finder(void) {}
	private:
		const TCHAR*			m_pString;
	public:
		template <typename T>
		bool operator () (const T& Pair)
		{
			if(0 == lstrcmp(m_pString, Pair.first))
				return true;
			return false;
		}
	};
}


#endif // Engine_Functor_h__
