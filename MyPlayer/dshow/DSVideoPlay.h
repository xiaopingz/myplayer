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
	virtual int				onResetWnd(void *pHdc);		//�����ڵ���OnSize,OnPaintʱ�����ô˴���


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

	virtual int				SetVolume( LONG nVolume );	//Specifies the volume, as a number from �C10,000 to 0, inclusive.  Full volume is 0, and �C10,000 is silence. 
	virtual LONG			GetVolume();	//Divide by 100 to get equivalent decibel value. For example, �C10,000 is �C100 dB.

	virtual BOOL			IsFullScreen();
	virtual int				SetFullScreen(BOOL bFull=TRUE);
	

protected:
	HWND					m_hNotifyWnd;		//dshow��Ϣ֪ͨ����
	HWND					m_hRenderWnd;
	BOOL					m_bIsFullScreen;
	HDC						m_oVideoWndDC;		//������Ƶ���ڵ�HDC���ڵ�һ�ν���ȫ������ʱ���г�ʼ��
};
