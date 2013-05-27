/// ColorCStatic.cpp :implement file
//

#include "stdafx.h"
#include "MyPlayer.h"
#include "TypeCStatic.h"


/// CColorCStatic

IMPLEMENT_DYNAMIC(CTypeCStatic, CStatic)

CTypeCStatic::CTypeCStatic()
{

}

CTypeCStatic::~CTypeCStatic()
{
}


BEGIN_MESSAGE_MAP(CTypeCStatic, CStatic)
	ON_WM_PAINT()
END_MESSAGE_MAP()

void CTypeCStatic::setCtlType(COLORREF backColor,COLORREF textColor,CString caption)
{
	m_BackColor = backColor;
	m_TextColor = textColor;
	m_strCaption = caption;
	Invalidate();
}



void CTypeCStatic::OnPaint()
{
	CPaintDC dc(this); /// device context for painting
	CRect rect;
	GetClientRect( &rect );
	CBrush BGBrush;
	BGBrush.CreateSolidBrush(m_BackColor);
	dc.SelectObject(&BGBrush);
	dc.Rectangle(&rect);
	dc.SetBkMode( TRANSPARENT );

	CFont font;
	font.CreatePointFont(3,_T("Arial"));
	CFont *pOldFont;
	pOldFont=dc.SelectObject(&font);
	dc.SetTextColor(m_TextColor);///set the text's color
	dc.DrawText(m_strCaption,&rect,DT_CENTER);///draw the text in the middle of the dlg
	dc.SelectObject(pOldFont);

	BGBrush.DeleteObject();
}

// CColorCStatic 消息处理程序


