
#include "DSGParam.h"

DSGParam::DSGParam(void)
: m_phRenderWnd(NULL)
, m_phNotifyWnd(NULL)
, m_bIsWindowLess(false)
, m_strFilePath()
{
}

DSGParam::DSGParam( const std::string strFilePath,bool isDebug /*= true*/, bool isWnd /*= true*/ )
: m_phRenderWnd(NULL)
, m_phNotifyWnd(NULL)
, m_bIsWindowLess(true)
, m_strFilePath(strFilePath)
, m_uiMode(0)
{
	m_uiMode |= modeFile;
	if(isDebug)
		m_uiMode |= modeDebug;
	if(isWnd)
		m_uiMode |= modeWnd;
}

DSGParam::DSGParam( const DSGParam & oParam )
{
	m_uiMode = oParam.m_uiMode;
	m_phNotifyWnd = oParam.m_phNotifyWnd;
	m_phRenderWnd = oParam.m_phRenderWnd;
	m_strFilePath = oParam.m_strFilePath;
	m_bIsWindowLess = oParam.m_bIsWindowLess;
}
DSGParam::~DSGParam(void)
{
}

DSGParam & DSGParam::operator=( const DSGParam & oParam )
{
	m_uiMode = oParam.m_uiMode;
	m_phNotifyWnd = oParam.m_phNotifyWnd;
	m_phRenderWnd = oParam.m_phRenderWnd;
	m_strFilePath = oParam.m_strFilePath;
	m_bIsWindowLess = oParam.m_bIsWindowLess;
	return *this;
}

void DSGParam::setFile( std::string strFile )
{
	m_strFilePath = strFile;
}

std::string DSGParam::getFile() const
{
	return m_strFilePath;
}

void DSGParam::setRenderWnd( HWND * pRenderWnd )
{
	m_phRenderWnd = pRenderWnd;
}

HWND * DSGParam::getRenderWnd() const
{
	return m_phRenderWnd;
}

void DSGParam::setNotifyWnd( HWND *pNotifyWnd )
{
	m_phNotifyWnd = pNotifyWnd;
}

HWND * DSGParam::getNotifyWnd() const
{
	return m_phNotifyWnd;
}

void DSGParam::setWindowless( bool isWndless )
{
	m_bIsWindowLess = isWndless;
}

bool DSGParam::isWindowless() const
{
	return m_bIsWindowLess;
}

unsigned int DSGParam::getMode() const
{
	return m_uiMode;
}