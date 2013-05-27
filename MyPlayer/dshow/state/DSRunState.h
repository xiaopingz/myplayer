#pragma once
#include "DSState.h"
#include "../DSGraph.h"
class DSRunState : public DSState
{
public:
	DSRunState(DSGraph *pGraph);
public:
	virtual ~DSRunState(void);

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
