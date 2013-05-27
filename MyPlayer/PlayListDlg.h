#pragma once


// PlayListDlg 对话框

class CPlayListDlg : public CDialog
{
	DECLARE_DYNAMIC(CPlayListDlg)

public:
	CPlayListDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CPlayListDlg();

// 对话框数据
	enum { IDD = IDD_PLAYLIST_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual void PostNcDestroy();

protected:
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd *pWnd,UINT nCtlColor);
	DECLARE_MESSAGE_MAP()
	CWnd	*m_pParent;
	CBrush	m_brBule;
public:
	afx_msg void OnClose();
};
