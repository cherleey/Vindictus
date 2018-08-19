

// unsigned long : 증가시키고 난 이후의 결과
unsigned long CBase::Add_Ref(void)
{
	return ++m_dwRefCnt;
}

// unsigned long : 감소시키기 이전의 결과
unsigned long CBase::Release(void)
{
	if(0 == m_dwRefCnt)
	{
		Free();
		delete this;
		return 0;
	}

	return m_dwRefCnt--;	
}