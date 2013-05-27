#pragma once
#include "InitDed.h"
#include <string>
#include <GdiPlus.h>
#include "common/ErrorCode.h"
using namespace Gdiplus;//应用 GDI+前必须的设置

// CButtonEx

class CButtonEx : public CButton
{
	DECLARE_DYNAMIC(CButtonEx)

public:
	CButtonEx(CString drawIconP,CString moveIconP);
	virtual ~CButtonEx();

protected:
	virtual void PreSubclassWindow();

public:
	void CButtonEx::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);

protected:
	afx_msg int CButtonEx::OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void CButtonEx::OnMouseMove(UINT nFlags, CPoint point);
	afx_msg LRESULT OnMouseLeave( WPARAM wParam , LPARAM lParam ) ;
	DECLARE_MESSAGE_MAP()
	CString			m_strDrawIcon;
	CString			m_strMoveIcon;
	CString			m_strLeaveIcon;
	GdiplusStartupInput gdiplusStartupInput;
	ULONG_PTR gdiplusToken;
};


