#pragma once
#include <string>
#include <DShow.h>
#include <Vfw.h>
#include <Dshowasf.h>

class DSGParam
{
public:
	enum DSGraphMode{
		modeFile = 1,
		modeDebug = 2,
		modeWnd = 4
	};
	typedef enum DSGraphMode		DSGMode;
public:
	DSGParam(void);
	DSGParam(const std::string strFilePath,bool isDebug = true, bool isWnd = true);
	DSGParam(const DSGParam &		oParam);
	DSGParam &			operator=(const DSGParam & oParam);
	~DSGParam(void);

	unsigned int	getMode() const;

	void			setFile(std::string		strFile);
	std::string		getFile()	const;

	void			setRenderWnd(HWND * pRenderWnd);
	HWND *			getRenderWnd() const;

	void			setNotifyWnd(HWND *pNotifyWnd);
	HWND *			getNotifyWnd()	const;

	void			setWindowless(bool isWndless);
	bool			isWindowless() const;
protected:

	friend class DSGraph;

	unsigned			m_uiMode;

	std::string			m_strFilePath;
	HWND *				m_phRenderWnd;
	HWND *				m_phNotifyWnd;
	bool				m_bIsWindowLess;
};
