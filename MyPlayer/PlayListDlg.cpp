// PlayListDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "MyPlayer.h"
#include "PlayListDlg.h"
#include "MyPlayerDlg.h"


// PlayListDlg �Ի���

IMPLEMENT_DYNAMIC(CPlayListDlg, CDialog)

CPlayListDlg::CPlayListDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CPlayListDlg::IDD, pParent)
	,m_pParent(pParent)
{
	m_brBule.CreateSolidBrush(RGB(0,192,192));

}

CPlayListDlg::~CPlayListDlg()
{
}

void CPlayListDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CPlayListDlg, CDialog)
	ON_WM_CLOSE()
	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()

void CPlayListDlg::PostNcDestroy()
{
	CDialog::PostNcDestroy();
	//((CMyPlayerDlg *)m_pParent)->OnDialogDestroyed(0,0);
	delete this;
}

// PlayListDlg ��Ϣ�������

void CPlayListDlg::OnClose()
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	m_pParent->SendMessage(WM_USER_DIALOG_DESTROYED,0,0);
	CDialog::OnClose();
}

HBRUSH CPlayListDlg::OnCtlColor(CDC* pDC, CWnd *pWnd,UINT nCtlColor)
{
	return (HBRUSH)m_brBule;
}