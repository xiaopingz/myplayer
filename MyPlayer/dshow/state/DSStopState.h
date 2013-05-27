#pragma once
#include "DSState.h"
#include "../DSGraph.h"
class DSStopState : public DSState
{
public:
	DSStopState(DSGraph *pGraph);
public:
	virtual ~DSStopState(void);

	virtual	int				createGraph(DSGParam oParams);
	virtual int				runGraph();
	virtual int				pauseGraph();
	virtual	int				stopGraph();

	virtual bool			isRunning();
	virtual bool			isPaused();
	virtual bool			isStopped();

protected:
	virtual int				addError(const char * strErr);
protected:
	DSGraph	*				m_pDSGraph;
};
