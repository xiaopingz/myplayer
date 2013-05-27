
#include "DSRunState.h"
#include "../DSGraph.h"

DSRunState::DSRunState( DSGraph *pGraph )
: m_pDSGraph(pGraph)
{
}
DSRunState::~DSRunState(void)
{
}

int DSRunState::createGraph( DSGParam oParams )
{
	return addError("The graph current is running, you cannot create it");
}

int DSRunState::runGraph()
{
	return addError("The graph current is running, you cannot run it again!");
}

int DSRunState::pauseGraph()
{
	HRESULT hr;
	hr = m_pDSGraph->m_pMediaCtrl->Pause();
	m_pDSGraph->m_pState = m_pDSGraph->pausedState();
	return 1;
}

int DSRunState::stopGraph()
{
	HRESULT hr;
	hr = m_pDSGraph->m_pMediaCtrl->Stop();
	m_pDSGraph->m_pState = m_pDSGraph->stoppedState();
	return 1;
}

bool DSRunState::isRunning()
{
	return true;
}

bool DSRunState::isPaused()
{
	return false;
}

bool DSRunState::isStopped()
{
	return false;
}

int DSRunState::addError( const char * strErr )
{
	return m_pDSGraph->addError(strErr);
}