#pragma once
#define  WM_GRAPHNOTIFY (WM_USER + 20)
#include "IVideoPlayer.h"
#include "DSGraph.h"
#include "DSGParam.h"


class DSVideoPlay : public DSGraph, public IVideoPlayer
{
public:
	DSVideoPlay(HWND hNotifyWnd,HWND hRenderWnd);
public:
	virtual ~DSVideoPlay(void);

	//implementation of IVideoPlayer
	virtual int				onResetWnd(void *pHdc);		//当窗口调用OnSize,OnPaint时，调用此窗口


	virtual int				SetSource(std::string sURL);

	virtual int				Play();
	virtual int				Pause();
	virtual int				Stop();

	virtual LONG			GetPosition();					//return value:time(second)
	virtual int				SetPosition( LONG nPosition );	

	virtual LONG			GetLength();	//total time: second	

	virtual MediaState		GetPlayState();

	virtual BOOL			IsMute();
	virtual int				SetMute(BOOL bMute);

	virtual int				SetVolume( LONG nVolume );	//Specifies the volume, as a number from –10,000 to 0, inclusive.  Full volume is 0, and –10,000 is silence. 
	virtual LONG			GetVolume();	//Divide by 100 to get equivalent decibel value. For example, –10,000 is –100 dB.

	virtual BOOL			IsFullScreen();
	virtual int				SetFullScreen(BOOL bFull=TRUE);
	

protected:
	HWND					m_hNotifyWnd;		//dshow消息通知窗口
	HWND					m_hRenderWnd;
	BOOL					m_bIsFullScreen;
	HDC						m_oVideoWndDC;		//保存视频窗口的HDC，在第一次进行全屏操作时进行初始化
};
