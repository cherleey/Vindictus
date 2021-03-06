#ifndef Export_Utility_h__
#define Export_Utility_h__

#include "Engine_Defines.h"
#include "Management.h"
#include "Scene.h"
#include "Layer.h"
#include "GameObject.h"
#include "Renderer.h"
#include "Transform.h"
#include "Texture.h"
#include "Rect_Texture.h"
#include "Cube_Texture.h"
#include "Terrain_Texture.h"
#include "Trail_Texture.h"
#include "StaticMesh.h"
#include "DynamicMesh.h"
#include "Component_Manager.h"
#include "Camera.h"
#include "BoxCollider.h"
#include "Material.h"
#include "Shader.h"

BEGIN(Engine)

//=For.Management =======================================================================================================================================================================================================================================================================================
//-Getter------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
inline Engine::CComponent* Get_Component(const _tchar* pLayerTag, Engine::CGameObject::COMTYPE eType, const _tchar* pComponentTag, const _uint& iObjectIdx = 0);
inline Engine::CLayer* Find_Layer(const _tchar* pLayerTag);
//-Setter------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//-General------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
inline HRESULT Ready_Management(const _uint& iIndex, const _tchar* pRendererTag);
inline _int Update_Management(const _float& fTimeDelta);
inline void Render_Management(void);
inline HRESULT SetUp_CurrentScene(CScene* pCurrentScene);

//=For.Component_Manager =======================================================================================================================================================================================================================================================================================
//-Getter------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//-Setter------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//-General-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
inline HRESULT Reserve_ComponentMgr(const _uint& iSize);
inline HRESULT Add_Component(const _uint& iIndex, const _tchar* pComponentTag, CComponent* pComponent);
inline CComponent* Clone_Component(const _uint& iIndex, const _tchar* pComponentTag);
inline void Release_Component(const _uint& iIndex);

//=Release_Utility====================================================================================================================================================================================================================================================================================================================
inline void Release_Utility(void);

#include "Export_Utility.inl"

END

#endif // Export_Utility_h__
