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

	IBaseFilter *			createFilter(CLSID	clsid,LPCWSTR pInFilterName,const	char *pName = NULL);	//����filter�����뵽graph�У���pNameΪNULLʱ����clsidֱ�Ӵ���filter����pName��ΪNullʱ����pName��clsid����������д���filter

	int						connectPin(IPin *pInIn,IPin *pInOut);					//��������pin,�ɹ�ʱ����ֵ����0
	IBaseFilter *			getFilter(const char *pName = NULL);					//��filterGraph�л��pName��filter
	IPin		*			getPin(IBaseFilter *pFilter,BOOL inInput = TRUE,const char * inPartialName = NULL);	//��ָ��filter�л��IPin
	IPin		*			getUnconnectedPin(IBaseFilter *pFilter, BOOL inInPut = TRUE);	//��ָ��filter�л��δ���ӵ�IPin

	void		*			getFilterInterface(IBaseFilter *pInFilter,REFIID inInterfaceID);	//����filter�Ľӿ�,����ֵΪ�ӿ�ָ��
	void		*			getFilterInterface(const char * pInFilterName,REFIID inInterfaceID);	//����filter�Ľӿڣ�����ֵΪ�ӿ�ָ��

	void		*			getPinInterface(IPin *pInPin,REFIID inInterfaceID);					//����pin�Ľӿڣ�����ֵΪ�ӿ�ָ��

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
	IMediaSeeking *						m_pMediaSeek;				//ý�嶨λʱ�Ľӿ�
	IBasicAudio	*						m_pVolumeCtrl;				//���������Ľӿ�
	IVMRWindowlessControl9 *			m_pVMRCtrl;					//ʹ��VMR9ʱ windowless mode ʱ��ָ��
	DSGParam							m_oGraphParam;				//param of the filter graph;

private:
	DWORD					dw_debug;	//for debug in graphedit
	ErrorCode				m_oErr;
	DSState *				m_pRunningState;
	DSState *				m_pPausedState;
	DSState *				m_pStoppedState;
	DSState *				m_pInitState;
};
