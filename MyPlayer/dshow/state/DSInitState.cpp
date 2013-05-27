
#include "DSInitState.h"


DSInitState::DSInitState( DSGraph *pGraph )
: m_pDSGraph(pGraph)
{

}
DSInitState::~DSInitState(void)
{
}

int DSInitState::createGraph( DSGParam oParams )
{

	HRESULT hr;
	//create m_pGraph
	hr = CoCreateInstance(CLSID_FilterGraph, 0, CLSCTX_INPROC_SERVER,IID_IGraphBuilder, (void**)&m_pDSGraph->m_pGraph);
	if(hr !=S_OK)		return addError("Cannot Create FilterGraph.Maybe you didn't call CoInitialize()");
	hr = m_pDSGraph->m_pGraph->QueryInterface(IID_IMediaControl,(void **)&m_pDSGraph->m_pMediaCtrl);
	if(hr!=S_OK)		return addError("Cannot get the interface IMediaControl!");
	hr = m_pDSGraph->m_pGraph->QueryInterface(IID_IMediaEventEx,(void **)&m_pDSGraph->m_pMediaEvent);
	if(hr!=S_OK)		return addError("Cannot get interface IMediaEvent!");
	hr = m_pDSGraph->m_pGraph->QueryInterface(IID_IMediaSeeking,(void **)&m_pDSGraph->m_pMediaSeek);
	if(hr!=S_OK)		return addError("Cannot get interface IMediaSeek!");

	if(oParams.getMode() & DSGParam::modeDebug)
		m_pDSGraph->addToRot();
	
	return 1;
}

int DSInitState::runGraph()
{
	HRESULT hr;
	hr = m_pDSGraph->m_pMediaCtrl->Run();
	if(S_OK!=hr && S_FALSE != hr)	return addError("Cannot run the filter graph.Maybe other application is using this device!");
	m_pDSGraph->m_pState = m_pDSGraph->runningState();
	return 1;
}

int DSInitState::pauseGraph()
{
	return addError("The current graph is not running, you cannot pause it!");
}

int DSInitState::stopGraph()
{
	return addError("The current graph is not running , you cannot stop it!");
}

bool DSInitState::isRunning()
{
	return false;
}

bool DSInitState::isPaused()
{
	return false;
}

bool DSInitState::isStopped()
{
	return false;
}

int DSInitState::addError( const char * strErr )
{
	return m_pDSGraph->addError(strErr);
}