#pragma region Includes
#include "stdafx.h"
#include "resource.h"
#include "About.h"
#include "Globals.h"
#pragma endregion

using namespace About;

namespace SysTray
{
	// sysTrayGuid	{8D511904-FF1A-4741-A992-CCB587804570}
	static const GUID sysTrayGuid = { 0x8d511904, 0xff1a, 0x4741, { 0xa9, 0x92, 0xcc, 0xb5, 0x87, 0x80, 0x45, 0x70 } };
	static int currentIconFlags = -1;
	#define		SYSTRAY_ICON_ID	0x131086
	
	enum Keys
	{
		None		= 0,
		Num_Lock	= 1,
		Caps_Lock	= 2,
		Scroll_Lock	= 4
	};

	HICON GetIcon(int keys)
	{
		switch (keys)
		{
			case Keys::None:
				//	0x000
				return LoadIcon(Global::hInst, MAKEINTRESOURCE(IDI_ICON_000));
			case Keys::Num_Lock:
				//	0x001
				return LoadIcon(Global::hInst, MAKEINTRESOURCE(IDI_ICON_001));
			case Keys::Num_Lock | Keys::Caps_Lock:
				//	0x011
				return LoadIcon(Global::hInst, MAKEINTRESOURCE(IDI_ICON_011));
			case Keys::Num_Lock | Keys::Scroll_Lock:
				//	0x101
				return LoadIcon(Global::hInst, MAKEINTRESOURCE(IDI_ICON_101));
			case Keys::Num_Lock | Keys::Caps_Lock | Keys::Scroll_Lock:
				//	0x111
				return LoadIcon(Global::hInst, MAKEINTRESOURCE(IDI_ICON_111));
			case Keys::Caps_Lock:
				//	0x010
				return LoadIcon(Global::hInst, MAKEINTRESOURCE(IDI_ICON_010));
			case Keys::Caps_Lock | Keys::Scroll_Lock:
				//	0x110
				return LoadIcon(Global::hInst, MAKEINTRESOURCE(IDI_ICON_110));
			case Keys::Scroll_Lock:
				//	0x100
				return LoadIcon(Global::hInst, MAKEINTRESOURCE(IDI_ICON_100));
		}

		// Compiler
		return LoadIcon(Global::hInst, MAKEINTRESOURCE(IDI_ICON_000));
	}

	int GetIconFlags()
	{
		int keys = Keys::None;

		if (0x01 & GetKeyState(VK_NUMLOCK)) keys |= Keys::Num_Lock;
		if (0x01 & GetKeyState(VK_CAPITAL)) keys |= Keys::Caps_Lock;
		if (0x01 & GetKeyState(VK_SCROLL))	keys |= Keys::Scroll_Lock;

		return keys;
	}

	bool ChangeSysTrayIcon(HWND hWnd)
	{
		if (GetIconFlags() != currentIconFlags)
		{
			NOTIFYICONDATA nid = {sizeof(nid)};
			nid.hWnd = hWnd;
			nid.uID = SYSTRAY_ICON_ID;
			nid.uCallbackMessage = APP_MSG_TRAY;
			nid.uVersion = NOTIFYICON_VERSION_4;

			// Icon
			currentIconFlags = GetIconFlags();
			nid.hIcon = GetIcon(currentIconFlags);

			// Flags
			nid.uFlags = NIF_ICON;
			nid.dwInfoFlags = NIIF_USER;

			// Show the notification.
			Shell_NotifyIcon(NIM_MODIFY, &nid) ? S_OK : E_FAIL;
		}

		return true;
	}

	bool CreateSysTray(HWND hWnd)
	{
		NOTIFYICONDATA nid = {sizeof(nid)};
		nid.hWnd = hWnd;
		nid.uID = SYSTRAY_ICON_ID;
		nid.uCallbackMessage = APP_MSG_TRAY;
		nid.uVersion = NOTIFYICON_VERSION_4;

		// This text will be shown as the icon's tooltip.
		StringCchCopy(nid.szTip, ARRAYSIZE(nid.szTip), L"Keyboard Lights");

		// Load the icon for high DPI.
		// LoadIconMetric(Global::hInst, MAKEINTRESOURCE(IDI_SMALL), LIM_SMALL, &(nid.hIcon));
		currentIconFlags = GetIconFlags();
		nid.hIcon = GetIcon(currentIconFlags);

		// Flags
		nid.uFlags = NIF_ICON | NIF_TIP | NIF_MESSAGE;
		nid.dwInfoFlags = NIIF_USER;

		// Show the notification.
		Shell_NotifyIcon(NIM_ADD, &nid) ? S_OK : E_FAIL;
		Shell_NotifyIcon(NIM_SETVERSION, &nid) ? S_OK : E_FAIL;

		return true;
	}

	bool RemoveSysTray(HWND hWnd)
	{
		NOTIFYICONDATA nid = {sizeof(nid)};
		nid.hWnd = hWnd;
		nid.uID = SYSTRAY_ICON_ID;

		// Show the notification.
		Shell_NotifyIcon(NIM_DELETE, &nid) ? S_OK : E_FAIL;

		return true;
	}
}