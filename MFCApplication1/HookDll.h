// HookDll.h : HookDll DLL 的主头文件
//

#pragma once

#ifndef __AFXWIN_H__
	#error "在包含此文件之前包含“stdafx.h”以生成 PCH 文件"
#endif

#include "resource.h"		// 主符号


// CHookDllApp
// 有关此类实现的信息，请参阅 HookDll.cpp
//
#define DllExport __declspec(dllexport)

//键盘消息回调
typedef void (*KeyBoardHookExCallBack)( int nCode,WPARAM wParam,LPARAM lParam );
DllExport int WINAPI SetKeyBoardHookExCallBack(KeyBoardHookExCallBack keyBoardHookExCB);

//鼠标消息回调
typedef void (*MouseHookExCallBack)(int nCode,WPARAM wParam,LPARAM lParam);
DllExport int WINAPI SetMouseHookExCallBack(MouseHookExCallBack mouseHookExCB);

//可以捕获系统按键的勾子
DllExport int WINAPI InstallKeyBoardHookEx();
DllExport int WINAPI UnInstallKeyBoardHookEx();

//鼠标钩子
DllExport int WINAPI InstallMouseHookEx();
DllExport int WINAPI UnInstallMouseHookEx();

class CHookDllApp : public CWinApp
{
public:
	CHookDllApp();

// 重写
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};

extern HHOOK g_hook;
extern HHOOK g_hookex;
extern HHOOK g_mouseHookEx;

extern KeyBoardHookExCallBack g_keyBoardHookExCB;
extern MouseHookExCallBack g_mouseHookExCB;
