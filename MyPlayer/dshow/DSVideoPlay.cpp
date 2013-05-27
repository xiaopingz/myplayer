#pragma warning(disable:4995)

#include <tchar.h>
#include <windows.h>
#include <assert.h>
#include "DSVideoPlay.h"

DSVideoPlay::DSVideoPlay(HWND hNotifyWnd, HWND hRenderWnd)
: m_hNotifyWnd(hNotifyWnd)
, m_hRenderWnd(hRenderWnd)
, m_bIsFullScreen(FALSE)
{
}

DSVideoPlay::~DSVideoPlay(void)
{

}

int DSVideoPlay::onResetWnd( void *pHdc )
{
	HRESULT hr;
	RECT rect;
	if(!m_oGraphParam.getRenderWnd())
	{
		return addError("No Video window can be reseted!");
	}
	::GetClientRect(*(m_oGraphParam.getRenderWnd()),&rect);
	if(m_oGraphParam.isWindowless())
	{
		if(pHdc)
			hr = m_pVMRCtrl->RepaintVideo(*(m_oGraphParam.getRenderWnd()),*(HDC *)pHdc);
		hr = m_pVMRCtrl->SetVideoPosition(NULL,&rect);
		return 1;
	}
	else 
	{
		return addError("Not support windowed mode!");
	}
	return addError("Wrong param!");
}

int DSVideoPlay::SetSource( std::string sURL )
{
	DSGParam oParam(sURL);
	oParam.setNotifyWnd(&m_hNotifyWnd);
	oParam.setRenderWnd(&m_hRenderWnd);
	oParam.setWindowless(true);
	int status = createGraph(oParam);
	if(status<0)
		return status;

	//开始创建剩下的链路
	//增加VMR9 render
	IBaseFilter *pVMR9Filter;
	HRESULT hr;
	pVMR9Filter = createFilter(CLSID_VideoMixingRenderer9,L"VMR9 Render");
	if(!pVMR9Filter)
		return addError("Cannot create VMR9 render!");
	//config the VMR9 render!
	IVMRFilterConfig9 * pVMR9Config = (IVMRFilterConfig9 *)getFilterInterface(pVMR9Filter,IID_IVMRFilterConfig9);
	if(!pVMR9Config)
		return addError("Cannot get the interface IID_IVMRFilterConfig9!");
	pVMR9Config->SetRenderingMode(VMRMode_Windowless);
	pVMR9Config->Release();
	pVMR9Config = NULL;

	IVMRWindowlessControl9 *pVMR9Ctrl = (IVMRWindowlessControl9 *)getFilterInterface(pVMR9Filter,IID_IVMRWindowlessControl9);
	if(!pVMR9Ctrl)
		return addError("Cannot get the interface IID_IVMRWindowlessControl9!");
	m_pVMRCtrl = pVMR9Ctrl;
	hr = pVMR9Ctrl->SetVideoClippingWindow(*(oParam.getRenderWnd()));
	if(hr == E_INVALIDARG)
		return addError("Error SetVideoClippingWindow!E_INVALIDARG:The hwnd parameter is NULL.");
	if(hr == VFW_E_WRONG_STATE)
		return addError("Error SetVideoClippingWindow!VFW_E_WRONG_STATE:The VMR is not in windowless mode.");
	if(hr!=S_OK)
		return addError("Error SetVideoClippingWindow!Unknown error!");
	RECT rect;
	::GetClientRect(*(oParam.getRenderWnd()),&rect);
	hr = pVMR9Ctrl->SetVideoPosition(NULL,&rect);

	size_t size = oParam.getFile().length(); 
	const wchar_t * buffer =string2LPCWSTR(oParam.getFile());
	//MultiByteToWideChar( CP_ACP, 0, oParam.getFile().c_str(), size, buffer, size * sizeof(wchar_t));
	//buffer[size] = 0;
	hr = m_pGraph->RenderFile(string2LPCWSTR(oParam.getFile()),NULL);
	if(FAILED(hr))
		return -1;
	//设置声音接口
	m_pVolumeCtrl = (IBasicAudio *)getFilterInterface("Default DirectSound Device",IID_IBasicAudio);
	assert(m_pVolumeCtrl!=NULL);

	//设置消息通知窗口
	m_pMediaEvent->SetNotifyWindow((OAHWND)m_hNotifyWnd, WM_GRAPHNOTIFY, 0);
	return 1;
}

int DSVideoPlay::Play()
{
	return runGraph();
}

int DSVideoPlay::Pause()
{
	return pauseGraph();
}

int DSVideoPlay::Stop()
{
	return stopGraph();
}

LONG DSVideoPlay::GetPosition()
{
	assert(m_pMediaSeek);
	LONGLONG lCurPos;
	m_pMediaSeek->GetCurrentPosition(&lCurPos);
	LONG pos = lCurPos/10000000;	//转换为秒
	return pos;
}

int DSVideoPlay::SetPosition( LONG nPosition )
{
	assert(m_pMediaSeek);
	pauseGraph();
	LONGLONG	lPos = static_cast<LONGLONG>(1.0*nPosition *10000000);
	HRESULT hr = m_pMediaSeek->SetPositions(&lPos,AM_SEEKING_AbsolutePositioning,NULL,AM_SEEKING_NoPositioning);
	runGraph();
	if(S_OK!=hr)
		return addError("Cannot set the position!");

	return 1;
}


LONG DSVideoPlay::GetLength()
{
	assert(m_pMediaSeek);
	LONGLONG lTotalTime;
	HRESULT hr = m_pMediaSeek->GetDuration(&lTotalTime);
	if(S_OK != hr)
		return addError("Cannot get the total time!");
	lTotalTime = lTotalTime/10000000;
	return lTotalTime;
}

MediaState DSVideoPlay::GetPlayState()
{
	MediaState ms;
	if(isRunning())
		ms = smsPlaying;
	else if (isPaused())
		ms = smsPaused;
	else if(isStopped())
		ms = smsStopped;
	else 
		ms = smsNull;
	return ms;
}

BOOL DSVideoPlay::IsMute()
{
	assert(m_pVolumeCtrl);
	long lVolume;
	HRESULT hr = m_pVolumeCtrl->get_Volume(&lVolume);
	if(lVolume <= -10000)
		return TRUE;
	else
		return FALSE;
}

int DSVideoPlay::SetMute( BOOL bMute )
{
	assert(m_pVolumeCtrl);
	long lVolume= -10000;
	HRESULT hr = m_pVolumeCtrl->put_Volume(lVolume);
	if(S_OK != hr)
		return addError("Cannot SetMute!");
	else
		return 1;
}

int DSVideoPlay::SetVolume( LONG nVolume )
{
	assert(m_pVolumeCtrl);
	HRESULT hr = m_pVolumeCtrl->put_Volume(nVolume);
	if(S_OK != hr)
		return addError("Cannot SetMute!");
	else
		return 1;
}

LONG DSVideoPlay::GetVolume()
{
	assert(m_pVolumeCtrl);
	long lVolume = 0;
	HRESULT hr = m_pVolumeCtrl->get_Volume(&lVolume);
	return lVolume;
}

BOOL DSVideoPlay::IsFullScreen()
{
	return m_bIsFullScreen;
}

int DSVideoPlay::SetFullScreen( BOOL bIsFullScreen/*=TRUE*/ )
{
	if(bIsFullScreen)
	{//设置为全屏模式
		if(!m_bIsFullScreen)
		{
			m_bIsFullScreen = bIsFullScreen;
			HWND *pWnd = m_oGraphParam.getRenderWnd();
			m_oVideoWndDC = ::GetDC(*pWnd);
			HDC oFullScreenDC = ::GetWindowDC(NULL);
			return onResetWnd((void *)&oFullScreenDC);
		}
		return 1;
	}
	else
	{//设置为窗口模式
		if(m_bIsFullScreen)
		{
			m_bIsFullScreen = bIsFullScreen;
			return onResetWnd((void *)&m_oVideoWndDC);
		}
		return 1;
	}
	return 1;
}