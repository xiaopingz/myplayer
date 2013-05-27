#pragma once


// CMySliderCtrl

class CMySliderCtrl : public CSliderCtrl
{
	DECLARE_DYNAMIC(CMySliderCtrl)

public:
	CMySliderCtrl(bool bIsVol);
	virtual ~CMySliderCtrl();

	void	setPosByCursor(CPoint point);
	int		getSize();
	bool	isVolumeSlider();

protected:
	//afx_msg		HBRUSH CtlColor(CDC *pDC,UINT nCtrlColor);
	//afx_msg		void OnLButtonDown(UINT nFlags, CPoint point);
	//afx_msg void		OnLButtonUp(UINT nFlags, CPoint point);
	DECLARE_MESSAGE_MAP()
	//CBrush		m_brBlackColor;
	CRect		m_sliRect;
	int			m_iCurentPos;
	int			m_iSize;
	bool		m_bIsVol;
};


