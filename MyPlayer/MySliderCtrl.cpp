#include "stdafx.h"
#include "MyPlayer.h"
#include "MySliderCtrl.h"


/// CMySliderCtrl file

IMPLEMENT_DYNAMIC(CMySliderCtrl, CSliderCtrl)

CMySliderCtrl::CMySliderCtrl(bool bIsVol)
{
	//m_brBlackColor.CreateSolidBrush(RGB(0,0,0));
	m_bIsVol = bIsVol;
}

CMySliderCtrl::~CMySliderCtrl()
{
}


BEGIN_MESSAGE_MAP(CMySliderCtrl, CSliderCtrl)
	//ON_WM_LBUTTONDOWN()
	//ON_WM_LBUTTONUP()
	//ON_WM_CTLCOLOR_REFLECT()
END_MESSAGE_MAP()

void CMySliderCtrl::setPosByCursor( CPoint point )
{
	int iMin,iMax,isize;
	GetRange(iMin,iMax);
	GetWindowRect(&m_sliRect);
	isize = m_sliRect.Width()-16;
	double pixPPos = (double)isize/(double)(iMax - iMin);
	point.x -=8;
	m_iCurentPos = point.x/pixPPos;
	this->SetPos(m_iCurentPos);
}



// CMySliderCtrl 消息处理程序
//
//void CMySliderCtrl::OnLButtonDown( UINT nFlags, CPoint point )
//{
//	//this->setPosByCursor(point);
//
//	CWnd::OnLButtonDown(nFlags, point);
//}

bool CMySliderCtrl::isVolumeSlider()
{
	return m_bIsVol;
}

int CMySliderCtrl::getSize()
{
	int iMin,iMax;
	GetRange(iMin,iMax);
	m_iSize = iMax - iMin ;
	return m_iSize;
}
//void CMySliderCtrl::OnLButtonUp( UINT nFlags, CPoint point )
//{
//	m_iCurentPos = this->GetPos();
//	this->setPosByCursor(point);
//
//	CWnd::OnLButtonUp(nFlags, point);
//}

//HBRUSH CMySliderCtrl::CtlColor( CDC *pDC,UINT nCtrlColor )
//{
//	return	(HBRUSH)m_brBlackColor;
//}