#include "Input_Device.h"

USING(Engine)
IMPLEMENT_SINGLETON(CInput_Device)

Engine::CInput_Device::CInput_Device(void)
: m_pInputSDK(NULL)
, m_pKeyBoard(NULL)
, m_pMouse(NULL)
{

}

Engine::CInput_Device::~CInput_Device()
{

}

HRESULT Engine::CInput_Device::Ready_InputDevice(HINSTANCE hInst, HWND hWnd)
{
	// ��Ű���带 �����ϱ����� ������ü�� �����۳�.
	if(FAILED(DirectInput8Create(hInst, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&m_pInputSDK, NULL)))
		return E_FAIL;	

	// �� ��ǻ�Ϳ� ����Ǿ��ִ� Ű���尴ü�� �����ϳ�.
	if(FAILED(m_pInputSDK->CreateDevice(GUID_SysKeyboard, &m_pKeyBoard, NULL)))
		return E_FAIL;

	// �������� Ű���� ��ü�� ���� ������ �����Ѵ�.
	m_pKeyBoard->SetDataFormat(&c_dfDIKeyboard);

	// ������ Ű������ ���� ������ �����ϳ�.
	m_pKeyBoard->SetCooperativeLevel(hWnd, DISCL_BACKGROUND | DISCL_NONEXCLUSIVE);

	// ������ Ű���带 Ȱ��ȭ��Ų��.
	m_pKeyBoard->Acquire();

	// �� ��ǻ�Ϳ� ����Ǿ��ִ� Ű���尴ü�� �����ϳ�.
	if(FAILED(m_pInputSDK->CreateDevice(GUID_SysMouse, &m_pMouse, NULL)))
		return E_FAIL;

	// �������� Ű���� ��ü�� ���� ������ �����Ѵ�.
	m_pMouse->SetDataFormat(&c_dfDIMouse);

	// ������ Ű������ ���� ������ �����ϳ�.
	m_pMouse->SetCooperativeLevel(hWnd, DISCL_BACKGROUND | DISCL_NONEXCLUSIVE);

	// ������ Ű���带 Ȱ��ȭ��Ų��.
	m_pMouse->Acquire();

	return S_OK;
}

void Engine::CInput_Device::SetUp_InputState()
{
	m_pKeyBoard->GetDeviceState(256, m_byKeyState);
	m_pMouse->GetDeviceState(sizeof(m_MouseState), &m_MouseState);
}

void Engine::CInput_Device::Free(void)
{
	Engine::Safe_Release(m_pKeyBoard);
	Engine::Safe_Release(m_pMouse);
	Engine::Safe_Release(m_pInputSDK);
}

