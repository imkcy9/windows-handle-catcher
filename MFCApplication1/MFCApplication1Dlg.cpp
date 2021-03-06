
// MFCApplication1Dlg.cpp: 实现文件
//

#include "stdafx.h"
#include "MFCApplication1.h"
#include "MFCApplication1Dlg.h"
#include "afxdialogex.h"
#include "TCPHelp.h"
#include "RegExpHelp.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CMFCApplication1Dlg 对话框


HWND g_hDisplayWnd = NULL;
HWND g_hMainHwnd = NULL;

CMFCApplication1Dlg::CMFCApplication1Dlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_MFCAPPLICATION1_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	tcpHelper = NULL;
	regExpHelp = new RegExpHelp();
	hwndBeMonitored = NULL;
}

void CMFCApplication1Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_HOTKEY1, hwndCaptureHotkey);
	DDX_Control(pDX, IDC_EDIT1, inputText);
	DDX_Control(pDX, IDC_LIST_HWND, hwndListControl);
	DDX_Control(pDX, IDC_HOTKEY2, sendStringHotKey);
	DDX_Control(pDX, IDC_HOTKEY_SHOW_HIDE, appHotKeyShowHide);
	DDX_Control(pDX, IDC_IPADDRESS2, ipTextInput);
	DDX_Control(pDX, IDC_BUTTON_BIND_OR_CONNECT, buttonBindOrConnect);
	DDX_Control(pDX, IDC_EDIT_REGEXP, regexpInput);
	DDX_Control(pDX, IDC_EDIT_REGEXP_GET_SEARCH_INDEX, regexpSearchIndexInput);
	DDX_Control(pDX, IDC_LIST_TCP_LOG, listTcpLog);
	DDX_Control(pDX, IDC_STATIC_ERROR_INFO, staticTextErrorInfo);
	DDX_Control(pDX, IDC_STATIC_MONITOR_HWND, staticTextMonitorHWND);
}

BEGIN_MESSAGE_MAP(CMFCApplication1Dlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_EN_CHANGE(IDC_EDIT1, &CMFCApplication1Dlg::OnEnChangeEdit1)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDCANCEL, &CMFCApplication1Dlg::OnBnClickedCancel)
	ON_BN_CLICKED(IDOK, &CMFCApplication1Dlg::OnBnClickedOk)
	ON_WM_LBUTTONDBLCLK()
	ON_BN_CLICKED(IDC_CAPTURE, &CMFCApplication1Dlg::OnBnClickedCapture)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()

	ON_MESSAGE(WM_HOTKEY, &CMFCApplication1Dlg::OnHotKey)

	ON_BN_CLICKED(IDC_CHECK_AUTO_CAPTURE_TITLE, &CMFCApplication1Dlg::OnBnClickedCheckAutoCaptureTitle)
	ON_NOTIFY(NM_RCLICK, IDC_LIST_HWND, &CMFCApplication1Dlg::OnNMRClickListHwnd)
	ON_COMMAND(ID_32771, &CMFCApplication1Dlg::On32771)
	ON_BN_CLICKED(IDC_CHECK_SEND_HOT_KEY, &CMFCApplication1Dlg::OnBnClickedCheckSendHotKey)
	ON_BN_CLICKED(IDC_CHECK_ENABLE_HOTKEY_SHOW_HIDE, &CMFCApplication1Dlg::OnBnClickedCheckEnableHotkeyShowHide)
	
	ON_BN_CLICKED(IDC_BUTTON_BIND_OR_CONNECT, &CMFCApplication1Dlg::OnBnClickedButtonBindOrConnect)
	ON_BN_CLICKED(IDC_RADIO_SERVER, &CMFCApplication1Dlg::OnBnClickedRadioServer)
	ON_BN_CLICKED(IDC_RADIO_CLIENT, &CMFCApplication1Dlg::OnBnClickedRadioClient)
	ON_WM_SHOWWINDOW()

	ON_BN_CLICKED(IDC_CHECK_UPPER_CASE, &CMFCApplication1Dlg::OnBnClickedCheckUpperCase)

	ON_MESSAGE(WM_RECV_SUB_MSG, &CMFCApplication1Dlg::OnSubMsgHandler)
	ON_MESSAGE(WM_RECV_MOUSE_GET_TITLE, &CMFCApplication1Dlg::OnMouseGetTitleHandler)
	ON_MESSAGE(WM_RECV_TCP_LOG, &CMFCApplication1Dlg::OnRecvTcpLog)

	ON_BN_CLICKED(IDC_CHECK_ENABLE_REGEXP, &CMFCApplication1Dlg::OnBnClickedCheckEnableRegexp)
	ON_BN_CLICKED(IDC_BUTTON_UNBIND_OR_DISCONNECT, &CMFCApplication1Dlg::OnBnClickedButtonUnbindOrDisconnect)
	ON_COMMAND(ID_32772, &CMFCApplication1Dlg::OnSetMonitorHWND)
	ON_BN_CLICKED(IDC_BUTTON_DELETE_MONITOR_HWND, &CMFCApplication1Dlg::OnBnClickedButtonDeleteMonitorHwnd)
	ON_WM_DESTROY()
END_MESSAGE_MAP()


// CMFCApplication1Dlg 消息处理程序

BOOL CMFCApplication1Dlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	//this->SetTimer(100, 5000, NULL);
	// 注册热键
	//::RegisterHotKey(GetSafeHwnd(), WM_HOTKEY, MOD_ALT, VK_F12);

	inputText.ModifyStyle(0, ES_UPPERCASE);
	regexpInput.SetWindowText(_T("(.*): *(.*) *\\((.*)\\) *B: ?(.*) ?A: ?(.*)"));
	SetDlgItemInt(IDC_EDIT_REGEXP_GET_SEARCH_INDEX, 1);
	((CButton*)GetDlgItem(IDC_CHECK_UPPER_CASE))->SetCheck(true);

	g_hDisplayWnd = inputText.m_hWnd;
	g_hMainHwnd = this->m_hWnd;
	int id = inputText.GetDlgCtrlID();
	((CButton *)GetDlgItem(IDC_RADIO_WM_CHAR))->SetCheck(true);
	((CButton *)GetDlgItem(IDC_CHECK_ENSURE_SEND))->SetCheck(false);
	((CButton *)GetDlgItem(IDC_CHECK_ENTER_AT_THE_END))->SetCheck(true);
	

	//long Styles;
	//Styles = GetWindowLong(hwndListControl.m_hWnd, GWL_STYLE);//获取窗口风格
	//SetWindowLong(hwndListControl.m_hWnd, GWL_STYLE, Styles | LVS_REPORT); //设置新的窗口风格

	LONG    lStyle = GetWindowLong(hwndListControl.m_hWnd, GWL_STYLE);    //获取当前窗口style
	lStyle &= ~LVS_TYPEMASK;    //清除显示方式位       
	lStyle |= LVS_REPORT;        //设置style       
	SetWindowLong(hwndListControl.m_hWnd, GWL_STYLE, lStyle);            //设置style      

	DWORD dwStyle = hwndListControl.GetExtendedStyle();
	dwStyle |= LVS_EX_FULLROWSELECT;    //选中某行使整行高亮（只适用与report风格的listctrl）       
	dwStyle |= LVS_EX_GRIDLINES;        //网格线（只适用与report风格的listctrl）       
	//dwStyle |= LVS_EX_CHECKBOXES;        //item前生成checkbox控件       
	hwndListControl.SetExtendedStyle(dwStyle);    //设置扩展风格 


	//hwndListControl.SetExtendedStyle(Styles | LVS_EX_FULLROWSELECT | LVS_REPORT);

	// 设置默认显示/隐藏快捷键
	appHotKeyShowHide.SetHotKey('1', HOTKEYF_ALT);
	((CButton*)GetDlgItem(IDC_CHECK_ENABLE_HOTKEY_SHOW_HIDE))->SetCheck(true);
	OnBnClickedCheckEnableHotkeyShowHide();

	sendStringHotKey.SetHotKey('2', HOTKEYF_ALT);
	((CButton*)GetDlgItem(IDC_CHECK_SEND_HOT_KEY))->SetCheck(true);
	OnBnClickedCheckSendHotKey();

	hwndCaptureHotkey.SetHotKey(VK_F10, HOTKEYF_ALT);
	((CButton*)GetDlgItem(IDC_CAPTURE))->SetCheck(true);
	OnBnClickedCapture();

	//插入标题	
	CString head[] = { TEXT("句柄"), TEXT("标题"), TEXT("ID"), TEXT("类") };
	//插入列	
	hwndListControl.InsertColumn(0, head[0], LVCFMT_LEFT, 100);
	hwndListControl.InsertColumn(1, head[1], LVCFMT_LEFT, 80);
	hwndListControl.InsertColumn(2, head[2], LVCFMT_LEFT, 50);
	hwndListControl.InsertColumn(3, head[3], LVCFMT_LEFT, 80);

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CMFCApplication1Dlg::OnTimer(UINT_PTR nIDEvent) {
	
}

void CMFCApplication1Dlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CMFCApplication1Dlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

BOOL CMFCApplication1Dlg::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_ESCAPE)
		return TRUE;

	return CDialogEx::PreTranslateMessage(pMsg);
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CMFCApplication1Dlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CMFCApplication1Dlg::OnEnChangeEdit1()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
}


void CMFCApplication1Dlg::OnBnClickedCancel()
{
	// TODO: 在此添加控件通知处理程序代码
	CDialogEx::OnCancel();
}


void CMFCApplication1Dlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	//CDialogEx::OnOK();

	CString text;
	GetDlgItemText(IDC_EDIT1, text);

	std::map<CString, CWnd*>::iterator it = itemsToSendMessage.begin();

	int ensureSendCheck = ((CButton *)GetDlgItem(IDC_CHECK_ENSURE_SEND))->GetCheck();
	if (ensureSendCheck) {
		CString message;
		message.Format(_T("发送字符串: %s 到所有句柄？"), text);
		if (MessageBox(message, _T("确认"), MB_YESNO) != IDYES) {
			//cout << "sss";
			return;
		}
	}

	if (tcpHelper) {
		std::string str(CT2A(text.GetString()));
		tcpHelper->publish((void*)str.c_str(), str.length());
	}

	while (it != itemsToSendMessage.end()) {
		CWnd* edit = it->second;

		
		if (IsWindow(edit->GetSafeHwnd())) {
			//edit->SendMessage(WM_SETTEXT, WPARAM('A'), (LPARAM)(LPCTSTR)text);
			int checked = ((CButton *)GetDlgItem(IDC_RADIO_WM_CHAR))->GetCheck();
			if (checked) {
				// 模拟键盘输入
				edit->SendMessage(WM_SETTEXT, WPARAM('A'), (LPARAM)(LPCTSTR)_T(""));
				int length = text.GetLength();
				for (int i = 0; i != length; i++) {
					WCHAR ch = text.GetAt(i);
					edit->SendMessage(WM_CHAR, WPARAM(ch), 0);
				}

				
			}
			else {
				edit->SendMessage(WM_SETTEXT, WPARAM('A'), (LPARAM)(LPCTSTR)text);
			}

			int enterAtTheEnd = ((CButton *)GetDlgItem(IDC_CHECK_ENTER_AT_THE_END))->GetCheck();
			if (enterAtTheEnd) {
				edit->PostMessage(WM_KEYDOWN, VK_RETURN, 0);
				edit->PostMessage(WM_CHAR, VK_RETURN, 0);
				edit->PostMessage(WM_KEYUP, VK_RETURN, 0);
			}

			it++;
		}
		else {
			it = itemsToSendMessage.erase(it);
		}
		

	}

	it = buttonsToSendMessage.begin();
	while (it != buttonsToSendMessage.end()) {
		CWnd* button = it->second;
		if (IsWindow(button->GetSafeHwnd())) {
			CButton* pBtn = (CButton*)GetDlgItem(IDC_CHECK_ENTER_AFTER_STR);
			int enterInTheEnd = pBtn->GetCheck();
			if (text.GetLength() > 0 && enterInTheEnd) {
				button->SendMessage(BM_CLICK);
			}

			it++;
		}
		else {
			it = buttonsToSendMessage.erase(it);
		}
	}


	focusAndSelectInputTextEdit();
}


void CMFCApplication1Dlg::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	CDialogEx::OnLButtonDblClk(nFlags, point);
}

LRESULT CMFCApplication1Dlg::OnSubMsgHandler(WPARAM wParam, LPARAM lParam)
{
	CString* subText = (CString*)lParam;
	inputText.SetWindowText(*subText);
	OnBnClickedOk();
	return LRESULT();
}

LRESULT CMFCApplication1Dlg::OnMouseGetTitleHandler(WPARAM wParam, LPARAM lParam) {
	std::string* subText = (std::string*)lParam;
	//CString* subText = (CString*)lParam;
	//wchar_t* pChar = (wchar_t*)lParam;
	//CString subText(pChar);

	CButton* pBtn = (CButton*)GetDlgItem(IDC_CHECK_ENABLE_REGEXP);
	int status = pBtn->GetCheck();
	if (status) {
		CString regexpPattern;
		regexpInput.GetWindowText(regexpPattern);
		int searchIndex = GetDlgItemInt(IDC_EDIT_REGEXP_GET_SEARCH_INDEX);
		//regexpSearchIndexInput.GetWindowText()
		//std::string str(CW2A(subText->GetString()));
		std::string strPattern(CW2A(regexpPattern.GetString()));
		std::string str;
		CString cstring = _T("");
		try {
			str = regExpHelp->getSearchIndexResult(*subText, strPattern, searchIndex);
		}
		catch (std::regex_error& e) {
			printf("regex_error %s", e.what());
			inputText.SendMessage(WM_SETTEXT, WPARAM('A'), (LPARAM)(LPCTSTR)_T(""));
			cstring = CA2T(e.what());
			staticTextErrorInfo.SetWindowText(cstring);
			return LRESULT();
		}

		
		cstring = CA2T(str.c_str());
		inputText.SendMessage(WM_SETTEXT, WPARAM('A'), (LPARAM)(LPCTSTR)cstring);
	}
	else {
		CString cstring;
		cstring = CA2T(subText->c_str());
		inputText.SendMessage(WM_SETTEXT, WPARAM('A'), (LPARAM)(LPCTSTR)cstring);
	}
	
	return LRESULT();
}

LRESULT CMFCApplication1Dlg::OnRecvTcpLog(WPARAM wparam, LPARAM lparam)
{
	std::string* log = (std::string*)lparam;

	CString cstring;
	cstring = CA2T(log->c_str());
	//listTcpLog.AddString(cstring);
	listTcpLog.InsertString(0, cstring);
	//listTcpLog.SetItemData(0, wparam);

	delete log;
	return LRESULT();
}

static  WORD Modifiers_HKCtrl_to_RegisterHK(WORD wSource)
{
	//#define HOTKEYF_SHIFT           0x01  
	//#define HOTKEYF_CONTROL         0x02  
	//#define HOTKEYF_ALT             0x04  
	//#define MOD_ALT         0x0001  
	//#define MOD_CONTROL     0x0002  
	//#define MOD_SHIFT       0x0004  
	//#define MOD_WIN         0x0008  
	WORD wRet = wSource;
	wRet &= ~HOTKEYF_EXT;       //过滤多余的可能多加的值0x1000  
	if (wSource & HOTKEYF_ALT && wSource & HOTKEYF_SHIFT)
	{
		wSource &= ~HOTKEYF_ALT;
		wSource &= ~HOTKEYF_SHIFT;

		wRet |= MOD_ALT;
		wRet |= MOD_SHIFT;
	}
	else
	{
		if (wSource & HOTKEYF_ALT)
		{
			wRet &= ~HOTKEYF_ALT;
			wRet |= MOD_ALT;
		}
		if (wSource & HOTKEYF_SHIFT)
		{
			wRet &= ~HOTKEYF_SHIFT;
			wRet |= MOD_SHIFT;
		}
	}

	return wRet;
}

static  WORD Modifiers_RegisterHK_to_HKCtrl(WORD wSource)
{
	//#define HOTKEYF_SHIFT           0x01  
	//#define HOTKEYF_CONTROL         0x02  
	//#define HOTKEYF_ALT             0x04  
	//#define MOD_ALT         0x0001  
	//#define MOD_CONTROL     0x0002  
	//#define MOD_SHIFT       0x0004  
	//#define MOD_WIN         0x0008  
	WORD wRet = wSource;
	if (wSource & MOD_ALT && wSource & MOD_SHIFT)
	{
		wRet &= ~MOD_ALT;
		wRet &= ~MOD_SHIFT;

		wRet |= HOTKEYF_ALT;
		wRet |= HOTKEYF_SHIFT;
	}
	else
	{
		if (wSource & MOD_ALT)
		{
			wRet &= ~MOD_ALT;
			wRet |= HOTKEYF_ALT;
		}
		if (wSource & MOD_SHIFT)
		{
			wRet &= ~MOD_SHIFT;
			wRet |= HOTKEYF_SHIFT;
		}
	}
	return wRet;
}

void CMFCApplication1Dlg::focusAndSelectInputTextEdit()
{
	inputText.SetFocus();
	inputText.SetSel(0,-1);
}

void CMFCApplication1Dlg::OnBnClickedCapture()
{
	// TODO: 在此添加控件通知处理程序代码
	CButton* pBtn = (CButton*)GetDlgItem(IDC_CAPTURE);
	int state = pBtn->GetCheck();
	WORD vkey, fsModifiers;
	hwndCaptureHotkey.GetHotKey(vkey, fsModifiers);
	hwndCaptureHotkey.GetHotKey();
	if (state == 1) {
		BOOL success = ::RegisterHotKey(GetSafeHwnd(), IDC_CAPTURE, Modifiers_HKCtrl_to_RegisterHK(fsModifiers), vkey);
		if (success) {
			hwndCaptureHotkey.EnableWindow(false);
		}
		else {
			pBtn->SetCheck(!state);
		}
	}
	else {
		hwndCaptureHotkey.GetHotKey();
		::UnregisterHotKey(GetSafeHwnd(), IDC_CAPTURE);
		hwndCaptureHotkey.EnableWindow(true);
		//ReleaseCapture();
		
	}
}

void CMFCApplication1Dlg::OnBnClickedCheckSendHotKey()
{
	// TODO: 在此添加控件通知处理程序代码
	CButton* pBtn = (CButton*)GetDlgItem(IDC_CHECK_SEND_HOT_KEY);
	int state = pBtn->GetCheck();
	WORD vkey, fsModifiers;
	sendStringHotKey.GetHotKey(vkey, fsModifiers);
	sendStringHotKey.GetHotKey();
	if (state == 1) {

		BOOL success = ::RegisterHotKey(GetSafeHwnd(), IDC_CHECK_SEND_HOT_KEY, Modifiers_HKCtrl_to_RegisterHK(fsModifiers), vkey);
		if (success) {
			sendStringHotKey.EnableWindow(false);
		}
		else {
			pBtn->SetCheck(!state);
		}
		//SetCapture();
	}
	else {
		sendStringHotKey.GetHotKey();
		::UnregisterHotKey(GetSafeHwnd(), IDC_CHECK_SEND_HOT_KEY);
		sendStringHotKey.EnableWindow(true);
		//ReleaseCapture();

	}
}



void CMFCApplication1Dlg::OnBnClickedCheckEnableHotkeyShowHide()
{
	// TODO: 在此添加控件通知处理程序代码
	CButton* pBtn = (CButton*)GetDlgItem(IDC_CHECK_ENABLE_HOTKEY_SHOW_HIDE);
	int state = pBtn->GetCheck();
	WORD vkey, fsModifiers;
	appHotKeyShowHide.GetHotKey(vkey, fsModifiers);
	appHotKeyShowHide.GetHotKey();
	if (state == 1) {

		BOOL success = ::RegisterHotKey(GetSafeHwnd(), IDC_CHECK_ENABLE_HOTKEY_SHOW_HIDE, Modifiers_HKCtrl_to_RegisterHK(fsModifiers), vkey);
		if (success) {
			appHotKeyShowHide.EnableWindow(false);
		}
		else {
			pBtn->SetCheck(!state);
		}
	}
	else {
		appHotKeyShowHide.GetHotKey();
		::UnregisterHotKey(GetSafeHwnd(), IDC_CHECK_ENABLE_HOTKEY_SHOW_HIDE);
		appHotKeyShowHide.EnableWindow(true);
		//ReleaseCapture();

	}
}


void CMFCApplication1Dlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	CDialogEx::OnLButtonDown(nFlags, point);
}


void CMFCApplication1Dlg::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	CDialogEx::OnLButtonUp(nFlags, point);
}


LRESULT  CMFCApplication1Dlg::OnHotKey(WPARAM wParam, LPARAM lParam)
{
	if (wParam == IDC_CAPTURE) {
		CPoint point;//定义一个用于确定光标位置的位置     
		GetCursorPos(&point);//获取当前光标的位置，以便使得菜单可以跟随光标  
		HWND pointHwnd = ::WindowFromPoint(point);

		if (NULL == pointHwnd) {
			return 0;
		}

		// 只取值本进程以外的句柄
		DWORD targetPID, targetTID;
		targetTID = GetWindowThreadProcessId(pointHwnd, &targetPID);

		DWORD currentPID, currentTID;
		currentTID = GetWindowThreadProcessId(this->m_hWnd, &currentPID);

		if (targetPID == currentPID) {

			return 0;
		}

		CString text;
		::SendMessage(pointHwnd, WM_GETTEXT, 100, (LPARAM)(LPCTSTR)text);
		//::SendMessage(g_hDisplayWnd, WM_SETTEXT, WPARAM('A'), (LPARAM)(LPCTSTR)text);


		int nID = ::GetDlgCtrlID(pointHwnd);//hControlWnd->GetDlgCtrlID();

		CWnd* edit = CWnd::FromHandle(pointHwnd); //wnd->GetDlgItem(nID);

		if (NULL == edit) {
			return 0;
		}
		//edit = edit->GetDlgItem(nID);

		CString sID;
		sID.Format(_T("%d"), nID);
		//this->GetDlgItem(IDC_EDIT_ID)->SetWindowText(sID);

		CString sHWND;
		sHWND.Format(_T("%d"), edit);
		//this->GetDlgItem(IDC_EDIT_HWND)->SetWindowText(sHWND);


		//this->GetDlgItem(IDC_EDIT_TITLE)->SetWindowText(text);


		WCHAR className[100];
		::GetClassName(edit->GetSafeHwnd(), (LPWSTR)className, sizeof(className));
		//this->GetDlgItem(IDC_EDIT_CLASS)->SetWindowText(className);

		// add hwnd
		if (NULL != edit) {
			WCHAR className[100];
			::GetClassName(edit->GetSafeHwnd(), (LPWSTR)className, sizeof(className));

			bool addSuccessful = false;
			if (wcscmp(className, _T("Button")) == 0) {
				if (buttonsToSendMessage.insert(std::make_pair(sHWND, edit)).second == true) {
					addSuccessful = true;
					CString sHWND;
					sHWND.Format(_T("%d"), edit);
				}
			}
			else {
				if (itemsToSendMessage.insert(std::make_pair(sHWND, edit)).second == true) {
					addSuccessful = true;
					CString sHWND;
					sHWND.Format(_T("%d"), edit);
				}
			}

			if (addSuccessful) {
				// add list control

				int index = hwndListControl.GetItemCount();
				//确定行		
				hwndListControl.InsertItem(index, sHWND);
				//hwndListControl.SetItemData(index, (DWORD_PTR)edit);
				//设置列		
				int j = 0;
				hwndListControl.SetItemText(index, ++j, text);
				hwndListControl.SetItemText(index, ++j, sID);
				hwndListControl.SetItemText(index, ++j, className);


			}
		}
	}

	if (wParam == IDC_CHECK_SEND_HOT_KEY) {
		if (hwndBeMonitored) {

			if (!IsWindow(hwndBeMonitored->m_hWnd)) {
				MessageBox(_T("监控句柄不存在"));

				return 0;
			}

			CString text;
			//SendMessage(hwndBeMonitored->GetSafeHwnd(), WM_GETTEXT, 100, (LPARAM)(LPCTSTR)text);
			hwndBeMonitored->SendMessage(WM_GETTEXT, 100, (LPARAM)(LPCTSTR)text);
			std::string str(CW2A(text.GetString()));

			SendMessage(WM_RECV_MOUSE_GET_TITLE, WPARAM('A'), (LPARAM)&str);

			OnBnClickedOk();
		}
		else {
			OnBnClickedOk();
		}
	}
	
	if (wParam == IDC_CHECK_ENABLE_HOTKEY_SHOW_HIDE) {
		if (this->IsWindowVisible()) {
			this->ShowWindow(0);
		}
		else {
			this->ShowWindow(1);
		}
	}

	return 0;
}


void mouseproc(int code, WPARAM wparam, LPARAM lparam)
{
	if (wparam == WM_MOUSEMOVE)        //只处理WM_MOUSEMOVE消息
	{
		MOUSEHOOKSTRUCT *mhookstruct;   //鼠标HOOK结构体
		mhookstruct = (MOUSEHOOKSTRUCT*)lparam;
		POINT pt = mhookstruct->pt;
		//将当前鼠标坐标点的x，y坐标作为参数向主程序窗口发送消息

		HWND pointHwnd = WindowFromPoint(pt);
		
		CString text;
		SendMessage(pointHwnd, WM_GETTEXT, 100, (LPARAM)(LPCTSTR)text);
		std::string str(CW2A(text.GetString()));

		SendMessage(g_hMainHwnd, WM_RECV_MOUSE_GET_TITLE, WPARAM('A'), (LPARAM)&str);
		
		
		//SendMessage(g_hMainHwnd, WM_RECV_MOUSE_GET_TITLE, WPARAM('A'), (LPARAM)(LPCTSTR)text);

		//SendMessage(g_hDisplayWnd, WM_SETTEXT, WPARAM('A'), (LPARAM)(LPCTSTR)text);

		//text.ReleaseBuffer();
	}

}

void CMFCApplication1Dlg::OnBnClickedCheckAutoCaptureTitle()
{
	// TODO: 在此添加控件通知处理程序代码
	CButton* pBtn = (CButton*)GetDlgItem(IDC_CHECK_AUTO_CAPTURE_TITLE);
	int state = pBtn->GetCheck();

	if (state == 1) {
		InstallMouseHookEx();
		SetMouseHookExCallBack(mouseproc);
	}
	else {
		UnInstallMouseHookEx();
		//hwndCaptureHotkey.GetHotKey();
	}
}





void CMFCApplication1Dlg::OnNMRClickListHwnd(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	//如果当前没有选中的行，右键菜单不显示
	POSITION pos = hwndListControl.GetFirstSelectedItemPosition();
	int CurSel = hwndListControl.GetNextSelectedItem(pos); //取得当前行号
	if (CurSel == -1)return;//如果没有选中的项目，返回

							//显示弹出菜单
	CPoint point;
	GetCursorPos(&point);

	CMenu menu;
	menu.LoadMenu(IDR_MENU1);//获取菜单的资源
	CMenu* popup = menu.GetSubMenu(0);//只获取第一个列菜单的指针
									  //弹出菜单显示
	popup->TrackPopupMenu(TPM_RIGHTBUTTON | TPM_LEFTALIGN, point.x, point.y, this);

	*pResult = 0;
}


void CMFCApplication1Dlg::On32771()
{
	// TODO: 在此添加命令处理程序代码
	POSITION pos = hwndListControl.GetFirstSelectedItemPosition();
	int nIndex = hwndListControl.GetNextSelectedItem(pos);
	//CWnd* wnd = (CWnd*)hwndListControl.GetItemData(nIndex);
	CString hwndNo = hwndListControl.GetItemText(nIndex, 0);
	itemsToSendMessage.erase(hwndNo);
	buttonsToSendMessage.erase(hwndNo);
	hwndListControl.DeleteItem(nIndex);
}


void CMFCApplication1Dlg::OnBnClickedButtonBindOrConnect()
{
	// TODO: 在此添加控件通知处理程序代码

	int serverCheck = ((CButton *)GetDlgItem(IDC_RADIO_SERVER))->GetCheck();
	int clientCheck = ((CButton *)GetDlgItem(IDC_RADIO_CLIENT))->GetCheck();

	if (!serverCheck && !clientCheck) {
		MessageBox(_T("请选择Server或Client类型"), _T("提示"), MB_OK);
		return;
	}

	//MessageBox(_T("该功能未实现"), _T("提示"),MB_OK);

	CString ip;
	CString address = _T("tcp://");
	ipTextInput.GetWindowTextW(ip);
	address.Append(ip);
	address.Append(_T(":"));

	CString port;
	((CEdit*)GetDlgItem(IDC_EDIT_PORT))->GetWindowText(port);
	address.Append(port);


	if (serverCheck) {
		if (!tcpHelper) {
			CString bindAddress = _T("tcp://*:");
			bindAddress.Append(port);
			tcpHelper = new TCPHelp(SERVER, std::string(CT2A(bindAddress.GetString())), this->m_hWnd);
		}

		if (tcpHelper->init()) {
			tcpHelper->start();
			((CButton*)GetDlgItem(IDC_BUTTON_BIND_OR_CONNECT))->EnableWindow(false);
			((CButton*)GetDlgItem(IDC_BUTTON_UNBIND_OR_DISCONNECT))->EnableWindow(true);
			((CButton *)GetDlgItem(IDC_RADIO_CLIENT))->EnableWindow(false);
		}
		else {
			delete tcpHelper;
			tcpHelper = NULL;
			((CButton*)GetDlgItem(IDC_BUTTON_BIND_OR_CONNECT))->EnableWindow(true);
		}
	}
	else {
		if (!tcpHelper) {
			tcpHelper = new TCPHelp(CLIENT, std::string(CT2A(address.GetString())), this->m_hWnd);
		}
		if (tcpHelper->init()) {
			tcpHelper->start();
			((CButton*)GetDlgItem(IDC_BUTTON_BIND_OR_CONNECT))->EnableWindow(false);
			((CButton*)GetDlgItem(IDC_BUTTON_UNBIND_OR_DISCONNECT))->EnableWindow(true);
			((CButton *)GetDlgItem(IDC_RADIO_SERVER))->EnableWindow(false);
		}
		else {
			delete tcpHelper;
			tcpHelper = NULL;
			((CButton*)GetDlgItem(IDC_BUTTON_BIND_OR_CONNECT))->EnableWindow(true);
		}
	}


}


void CMFCApplication1Dlg::OnBnClickedRadioServer()
{
	// TODO: 在此添加控件通知处理程序代码
	int state = ((CButton *)GetDlgItem(IDC_RADIO_SERVER))->GetCheck();
	if (state) {
		//ipTextInput.EnableWindow(false);
		//ipTextInput.SetWindowTextW(_T("127.0.0.1"));
		buttonBindOrConnect.SetWindowTextW(_T("绑定端口"));
		ipTextInput.EnableWindow(false);
		ipTextInput.SetWindowTextW(_T(""));
	}
	else {
		ipTextInput.EnableWindow(true);
	}
	
}


void CMFCApplication1Dlg::OnBnClickedRadioClient()
{
	// TODO: 在此添加控件通知处理程序代码
	int state = ((CButton *)GetDlgItem(IDC_RADIO_CLIENT))->GetCheck();
	if (state) {
		ipTextInput.EnableWindow(true);
		buttonBindOrConnect.SetWindowTextW(_T("连接"));
	}
	else {
		ipTextInput.EnableWindow(false);
	}
}


void CMFCApplication1Dlg::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialogEx::OnShowWindow(bShow, nStatus);
	
	if (bShow) {
		focusAndSelectInputTextEdit();
	}
}




void CMFCApplication1Dlg::OnBnClickedCheckUpperCase()
{
	// TODO: 在此添加控件通知处理程序代码
	CButton* pBtn = (CButton*)GetDlgItem(IDC_CHECK_UPPER_CASE);
	int state = pBtn->GetCheck();

	if (state) {
		inputText.ModifyStyle(0, ES_UPPERCASE);
	}
	else {
		inputText.ModifyStyle(ES_UPPERCASE, 0);
	}
}




void CMFCApplication1Dlg::OnBnClickedCheckEnableRegexp()
{
	// TODO: 在此添加控件通知处理程序代码
	CButton* pBtn = (CButton*)GetDlgItem(IDC_CHECK_ENABLE_REGEXP);
	int state = pBtn->GetCheck();

	if (state) {
		regexpInput.EnableWindow(false);
	}
	else {
		regexpInput.EnableWindow(true);
		staticTextErrorInfo.SetWindowText(_T(""));
	}
}


void CMFCApplication1Dlg::OnBnClickedButtonUnbindOrDisconnect()
{
	// TODO: 在此添加控件通知处理程序代码
	if (tcpHelper) {
		((CButton*)GetDlgItem(IDC_BUTTON_UNBIND_OR_DISCONNECT))->EnableWindow(false);
		tcpHelper->stop();
		tcpHelper->join();

		delete tcpHelper;
		tcpHelper = NULL;

		((CButton *)GetDlgItem(IDC_RADIO_SERVER))->EnableWindow(true);
		((CButton *)GetDlgItem(IDC_RADIO_CLIENT))->EnableWindow(true);
		((CButton*)GetDlgItem(IDC_BUTTON_BIND_OR_CONNECT))->EnableWindow(true);

	}
}


void CMFCApplication1Dlg::OnSetMonitorHWND()
{
	// TODO: 在此添加命令处理程序代码
	POSITION pos = hwndListControl.GetFirstSelectedItemPosition();
	int nIndex = hwndListControl.GetNextSelectedItem(pos);
	//CWnd* wnd = (CWnd*)hwndListControl.GetItemData(nIndex);
	CString hwndNo = hwndListControl.GetItemText(nIndex, 0);
	auto it = itemsToSendMessage.find(hwndNo);
	if (it == itemsToSendMessage.end()) {
		it = buttonsToSendMessage.find(hwndNo);
	}

	hwndBeMonitored = it->second;

	itemsToSendMessage.erase(hwndNo);
	buttonsToSendMessage.erase(hwndNo);
	hwndListControl.DeleteItem(nIndex);

	staticTextMonitorHWND.SetWindowText(_T("当前监控句柄：") + hwndNo);
	CButton* btn = ((CButton*)GetDlgItem(IDC_BUTTON_DELETE_MONITOR_HWND));
	btn->ShowWindow(true);
}


void CMFCApplication1Dlg::OnBnClickedButtonDeleteMonitorHwnd()
{
	// TODO: 在此添加控件通知处理程序代码
	staticTextMonitorHWND.SetWindowText(_T(""));
	CButton* btn = ((CButton*)GetDlgItem(IDC_BUTTON_DELETE_MONITOR_HWND));
	btn->ShowWindow(false);
	hwndBeMonitored = NULL;
}


void CMFCApplication1Dlg::OnDestroy()
{
	CDialogEx::OnDestroy();

	// TODO: 在此处添加消息处理程序代码
	if (regExpHelp) {
		delete regExpHelp;
		regExpHelp = NULL;
	}

	if (tcpHelper) {
		tcpHelper->stop();
		tcpHelper->join();
		delete tcpHelper;
		tcpHelper = NULL;
	}
}
