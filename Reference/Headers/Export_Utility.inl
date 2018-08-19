USING(Engine)

//=For.Management =======================================================================================================================================================================================================================================================================================
//-Getter------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
Engine::CComponent* Engine::Get_Component(const _tchar* pLayerTag, Engine::CGameObject::COMTYPE eType, const _tchar* pComponentTag, const _uint& iObjectIdx/* = 0*/)
{
	return CManagement::GetInstance()->Get_Component(pLayerTag, eType, pComponentTag, iObjectIdx);
}
Engine::CLayer* Engine::Find_Layer(const _tchar* pLayerTag)
{
	return CManagement::GetInstance()->Find_Layer(pLayerTag);
}
//-Setter------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//-General------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
HRESULT Engine::Ready_Management(const _uint& iIndex, const _tchar* pRendererTag)
{
	return CManagement::GetInstance()->Ready_Management(iIndex, pRendererTag);
}
_int Engine::Update_Management(const _float& fTimeDelta)
{
	return CManagement::GetInstance()->Update_Management(fTimeDelta);
}
void Engine::Render_Management(void)
{
	CManagement::GetInstance()->Render_Management();
}
HRESULT Engine::SetUp_CurrentScene(CScene* pCurrentScene)
{
	return CManagement::GetInstance()->SetUp_CurrentScene(pCurrentScene);
}

//=For.Component_Manager =======================================================================================================================================================================================================================================================================================
//-Getter------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//-Setter------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//-General------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

HRESULT Engine::Reserve_ComponentMgr(const _uint& iSize)
{
	return CComponent_Manager::GetInstance()->Reserve_ComponentMgr(iSize);
}

HRESULT Engine::Add_Component(const _uint& iIndex, const _tchar* pComponentTag, CComponent* pComponent)
{
	return CComponent_Manager::GetInstance()->Add_Component(iIndex, pComponentTag, pComponent);
}
CComponent* Engine::Clone_Component(const _uint& iIndex, const _tchar* pComponentTag)
{
	return CComponent_Manager::GetInstance()->Clone_Component(iIndex, pComponentTag);
}

void Engine::Release_Component(const _uint& iIndex)
{
	CComponent_Manager::GetInstance()->Release_Component(iIndex);
}

//=Release_Utility====================================================================================================================================================================================================================================================================================================================
void Engine::Release_Utility(void)
{
	if(0 != CManagement::GetInstance()->DestroyInstance())
		MSG_BOX("CManagement Release Failed");	

	if(0 != CComponent_Manager::GetInstance()->DestroyInstance())
		MSG_BOX("CManagement Release Failed");	
}