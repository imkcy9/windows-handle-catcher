// HookDll.h : HookDll DLL ����ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CHookDllApp
// �йش���ʵ�ֵ���Ϣ������� HookDll.cpp
//
#define DllExport __declspec(dllexport)

//������Ϣ�ص�
typedef void (*KeyBoardHookExCallBack)( int nCode,WPARAM wParam,LPARAM lParam );
DllExport int WINAPI SetKeyBoardHookExCallBack(KeyBoardHookExCallBack keyBoardHookExCB);

//�����Ϣ�ص�
typedef void (*MouseHookExCallBack)(int nCode,WPARAM wParam,LPARAM lParam);
DllExport int WINAPI SetMouseHookExCallBack(MouseHookExCallBack mouseHookExCB);

//���Բ���ϵͳ�����Ĺ���
DllExport int WINAPI InstallKeyBoardHookEx();
DllExport int WINAPI UnInstallKeyBoardHookEx();

//��깳��
DllExport int WINAPI InstallMouseHookEx();
DllExport int WINAPI UnInstallMouseHookEx();

class CHookDllApp : public CWinApp
{
public:
	CHookDllApp();

// ��д
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};

extern HHOOK g_hook;
extern HHOOK g_hookex;
extern HHOOK g_mouseHookEx;

extern KeyBoardHookExCallBack g_keyBoardHookExCB;
extern MouseHookExCallBack g_mouseHookExCB;
