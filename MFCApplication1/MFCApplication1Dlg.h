
// MFCApplication1Dlg.h: 头文件
//

#pragma once

#include <map>

// CMFCApplication1Dlg 对话框
class TCPHelp;
class RegExpHelp;
class CMFCApplication1Dlg : public CDialogEx
{
// 构造
public:
	CMFCApplication1Dlg(CWnd* pParent = nullptr);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MFCAPPLICATION1_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持

	BOOL PreTranslateMessage(MSG* pMsg);
// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnEnChangeEdit1();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedOk();
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);

	afx_msg LRESULT OnSubMsgHandler(WPARAM, LPARAM);
	afx_msg LRESULT OnMouseGetTitleHandler(WPARAM, LPARAM);
	afx_msg LRESULT OnRecvTcpLog(WPARAM, LPARAM);
private:
	std::map<CString, CWnd*> itemsToSendMessage;
	std::map<CString, CWnd*> buttonsToSendMessage;
	// 被监控的句柄
	CWnd* hwndBeMonitored;
	void focusAndSelectInputTextEdit();

	TCPHelp* tcpHelper;
	RegExpHelp* regExpHelp;
public:
	afx_msg void OnBnClickedCapture();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg LRESULT OnHotKey(WPARAM wParam, LPARAM lParam);
	//CListBox HWND_LIST;
	//CWnd* CurrentHwndToRecord;
	CHotKeyCtrl hwndCaptureHotkey;
	CHotKeyCtrl sendStringHotKey;
	CHotKeyCtrl appHotKeyShowHide;
	afx_msg void OnBnClickedCheckAutoCaptureTitle();
	CEdit inputText;
	CListCtrl hwndListControl;
	afx_msg void OnNMRClickListHwnd(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void On32771();
	afx_msg void OnBnClickedCheckSendHotKey();

	
	afx_msg void OnBnClickedCheckEnableHotkeyShowHide();

	afx_msg void OnBnClickedButtonBindOrConnect();
	CIPAddressCtrl ipTextInput;
	afx_msg void OnBnClickedRadioServer();
	afx_msg void OnBnClickedRadioClient();
	CButton buttonBindOrConnect;
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnBnClickedCheckUpperCase();
	CEdit regexpInput;
	afx_msg void OnBnClickedCheckEnableRegexp();
	CEdit regexpSearchIndexInput;
	CListBox listTcpLog;
	CStatic staticTextErrorInfo;
	afx_msg void OnBnClickedButtonUnbindOrDisconnect();
	afx_msg void OnSetMonitorHWND();
	CStatic staticTextMonitorHWND;
	afx_msg void OnBnClickedButtonDeleteMonitorHwnd();
	afx_msg void OnDestroy();

	static bool m_bCaptureParentOnly;
	afx_msg void OnBnClickedCheckCaptureParentOnly();
};

