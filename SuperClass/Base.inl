

// unsigned long : ������Ű�� �� ������ ���
unsigned long CBase::Add_Ref(void)
{
	return ++m_dwRefCnt;
}

// unsigned long : ���ҽ�Ű�� ������ ���
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