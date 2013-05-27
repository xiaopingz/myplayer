/// ButtonEx.cpp : 实现文件
//

#include "stdafx.h"
#include "MyPlayer.h"
#include "ButtonEx.h"

#pragma comment (lib,"GdiPlus.lib")

/// CButtonEx

IMPLEMENT_DYNAMIC(CButtonEx, CButton)

CButtonEx::CButtonEx(CString drawIconP,CString moveIconP)
{
	m_strDrawIcon = drawIconP;
	m_strMoveIcon = moveIconP;
	m_strLeaveIcon = drawIconP;
	GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);
}

CButtonEx::~CButtonEx()
{
	GdiplusShutdown(gdiplusToken);
}


BEGIN_MESSAGE_MAP(CButtonEx, CButton)
	ON_WM_CREATE()
	ON_WM_MOUSEMOVE()
	ON_MESSAGE(WM_MOUSELEAVE,OnMouseLeave)
END_MESSAGE_MAP()

void CButtonEx::PreSubclassWindow()
{
	CButton::PreSubclassWindow();
	ModifyStyle(0, BS_OWNERDRAW);
}


// CButtonEx 消息处理程序

int CButtonEx::OnCreate(LPCREATESTRUCT lpCreateStruct)  
{
	if (CButton::OnCreate(lpCreateStruct) == -1)
		return -1;

	CRect rc;
	GetClientRect(&rc);
	CDC dc;
	Graphics graph(GetDC()->m_hDC);
	Image image(string2LPCWSTR((LPCTSTR)m_strDrawIcon));
	graph.DrawImage(&image,0,0,rc.right-rc.left,rc.bottom-rc.top);

	///CButton::OnCreate(lpCreateStruct);
	return 0;
}

///处理 MouseMove 和MouseLeave
void CButtonEx::OnMouseMove(UINT nFlags, CPoint point)  
{
	CRect rc;
	GetClientRect(&rc);
	CDC dc;
	Graphics graph(GetDC()->m_hDC);
	Image image(string2LPCWSTR((LPCTSTR)m_strMoveIcon));//鼠标移到控件上的图标
	graph.DrawImage(&image,0,0,rc.right-rc.left,rc.bottom-rc.top);

	TRACKMOUSEEVENT event ;
	event.cbSize = sizeof( event );
	event.dwFlags = TME_LEAVE ;
	event.dwHoverTime = 0 ;
	event.hwndTrack = GetSafeHwnd() ;
	VERIFY (_TrackMouseEvent( &event )) ;
	CButton::OnMouseMove(nFlags, point);
}


LRESULT CButtonEx::OnMouseLeave( WPARAM wParam , LPARAM lParam )
{
	CRect rc;
	GetClientRect(&rc);
	CDC dc;
	Graphics graph(GetDC()->m_hDC);
	Image image(string2LPCWSTR((LPCTSTR)m_strLeaveIcon));//鼠标从控件上移开后的图标
	graph.DrawImage(&image,0,0,rc.right-rc.left,rc.bottom-rc.top);
	return (LRESULT)1;
}


void CButtonEx::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)  
{
	/// TODO: Add your code to draw the specified item
	CRect rc;
	GetClientRect(&rc);
	CDC dc;
	Graphics graph(GetDC()->m_hDC);
	Image image(string2LPCWSTR((LPCTSTR)m_strDrawIcon));//最初创建时的图标
	graph.DrawImage(&image,0,0,rc.right-rc.left,rc.bottom-rc.top);

}
