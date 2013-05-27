#pragma once
#include <string>
#include <Windows.h>
#define ONESECOND	1000

typedef enum _MediaState
{
	smsNull = -1,
	smsStopped =1,
	smsPlaying,
	smsPaused
} MediaState;

class IVideoPlayer
{
public:
	IVideoPlayer(void);
	virtual ~IVideoPlayer(void);

	virtual int				onResetWnd(void *pHdc) = 0;		//�����ڵ���OnSize,OnPaintʱ�����ô˴���


	virtual int				SetSource(std::string sURL) = 0;

	virtual int				Play() = 0;
	virtual int				Pause() = 0;
	virtual int				Stop() = 0;

	virtual LONG			GetPosition() = 0;					//return value:time(second)
	virtual int				SetPosition( LONG nPosition ) = 0;	

	virtual LONG			GetLength() = 0;	//total time: second	

	virtual MediaState		GetPlayState() = 0;

	virtual BOOL			IsMute() = 0;
	virtual int				SetMute(BOOL bMute) = 0;

	virtual int				SetVolume( LONG nVolume ) = 0;	//Specifies the volume, as a number from �C10,000 to 0, inclusive.  Full volume is 0, and �C10,000 is silence. 
	virtual LONG			GetVolume() = 0;	//Divide by 100 to get equivalent decibel value. For example, �C10,000 is �C100 dB.

	//virtual LONG		GetBuffering() = 0;

	virtual BOOL			IsFullScreen() = 0;
	virtual int				SetFullScreen(BOOL bFull=TRUE) = 0;

};
