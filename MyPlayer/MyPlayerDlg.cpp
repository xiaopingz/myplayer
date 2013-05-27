/** MyPlayerDlg.cpp : 实现文件
*/

#include "stdafx.h"
#include "MyPlayer.h"
#include "MyPlayerDlg.h"
#include <stdio.h>
#include <time.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


///用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

/// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

/// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


/// CMyPlayerDlg 对话框




CMyPlayerDlg::CMyPlayerDlg(CWnd* pParent /**=NULL*/)
	: CDialog(CMyPlayerDlg::IDD, pParent)
	, m_pPlayer(NULL)
	, m_sliVoice(true)
	, m_sliTempo(false)
{
	m_brBlackBrush.CreateSolidBrush(RGB(0,0,0));
	m_brLBlueBrush.CreateSolidBrush(RGB(0,0,192));
	m_brLGreenBrush.CreateSolidBrush(RGB(192,192,0));
	m_brLRedBrush.CreateSolidBrush(RGB(192,0,0));
	m_brOraBrush.CreateSolidBrush(RGB(255,192,0));
	m_pCurBrush = &m_brBlackBrush;
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_bIsMax = false;
	//m_bCC = false;
	playlistDlg = NULL;
	m_strFileTitle = _T("::Video Mode");
	m_strTimeShow = _T("00:00:00|00:00:00");
	CoInitialize(NULL);
}

CMyPlayerDlg::~CMyPlayerDlg()
{
	delete		m_wndFrameImageBEX;
	delete		m_wndCloseBEX;
	delete		m_wndMaxBEX;
	delete		m_wndMinBEX;
	delete		m_wndPLBEX;
	delete		m_wndVoiceBEX;
	delete		m_wndPPBEX;
	delete		m_wndNextBEX;
	delete		m_wndPreBEX;
	delete		m_wndStopBEX;
	delete		m_ChangeColorBEX;
	CoUninitialize();
}


void CMyPlayerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CMyPlayerDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	//ON_BN_CLICKED(IDC_CLOSEBEX,&CMyPlayerDlg::OnCloseD)
	ON_BN_CLICKED(IDC_MAXBEX,&CMyPlayerDlg::OnMaximize)
	ON_BN_CLICKED(IDC_MINBEX,&CMyPlayerDlg::OnMinimize)
	ON_BN_CLICKED(IDC_PLBEX,&CMyPlayerDlg::OnShowPL)
	ON_BN_CLICKED(IDC_PPBEX,&CMyPlayerDlg::OnPlay)
	ON_BN_CLICKED(IDC_STOPBEX,&CMyPlayerDlg::OnStop)
	//ON_BN_CLICKED(IDC_CHANGECOLOR,&CMyPlayerDlg::OnChangeColor)
	//ON_STN_DBLCLK(IDC_FILETITLE,OnTitleDBLClicked)
	ON_WM_SIZE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_CTLCOLOR()
	ON_WM_TIMER()
	ON_WM_HSCROLL()
	//ON_COMMAND(IDM_MENUOPEN,OnMenuOpen)
	//ON_COMMAND(IDM_MENUFULL,OnMaximize)
	//ON_COMMAND(IDM_MENUPL,OnShowPL)
	ON_MESSAGE(WM_USER_DIALOG_DESTROYED,OnDialogDestroyed)
END_MESSAGE_MAP()


/// CMyPlayerDlg 消息处理程序

BOOL CMyPlayerDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	/// 将“关于...”菜单项添加到系统菜单中。

	/// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	/// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	///  执行此操作
	SetIcon(m_hIcon, TRUE);			/// 设置大图标
	SetIcon(m_hIcon, FALSE);		/// 设置小图标

	/// TODO: 在此添加额外的初始化代码
	///SendMessage(WM_CTLCOLOR,0,0);

	GetClientRect(&dlgRect);

///按钮控件

	///自绘标题栏图标（菜单按钮）
	m_wndFrameImageBEX = new CButtonEx(S_FRAMEIMAGE_D,S_FRAMEIMAGE_F);
	m_wndFrameImageBEX->Create(_T(" "),WS_CHILD|WS_VISIBLE|BS_NOTIFY|BS_PUSHBUTTON,
		CRect(dlgRect.left,dlgRect.top,dlgRect.left+23,dlgRect.top+23),this,IDC_FRAMEIBEX);

	///标题栏右侧最大化，最小化及关闭按钮
	m_bIsMax = false;

	m_wndCloseBEX = new CButtonEx(S_CLOSEIMAGE_D,S_CLOSEIMAGE_F);
	m_wndCloseBEX->Create(_T(" "),WS_CHILD|WS_VISIBLE|BS_NOTIFY|BS_PUSHBUTTON,
		CRect(dlgRect.right-23,dlgRect.top,dlgRect.right,dlgRect.top+23),this,IDC_CLOSEBEX);

	m_wndMaxBEX = new CButtonEx(S_MAXIMAGE_D,S_MAXIMAGE_F);
	m_wndMaxBEX->Create(_T(" "),WS_CHILD|WS_VISIBLE|BS_NOTIFY|BS_PUSHBUTTON,
		CRect(dlgRect.right-46,dlgRect.top,dlgRect.right-23,dlgRect.top+23),this,IDC_MAXBEX);

	m_wndMinBEX = new CButtonEx(S_MINIMAGE_D,S_MINIMAGE_F);
	m_wndMinBEX->Create(_T(" "),WS_CHILD|WS_VISIBLE|BS_NOTIFY|BS_PUSHBUTTON,
		CRect(dlgRect.right-69,dlgRect.top,dlgRect.right-46,dlgRect.top+23),this,IDC_MINBEX);

	m_wndPLBEX = new CButtonEx(S_PL_D,S_PL_F);
	m_wndPLBEX->Create(_T(" "),WS_CHILD|WS_VISIBLE|BS_NOTIFY|BS_PUSHBUTTON,
		CRect(dlgRect.right-50,dlgRect.bottom-54,dlgRect.right-3,dlgRect.bottom-30),this,IDC_PLBEX);

	m_wndVoiceBEX = new CButtonEx(S_VOICEBEX_D,S_VOICEBEX_F);
	m_wndVoiceBEX->Create(_T(" "),WS_CHILD|WS_VISIBLE|BS_PUSHBUTTON|BS_NOTIFY,
		CRect(dlgRect.right-113,dlgRect.bottom-25,dlgRect.right-90,dlgRect.bottom-2),this,IDC_VOICEBEX);
	
	m_wndPPBEX = new CButtonEx(S_PPBEX_D,S_PPBEX_F);
	m_wndPPBEX->Create(_T(" "),WS_CHILD|WS_VISIBLE|BS_PUSHBUTTON|BS_NOTIFY,
		CRect(dlgRect.Width()/2-27,dlgRect.bottom-54,dlgRect.Width()/2-2,dlgRect.bottom-29),this,IDC_PPBEX);

	m_wndStopBEX = new CButtonEx(S_STOPBEX_D,S_STOPBEX_F);
	m_wndStopBEX->Create(_T(" "),WS_CHILD|WS_VISIBLE|BS_PUSHBUTTON|BS_NOTIFY,
		CRect(dlgRect.Width()/2+3,dlgRect.bottom-54,dlgRect.Width()/2+28,dlgRect.bottom-29),this,IDC_STOPBEX);

	m_wndPreBEX = new CButtonEx(S_PREBEX_D,S_PREBEX_F);
	m_wndPreBEX->Create(_T(" "),WS_CHILD|WS_VISIBLE|BS_PUSHBUTTON|BS_NOTIFY,
		CRect(dlgRect.Width()/2-57,dlgRect.bottom-54,dlgRect.Width()/2-32,dlgRect.bottom-29),this,IDC_PREBEX);
	
	m_wndNextBEX = new CButtonEx(S_NEXTBEX_D,S_NEXTBEX_F);
	m_wndNextBEX->Create(_T(" "),WS_CHILD|WS_VISIBLE|BS_PUSHBUTTON|BS_NOTIFY,
		CRect(dlgRect.Width()/2+33,dlgRect.bottom-54,dlgRect.Width()/2+58,dlgRect.bottom-29),this,IDC_NEXTBEX);

	m_ChangeColorBEX = new CButtonEx(S_CHANGECOLOR_D,S_CHANGECOLOR_F);
	m_ChangeColorBEX->Create(_T(" "),WS_CHILD|WS_VISIBLE|BS_PUSHBUTTON,
		CRect(dlgRect.right-80,dlgRect.bottom-54,dlgRect.right-55,dlgRect.bottom-29),this,IDC_CHANGECOLOR);


///静态控件

	///标题栏，正在播放的文件名显示
	m_staFileTitle.Create((LPCTSTR)m_strFileTitle,WS_VISIBLE|WS_CHILD|SS_CENTER|SS_CENTERIMAGE,
		CRect(dlgRect.left+69,dlgRect.top,dlgRect.right-69,dlgRect.top+23),this,IDC_FILETITLE);
	m_staFileTitle.setCtlType(RGB(0,0,0),RGB(0,192,0),m_strFileTitle);

	///视频播放区域
	m_staVArea.Create(_T(""),WS_CHILD|WS_BORDER|WS_VISIBLE|SS_BITMAP|SS_CENTERIMAGE,
		CRect(dlgRect.left+3,dlgRect.top+27,dlgRect.right-3,dlgRect.bottom-55),this,IDC_VPAREA);
	HBITMAP hbitmap = LoadBitmap(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDB_BMP_BACKGROUND));
	m_staVArea.SetBitmap(hbitmap);
	
	m_staVArea.ModifyStyleEx(0, WS_EX_CLIENTEDGE);///给视频窗口四周增加立体边框
	m_staVArea.SetWindowPos(0, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_DRAWFRAME);

	///时间显示
	m_staTime.Create((LPCTSTR)m_strTimeShow,WS_VISIBLE|WS_CHILD|SS_CENTER,
		CRect(dlgRect.left+3,dlgRect.bottom-54,dlgRect.left+133,dlgRect.bottom-30),this,IDC_TIME);
	m_staTime.setCtlType(RGB(0,0,0),RGB(192,192,192),m_strTimeShow);

///滑杆控件

	///声音控制
	m_sliVoice.Create(WS_CHILD|WS_VISIBLE|TBS_AUTOTICKS|TBS_BOTTOM,
		CRect(dlgRect.right-90,dlgRect.bottom-25,dlgRect.right-2,dlgRect.bottom-2),this,IDC_VOICESLD);
	m_sliVoice.SetRange(0,50);
	m_sliVoice.SetPos(8);
	m_sliVoice.SetTicFreq(10);

	///播放进度控制
	m_sliTempo.Create(WS_VISIBLE|WS_CHILD|TBS_BOTTOM|TBS_HORZ|TBS_AUTOTICKS,
		CRect(dlgRect.left+3,dlgRect.bottom-25,dlgRect.right-115,dlgRect.bottom-2),this,IDC_TEMPOSLD);
	m_sliTempo.SetRange(0,1000);
	m_sliTempo.SetPos(0);

///右键菜单
	/**m_mVArea.CreatePopupMenu();
	m_mVArea.AppendMenu(MF_STRING,IDM_MENUOPEN,(LPCTSTR)_T("打开文件"));
	m_mVArea.AppendMenu(MF_STRING,IDM_MENUFULL,(LPCTSTR)_T("全屏"));
	m_mVArea.AppendMenu(MF_STRING,IDM_MENUPL,(LPCTSTR)_T("打开播放列表"));
	m_mVArea.AppendMenu(MF_STRING,IDM_MENUEXIT,(LPCTSTR)_T("退出"));
	m_mVArea.EnableMenuItem(IDM_MENUOPEN,MF_ENABLED);
	m_mVArea.EnableMenuItem(IDM_MENUFULL,MF_ENABLED);
	m_mVArea.EnableMenuItem(IDM_MENUPL,MF_ENABLED);
	m_mVArea.EnableMenuItem(IDM_MENUEXIT,MF_ENABLED);*/


	if (!SetTimer(ID_TIMER,1000,NULL))
	{
		MessageBox(_T("ERROR:SetTimer failed !"));
		return false;
	}
	return TRUE;  /// 除非将焦点设置到控件，否则返回 TRUE
}

void CMyPlayerDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

/// 如果向对话框添加最小化按钮，则需要下面的代码
///  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
///  这将由框架自动完成。

void CMyPlayerDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); /// 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		/// 使图标在工作矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		/// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		///设置播放器界面的背景色
		CPaintDC	dc(this);
		GetClientRect(&dlgRect);
	
		///此处已用WM_CTLCOLOR消息处理
		/**	CBrush		bkgBrush;
		bkgBrush.CreateSolidBrush(RGB(0,0,0));
		dc.SelectObject(&bkgBrush);
		dc.Rectangle(dlgRect.left,dlgRect.top,dlgRect.right,dlgRect.bottom);*/

		
		///标题栏窗口标题
		m_strWndTitle = _T("AXP");
		dc.SetTextColor(RGB(192,192,0));
		dc.SetBkMode(TRANSPARENT);
		dc.DrawText(m_strWndTitle,CRect(dlgRect.left+25,dlgRect.top,dlgRect.left+50,dlgRect.top+23),
			DT_VCENTER|DT_CENTER|DT_SINGLELINE);

		///重设各控件位置
		setAllPos();
		if(m_pPlayer!=NULL)
		{
			CDC* pDC = m_staVArea.GetWindowDC();
			m_pPlayer->onResetWnd((void *)&(pDC->m_hDC));
		}
		CDialog::OnPaint();
	}
}

///当用户拖动最小化窗口时系统调用此函数取得光标显示。
///
HCURSOR CMyPlayerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CMyPlayerDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	/// TODO: 在此处添加消息处理程序代码
	

	Invalidate();
}

//按窗口又上角关闭按钮，关闭窗口
//void CMyPlayerDlg::OnCloseD()
//{
//	static	bool m_bSend = false;
//	if (!m_bSend)
//	{
//		this->SendMessage(WM_CLOSE,0,0);
//		m_bSend = true;
//	}
//}

//窗口最大化
void CMyPlayerDlg::OnMaximize()
{
	if (!m_bIsMax)
	{
		ShowWindow(SW_SHOWMAXIMIZED);//这里似乎是全屏了
		m_bIsMax = true;
	}
	else
	{
		ShowWindow(SW_RESTORE);
		m_bIsMax = false;
	}
}

///窗口最小化
void CMyPlayerDlg::OnMinimize()
{
	ShowWindow(SW_SHOWMINIMIZED);
}

void CMyPlayerDlg::setAllPos()
{
	setBExPos();
	setCStaPos();
	setCSliPos();
}

void CMyPlayerDlg::setBExPos()
{
	m_wndFrameImageBEX->SetWindowPos(NULL,dlgRect.left,dlgRect.top,23,23,SWP_NOZORDER);
	m_wndCloseBEX->SetWindowPos(NULL,dlgRect.right-23,dlgRect.top,23,23,SWP_NOZORDER);
	m_wndMaxBEX->SetWindowPos(NULL,dlgRect.right-46,dlgRect.top,23,23,SWP_NOZORDER);
	m_wndMinBEX->SetWindowPos(NULL,dlgRect.right-69,dlgRect.top,23,23,SWP_NOZORDER);
	m_wndPLBEX->SetWindowPos(NULL,dlgRect.right-50,dlgRect.bottom-54,48,24,SWP_NOZORDER);
	m_wndVoiceBEX->SetWindowPos(NULL,dlgRect.right-113,dlgRect.bottom-25,23,23,SWP_NOZORDER);
	m_wndPPBEX->SetWindowPos(NULL,dlgRect.Width()/2-27,dlgRect.bottom-54,25,25,SWP_NOZORDER);
	m_wndStopBEX->SetWindowPos(NULL,dlgRect.Width()/2+3,dlgRect.bottom-54,25,25,SWP_NOZORDER);
	m_wndPreBEX->SetWindowPos(NULL,dlgRect.Width()/2-57,dlgRect.bottom-54,25,25,SWP_NOZORDER);
	m_wndNextBEX->SetWindowPos(NULL,dlgRect.Width()/2+33,dlgRect.bottom-54,25,25,SWP_NOZORDER);
	m_ChangeColorBEX->SetWindowPos(NULL,dlgRect.right-80,dlgRect.bottom-54,25,25,SWP_NOZORDER);

}

void CMyPlayerDlg::setCStaPos()
{
	m_staFileTitle.SetWindowPos(NULL,dlgRect.left+69,dlgRect.top,dlgRect.Width()-138,23,SWP_NOZORDER);
	m_staVArea.SetWindowPos(NULL,dlgRect.left+3,dlgRect.top+27,dlgRect.Width()-6,dlgRect.Height()-82,SWP_NOZORDER);
	m_staTime.SetWindowPos(NULL,dlgRect.left+3,dlgRect.bottom-54,130,24,SWP_NOZORDER);
}

void CMyPlayerDlg::setCSliPos()
{
	m_sliVoice.SetWindowPos(NULL,dlgRect.right-90,dlgRect.bottom-25,88,23,SWP_NOZORDER);
	m_sliTempo.SetWindowPos(NULL,dlgRect.left+3,dlgRect.bottom-25,dlgRect.Width()-118,23,SWP_NOZORDER);
}

///拖动“标题栏”非按钮区时，移动窗口
void CMyPlayerDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	/// TODO: 在此添加消息处理程序代码和/或调用默认值

	CRect wndMoveChRect(dlgRect.left+23,dlgRect.top,dlgRect.right-69,dlgRect.top+23);
	if(wndMoveChRect.PtInRect(point))
		PostMessage(WM_NCLBUTTONDOWN,HTCAPTION,MAKELPARAM(point.x,point.y));
	
	CDialog::OnLButtonDown(nFlags, point);
}


///双击标题栏时，最大化/还原窗口
void CMyPlayerDlg::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	/// TODO: 在此添加消息处理程序代码和/或调用默认值
	CRect wndMoveChRect(dlgRect.left+23,dlgRect.top,dlgRect.right-69,dlgRect.top+23);
	if(wndMoveChRect.PtInRect(point))
	{
		if (!m_bIsMax)
		{
			ShowWindow(SW_SHOWMAXIMIZED);
			m_bIsMax = true;
		}
		else
		{
			ShowWindow(SW_RESTORE);
			m_bIsMax = false;
		}
	}

	CDialog::OnLButtonDblClk(nFlags, point);
}


void CMyPlayerDlg::OnShowPL()
{
	if (!playlistDlg == NULL)
		playlistDlg->SetFocus();
	else{
		playlistDlg = new CPlayListDlg(this);
		playlistDlg->Create(IDD_PLAYLIST_DIALOG);
		playlistDlg->ShowWindow(SW_SHOW);
	}
}

//原本准备用这个函数响应双击标题栏时最大化/还原窗口
//但是使staFileTitle有了属性SS_NOTIFY之后，移动窗口的区域在空间内的部分会被覆盖
//void CMyPlayerDlg::OnTitleDBLClicked()
//{
//	if (!m_bIsMax)
//	{
//		ShowWindow(SW_SHOWMAXIMIZED);
//		m_bIsMax = true;
//	}
//	else
//	{
//		ShowWindow(SW_RESTORE);
//		m_bIsMax = false;
//	}
//}

LRESULT CMyPlayerDlg::OnDialogDestroyed(WPARAM wParam,LPARAM lParam)
{
	playlistDlg = NULL;
	return 0;
}

void CMyPlayerDlg::OnPlay()
{
	if ( !m_pPlayer )
	{
		m_pPlayer = new DSVideoPlay(this->m_hWnd,m_staVArea.m_hWnd);
		OnMenuOpen();
		m_pPlayer->SetSource(std::string((LPCTSTR)m_strFilePath));
		//m_pPlayer->SetSource((LPCTSTR)"L:\\hear\\Hear.2009.CHINESE.DVDRip.XviD-MaydAY.cd1.avi");
		m_pPlayer->Play();
		//m_strFileTitle = _T("Hear.2009.CHINESE.DVDRip.XviD-MaydAY.cd1.avi");
		m_staFileTitle.setCtlType(RGB(0,0,0),RGB(0,192,0),m_strFileTitle);
		//m_pPlayer->SetFullScreen();       //检查全屏问题！
	}
	else 
	{
		m_iPlayState = m_pPlayer->GetPlayState();
		switch( m_iPlayState )
		{
		case smsPaused:
		case smsStopped:
			m_pPlayer->Play();
			break;
		case smsPlaying:
			m_pPlayer->Pause();
			break;
		default:break;
		}
	}
	
}

void CMyPlayerDlg::OnStop()
{
	if ( m_pPlayer )
	{
		m_iPlayState = m_pPlayer->GetPlayState();
		if ( m_iPlayState == smsPlaying || m_iPlayState == smsPaused )
		{
			m_pPlayer->Stop();
			delete m_pPlayer;
			m_pPlayer = NULL;
			HBITMAP hbitmap = LoadBitmap(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDB_BMP_BACKGROUND));
			m_staVArea.SetBitmap(hbitmap);
		}
	}
}

HBRUSH CMyPlayerDlg::OnCtlColor( CDC* pDC, CWnd* pWnd, UINT nCtlColor )
{
	//if (!m_bCC)
		return (HBRUSH)(*m_pCurBrush);
	/*else
	{
		srand((unsigned)time(NULL));
		int iRand = rand()%5;
		switch(iRand)
		{
		case 0:
			return (HBRUSH)m_brLBlueBrush;
			break;
		case 1:
			return (HBRUSH)m_brLGreenBrush;
			break;
		case 2:
			return (HBRUSH)m_brLRedBrush;
			break;
		case 3:
			return (HBRUSH)m_brOraBrush;
			break;
		case 4:
			return (HBRUSH)m_brBlackBrush;
			break;
		}
	}*/

	//CFrameWnd::OnCtlColor(pDC,pWnd,nCtlColor);
}

//void CMyPlayerDlg::OnChangeColor()
//{
//	m_pCurBrush = &m_brLGreenBrush;
//	AfxGetMainWnd()->PostMessage(WM_CTLCOLOR,0,0);
//	//m_bCC = true;
//}

void CMyPlayerDlg::OnHScroll( UINT nSBCode, UINT nPos, CScrollBar* pScrollBar )
{
	if (((CMySliderCtrl *)pScrollBar)->isVolumeSlider())
	{
		if (m_pPlayer->IsMute()){;}
		else
		{
			nPos = ((CSliderCtrl *)pScrollBar)->GetPos();
			LONG nVolume =nPos*40-2000;
			m_pPlayer->SetVolume(nVolume);
		}
	}

	else
	{
		int sliSize = ((CMySliderCtrl *)pScrollBar)->getSize();
		nPos = ((CSliderCtrl *)pScrollBar)->GetPos();
		double rate = 1.0*nPos/sliSize;
		LONG videoTotalTime = m_pPlayer->GetLength();
		LONG videoCurTime = rate *videoTotalTime;
		m_pPlayer->SetPosition((LONG)videoCurTime);
	}
}


void CMyPlayerDlg::setShowTime( LONG curPosOfPlay )
{
	CString strTime;
	//转换文件总时间
	LONG vLength = m_pPlayer->GetLength();
	int tHour ,tMinute ,tSecond ;
	if ( vLength >= 3600 )
	{
		tHour = vLength / 3600 ;
		if( (tSecond = vLength % 3600) >= 60)
		{
			tMinute = tSecond/60;
			tSecond %=60; 
		}
	}
	else
	{
		tHour = 0 ;
		if ( vLength >= 60 )
		{
			tMinute = vLength/60;
			tSecond = vLength%60;
		}
		else
		{
			tMinute = 0 ;
			tSecond = vLength;
		}
	}
	
	//转换已播放文件的时间
	int cHour ,cMinute ,cSecond ;
	if ( curPosOfPlay >= 3600 )
	{
		cHour = curPosOfPlay / 3600 ;
		if( (cSecond = curPosOfPlay % 3600) >= 60)
		{
			cMinute = cSecond/60;
			cSecond %=60; 
		}
	}
	else
	{
		cHour = 0 ;
		if ( curPosOfPlay >= 60 )
		{
			cMinute = curPosOfPlay/60;
			cSecond = curPosOfPlay % 60;
		}
		else
		{
			cMinute = 0 ;
			cSecond = curPosOfPlay;
		}
	}
	strTime.Format(_T("%2d:%2d:%2d|%2d:%2d:%2d"),cHour,cMinute,cSecond,tHour,tMinute,tSecond);
	m_staTime.setCtlType(RGB(0,0,0),RGB(192,192,192),strTime);
}


void CMyPlayerDlg::setShowSysTime()
{
	CTime curTime = CTime::GetCurrentTime();
	int nSecond = curTime.GetSecond();
	int nMinute = curTime.GetMinute();
	int nHour = curTime.GetHour();
	CString strSysTime;
	strSysTime.Format(_T("00:00:00|%2d:%2d:%2d"),nHour,nMinute,nSecond);
	m_staTime.setCtlType(RGB(0,0,0),RGB(192,192,192),strSysTime);
}

void CMyPlayerDlg::OnTimer( UINT_PTR nIDEvent )
{
	if (!m_pPlayer || m_pPlayer->GetPlayState() == smsNull)
		setShowSysTime();
	else 
		setShowTime(m_pPlayer->GetPosition());
}



void CMyPlayerDlg::OnMenuOpen()
{
	TCHAR	strFilter[] = _T("AVI files(*.avi)|*.avi|All files(*.*)|*.*||");
	CFileDialog dlgFO(TRUE,_T("avi"),_T("*.avi"),OFN_HIDEREADONLY | OFN_PATHMUSTEXIST ,strFilter);

	if (dlgFO.DoModal() == IDOK)
	{
		m_strFileTitle = dlgFO.GetFileName();
		m_strFilePath = dlgFO.GetPathName();

	}
	Invalidate();
}