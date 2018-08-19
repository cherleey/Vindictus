#ifndef Engine_Functor_h__
#define Engine_Functor_h__

// Functor : 함수객체
// 함수객채 : 객체를 함수처럼 쓸수 있는거->객체()

namespace Engine
{
	// stl컨테이너의 원소가 
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

	// stl컨테이너의 원소가 pair타입일때
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
