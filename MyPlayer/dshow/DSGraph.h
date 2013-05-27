#pragma once


#include <DShow.h>
#include <Vfw.h>
#include <Dshowasf.h>
#include <D3D9.h>
#include <vmr9.h>
#include "../common/ErrorCode.h"
#include "state/DSState.h"
#include "DSGParam.h"


class DSGraph
{
	friend class DSState;
	friend class DSInitState;
	friend class DSPausedState;
	friend class DSRunState;
	friend class DSStopState;
protected:
	DSGraph(void);
public:
	virtual ~DSGraph(void);
	const char *			getError(int status){return m_oErr.getError(status);};

protected:
	virtual	int				createGraph(DSGParam oParams);
	virtual int				runGraph();
	virtual int				pauseGraph();
	virtual	int				stopGraph();

	virtual bool			isRunning();
	virtual bool			isPaused();
	virtual bool			isStopped();


	DSState *				runningState() const;
	DSState *				pausedState()	const;
	DSState *				stoppedState()	const;
	DSState *				initState()		const;

	int						addError(const char *strErr){return m_oErr.addError(strErr);};

	IBaseFilter *			createFilter(CLSID	clsid,LPCWSTR pInFilterName,const	char *pName = NULL);	//创建filter并加入到graph中，当pName为NULL时，以clsid直接创建filter，当pName不为Null时，以pName在clsid所属的类别中创建filter

	int						connectPin(IPin *pInIn,IPin *pInOut);					//连接两个pin,成功时返回值大于0
	IBaseFilter *			getFilter(const char *pName = NULL);					//在filterGraph中获得pName的filter
	IPin		*			getPin(IBaseFilter *pFilter,BOOL inInput = TRUE,const char * inPartialName = NULL);	//在指定filter中获得IPin
	IPin		*			getUnconnectedPin(IBaseFilter *pFilter, BOOL inInPut = TRUE);	//在指定filter中获得未连接的IPin

	void		*			getFilterInterface(IBaseFilter *pInFilter,REFIID inInterfaceID);	//查找filter的接口,返回值为接口指针
	void		*			getFilterInterface(const char * pInFilterName,REFIID inInterfaceID);	//查找filter的接口，返回值为接口指针

	void		*			getPinInterface(IPin *pInPin,REFIID inInterfaceID);					//查找pin的接口，返回值为接口指针

private:
	int						delGraph();
	//for debug, in graphedit
	HRESULT					addToRot();
	void					removeFromRot();

protected:
	DSState	*							m_pState;
	IGraphBuilder *						m_pGraph;					//filter graph;
	IMediaControl *						m_pMediaCtrl;				//control the media;
	IMediaEventEx	  *					m_pMediaEvent;				//event of the filter graph;
	IMediaSeeking *						m_pMediaSeek;				//媒体定位时的接口
	IBasicAudio	*						m_pVolumeCtrl;				//设置声音的接口
	IVMRWindowlessControl9 *			m_pVMRCtrl;					//使用VMR9时 windowless mode 时的指针
	DSGParam							m_oGraphParam;				//param of the filter graph;

private:
	DWORD					dw_debug;	//for debug in graphedit
	ErrorCode				m_oErr;
	DSState *				m_pRunningState;
	DSState *				m_pPausedState;
	DSState *				m_pStoppedState;
	DSState *				m_pInitState;
};
