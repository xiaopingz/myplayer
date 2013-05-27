#pragma once

#include "../../common/ErrorCode.h"
#include "../DSGParam.h"
class DSState
{
protected:
	DSState(void);
public:
	virtual ~DSState(void);

	virtual	int				createGraph(DSGParam oParams) =0;
	virtual int				runGraph() =0;
	virtual int				pauseGraph() =0;
	virtual	int				stopGraph() =0;

	virtual bool			isRunning() =0;
	virtual bool			isPaused() =0;
	virtual bool			isStopped() =0;


protected:
	virtual int				addError(const char *strErr) =0;

};
