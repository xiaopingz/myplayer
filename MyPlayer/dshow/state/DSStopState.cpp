
#include "DSStopState.h"
#include "../DSGraph.h"

DSStopState::DSStopState( DSGraph *pGraph )
: m_pDSGraph(pGraph)
{
}
DSStopState::~DSStopState(void)
{
}

int DSStopState::createGraph( DSGParam oParams )
{
	m_pDSGraph->delGraph();
	return m_pDSGraph->createGraph(oParams);
}

int DSStopState::runGraph()
{
	//unknown beheavor
	HRESULT hr;
	hr = m_pDSGraph->m_pMediaCtrl->Run();
	if(S_OK!=hr && S_FALSE != hr)	return addError("Cannot run the filter graph.Maybe other application is using this device!");
	m_pDSGraph->m_pState = m_pDSGraph->runningState();
	return 1;
}

int DSStopState::pauseGraph()
{
	return addError("The graph current is stopped, you cannot pause it!");
}

int DSStopState::stopGraph()
{
	return addError("The graph current has already stopped, you cannot stop it again!");
}

bool DSStopState::isRunning()
{
	return false;
}

bool DSStopState::isPaused()
{
	return false;
}

bool DSStopState::isStopped()
{
	return true;
}

int DSStopState::addError( const char * strErr )
{
	return m_pDSGraph->addError(strErr);
}