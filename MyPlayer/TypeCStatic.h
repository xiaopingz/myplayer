#pragma once


// CColorCStatic

class CTypeCStatic : public CStatic
{
	DECLARE_DYNAMIC(CTypeCStatic)

public:
	CTypeCStatic();
	virtual ~CTypeCStatic();

protected:
	afx_msg void OnPaint();
	DECLARE_MESSAGE_MAP()

protected:
	CString		m_strCaption;
	COLORREF	m_BackColor;
	COLORREF	m_TextColor;
	//INT			m_fontSize;

public:
	void setCtlType(COLORREF backColor,COLORREF textColor,CString caption);
};


