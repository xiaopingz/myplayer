#pragma once
#include "DSState.h"
#include "../DSGraph.h"
class DSPausedState : public DSState
{
public:
	DSPausedState(DSGraph *pGraph);
public:
	virtual ~DSPausedState(void);
	virtual	int				createGraph(DSGParam oParams);
	virtual int				runGraph();
	virtual int				pauseGraph();
	virtual	int				stopGraph();

	virtual bool			isRunning();
	virtual bool			isPaused();
	virtual bool			isStopped();
protected:
	virtual int				addError(const char *strErr);
protected:
	DSGraph	*				m_pDSGraph;
};
