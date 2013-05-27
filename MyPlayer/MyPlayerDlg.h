// MyPlayerDlg.h : ͷ�ļ�
//

#pragma once

#include "ButtonEx.h"
#include "TypeCStatic.h"
#include "PlayListDlg.h"
#include "dshow/DSVideoPlay.h"
#include "MySliderCtrl.h"

#define WM_USER_DIALOG_DESTROYED			WM_USER + 1

// CMyPlayerDlg �Ի���
class CMyPlayerDlg : public CDialog
{
// ����
public:
	CMyPlayerDlg(CWnd* pParent = NULL);	// ��׼���캯��
	virtual ~CMyPlayerDlg();
// �Ի�������
	enum { IDD = IDD_MYPLAYER_DIALOG };


// ʵ��
protected:
	HICON m_hIcon;
	bool	m_bIsMax;

public:
	CRect		dlgRect;//���ڣ��Ի��򣩿�������
	CButtonEx		*m_wndFrameImageBEX;
	CButtonEx		*m_wndCloseBEX;
	CButtonEx		*m_wndMaxBEX;
	CButtonEx		*m_wndMinBEX;
	CButtonEx		*m_wndPLBEX;
	CButtonEx		*m_wndVoiceBEX;
	CButtonEx		*m_wndPPBEX;
	CButtonEx		*m_wndNextBEX;
	CButtonEx		*m_wndPreBEX;
	CButtonEx		*m_wndStopBEX;
	CButtonEx		*m_ChangeColorBEX;

	CTypeCStatic	m_staFileTitle;
	CTypeCStatic	m_staTime;
	CStatic			m_staVArea;

	CMySliderCtrl		m_sliVoice;
	CMySliderCtrl	m_sliTempo;

	CPlayListDlg	*playlistDlg;



	CString			m_strWndTitle;
	CString			m_strFileTitle;
	CString			m_strTimeShow;
	CString			m_strFilePath;

	IVideoPlayer *	m_pPlayer;
	int				m_iPlayState;

	CBrush			m_brBlackBrush;
	CBrush			m_brLRedBrush;
	CBrush			m_brLGreenBrush;
	CBrush			m_brLBlueBrush;
	CBrush			m_brPinkBrush;
	CBrush			m_brOraBrush;
	CBrush *		m_pCurBrush;
	//bool			m_bCC;

	CMenu			m_mVArea;
	CMenu			m_mPL;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��

protected:
	void setAllPos();
	void setBExPos();
	void setCStaPos();
	void setCSliPos();

	void setShowTime(LONG curPosOfPlay);
	void setShowSysTime();

	void OnMenuOpen();




protected:
	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
		
	DECLARE_MESSAGE_MAP()

public:
	//afx_msg void OnCloseD();
	afx_msg void OnMaximize();
	afx_msg void OnMinimize();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnShowPL();
	afx_msg void OnPlay();
	afx_msg void OnStop();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	//afx_msg void OnChangeColor();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	//afx_msg void OnMenuOpen();
	//afx_msg void OnTitleDBLClicked();
	afx_msg LRESULT OnDialogDestroyed(WPARAM wParam,LPARAM lParam);
public:
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);

public:
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);

	//LPCWSTR stringToLPCWSTR(std::string str);
};
