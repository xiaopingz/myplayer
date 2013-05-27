#pragma once

#include "DSState.h"
#include "../DSGParam.h"
#include "../DSGraph.h"

class DSInitState : public DSState
{
public:
	DSInitState(DSGraph *pGraph);
public:
	virtual ~DSInitState(void);

	virtual	int				createGraph(DSGParam oParams);
	virtual int				runGraph();
	virtual int				pauseGraph();
	virtual	int				stopGraph();

	virtual bool			isRunning();
	virtual bool			isPaused();
	virtual bool			isStopped();

protected:
	int						addError(const char * strErr);

protected:
	DSGraph	*				m_pDSGraph;
};
