#pragma once


// PlayListDlg �Ի���

class CPlayListDlg : public CDialog
{
	DECLARE_DYNAMIC(CPlayListDlg)

public:
	CPlayListDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CPlayListDlg();

// �Ի�������
	enum { IDD = IDD_PLAYLIST_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	virtual void PostNcDestroy();

protected:
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd *pWnd,UINT nCtlColor);
	DECLARE_MESSAGE_MAP()
	CWnd	*m_pParent;
	CBrush	m_brBule;
public:
	afx_msg void OnClose();
};
