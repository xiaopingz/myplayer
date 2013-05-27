
#include "DSPausedState.h"


DSPausedState::DSPausedState( DSGraph *pGraph )
: m_pDSGraph(pGraph)
{

}
DSPausedState::~DSPausedState(void)
{
}

int DSPausedState::addError( const char *strErr )
{
	return m_pDSGraph->addError(strErr);
}

int DSPausedState::createGraph( DSGParam oParams )
{
	return addError("The graph current is paused, you cannot create it again!");
}

int DSPausedState::runGraph()
{
	HRESULT hr;
	hr = m_pDSGraph->m_pMediaCtrl->Run();
	if(S_OK!=hr && S_FALSE != hr)	return addError("Cannot run the filter graph.Maybe other application is using this device!");
	m_pDSGraph->m_pState = m_pDSGraph->runningState();
	return 1;
}

int DSPausedState::pauseGraph()
{
	return addError("The graph current has already paused, you cannot pause it again!");
}

int DSPausedState::stopGraph()
{
	HRESULT hr;
	hr = m_pDSGraph->m_pMediaCtrl->Stop();
	m_pDSGraph->m_pState = m_pDSGraph->stoppedState();
	return 1;
}

bool DSPausedState::isRunning()
{
	return false;
}

bool DSPausedState::isPaused()
{
	return true;
}

bool DSPausedState::isStopped()
{
	return false;
}