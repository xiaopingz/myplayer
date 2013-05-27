#pragma warning(disable:4995)
#include "DSGraph.h"
#include "state/DSRunState.h"
#include "state/DSInitState.h"
#include "state/DSPausedState.h"
#include "state/DSStopState.h"

DSGraph::DSGraph(void)
{
	dw_debug = 0;
	m_pGraph = NULL;
	m_pMediaCtrl = NULL;
	m_pMediaEvent = NULL;
	m_pMediaSeek = NULL;
	m_pVMRCtrl = NULL;
	m_pVolumeCtrl = NULL;

	m_pState = NULL;
	m_pRunningState = new DSRunState(this);
	m_pInitState = new DSInitState(this);
	m_pPausedState = new DSPausedState(this);
	m_pStoppedState = new DSPausedState(this);

	m_pState = initState();
}

DSGraph::~DSGraph(void)
{
	if(m_pState->isRunning())
		m_pState->stopGraph();
	delGraph();
	delete m_pRunningState;
	delete m_pInitState;
	delete m_pPausedState;
	delete m_pStoppedState;
}

int DSGraph::createGraph( DSGParam oParams )
{
	m_oGraphParam = oParams;
	return m_pState->createGraph(oParams);
}

int DSGraph::runGraph()
{
	return m_pState->runGraph();
}

int DSGraph::pauseGraph()
{
	return m_pState->pauseGraph();
}

int DSGraph::stopGraph()
{
	return m_pState->stopGraph();
}

bool DSGraph::isRunning()
{
	return m_pState->isRunning();
}

bool DSGraph::isPaused()
{
	return m_pState->isPaused();
}

bool DSGraph::isStopped()
{
	return m_pState->isStopped();
}

DSState * DSGraph::runningState() const
{
	return m_pRunningState;
}

DSState * DSGraph::pausedState() const
{
	return m_pPausedState;
}

DSState * DSGraph::stoppedState() const
{
	return m_pStoppedState;
}

DSState * DSGraph::initState() const
{
	return m_pInitState;
}

IBaseFilter * DSGraph::createFilter( CLSID clsid,LPCWSTR pInFilterName,const char *pName /*= NULL*/ )
{
	HRESULT hr;
	IBaseFilter *pFilter= NULL;
	if(!pName)//已知filter的CLSID，可直接获得
	{
		CoCreateInstance(clsid,NULL,CLSCTX_INPROC_SERVER,IID_IBaseFilter,(void **)&pFilter);

		hr = m_pGraph->AddFilter(pFilter,pInFilterName);	//直接将该filter添加到graph中
		if(hr !=S_OK)
			return NULL;
		return pFilter;
	}
	else
	{
		IEnumMoniker *pEmCat = 0;
		ICreateDevEnum *pCreateDevEnum = NULL;
		int nClasses=0;

		// Create an enumerator
		hr = CoCreateInstance(CLSID_SystemDeviceEnum, NULL, CLSCTX_INPROC_SERVER,IID_ICreateDevEnum, (void**)&pCreateDevEnum);
		if(!SUCCEEDED(hr))
			return NULL;
		if (FAILED(hr))
		{
			return pFilter;
		}
		hr = pCreateDevEnum->CreateClassEnumerator(clsid, &pEmCat, 0);
		if(!SUCCEEDED(hr))
			return NULL;

		if(hr == S_OK)
		{
			ULONG	fetched = 0;
			IMoniker *	moniker = NULL;
			char friendlyName[256];
			while(!pFilter && SUCCEEDED(pEmCat->Next(1,&moniker,&fetched))&&fetched)
			{
				if(moniker)
				{
					IPropertyBag	*	propertyBag = NULL;
					VARIANT				name;
					friendlyName[0]	 = 0;
					hr = moniker->BindToStorage(0,0,IID_IPropertyBag,(void **)&propertyBag);
					if(SUCCEEDED(hr))
					{
						name.vt = VT_BSTR;
						hr = propertyBag->Read(L"FriendlyName",&name,NULL);
					}
					if(SUCCEEDED(hr))
					{
						WideCharToMultiByte(CP_ACP,0,name.bstrVal,-1,friendlyName,256,NULL,NULL);
						if(strcmp(friendlyName,pName) == 0)
						{
							moniker->BindToObject(0,0,IID_IBaseFilter,(void **)&pFilter);
						}
					}
					if(propertyBag)
					{
						propertyBag->Release();
						propertyBag = NULL;
					}
					moniker->Release();
				}
			}
			pEmCat->Release();
		}
		//直接将该filter添加到graph中
		hr = m_pGraph->AddFilter(pFilter,pInFilterName);
		if(hr !=S_OK)
			return NULL;
	}
	return pFilter;
}

int DSGraph::connectPin( IPin *pInIn,IPin *pInOut )
{
	HRESULT hr;
	hr = m_pGraph->Connect(pInOut,pInIn);
	if(hr == VFW_E_NOT_IN_GRAPH)
		return addError("Error connectPin!VFW_E_NOT_IN_GRAPH:At least one of the filters is not in the filter graph.");
	if(hr == VFW_E_CANNOT_CONNECT)
		return addError("Error connectPin!VFW_E_CANNOT_CONNECT:No combination of intermediate filters could be found to make the connection.");
	if(hr == E_POINTER)
		return addError("Error connectPin!E_POINTER:NULL pointer argument.");
	if(hr == E_ABORT)
		return addError("Error connectPin!E_ABORT:Operation aborted.");
	if(hr == VFW_S_PARTIAL_RENDER)
		return addError("Error connectPin!VFW_S_PARTIAL_RENDER:Partial success; some of the streams from this pin use an unsupported format.");
	if(hr!=S_OK)
		return addError("Error connectPin!Unknown error");
	return 1;
}

IBaseFilter * DSGraph::getFilter( const char *pName /*= NULL*/ )
{
	IEnumFilters *pEnum = NULL;
	IBaseFilter *pFilter;
	ULONG cFetched;
	if(pName == NULL)
		return NULL;
	HRESULT hr = m_pGraph->EnumFilters(&pEnum);
	if (FAILED(hr)) return NULL;

	while(pEnum->Next(1, &pFilter, &cFetched) == S_OK)
	{
		FILTER_INFO FilterInfo;
		hr = pFilter->QueryFilterInfo(&FilterInfo);
		if (FAILED(hr))
		{
			continue;  // Maybe the next one will work.
		}
		char szName[MAX_FILTER_NAME];
		int cch = WideCharToMultiByte(CP_ACP, 0, FilterInfo.achName,MAX_FILTER_NAME, szName, MAX_FILTER_NAME, 0, 0);
		// The FILTER_INFO structure holds a pointer to the Filter Graph
		// Manager, with a reference count that must be released.
		if (FilterInfo.pGraph != NULL)
		{
			FilterInfo.pGraph->Release();
		}
		pFilter->Release();
		if(strcmp(pName,szName) == 0)
		{
			return pFilter;
		}
	}
	return NULL;
}

IPin		* DSGraph::getPin( IBaseFilter *inFilter,BOOL inInput /*= TRUE*/,const char * inPartialName /*= NULL*/ )
{
	PIN_DIRECTION direction = inInput ? PINDIR_INPUT : PINDIR_OUTPUT;
	IPin * foundPin = NULL;

	if (inFilter)
	{
		IEnumPins * pinEnum = NULL;
		if (SUCCEEDED(inFilter->EnumPins(&pinEnum)))
		{
			pinEnum->Reset();

			IPin * pin = NULL;
			ULONG fetchCount = 0;
			while (!foundPin && SUCCEEDED(pinEnum->Next(1, &pin, &fetchCount)) && fetchCount)
			{
				if (pin)
				{
					PIN_INFO pinInfo;
					if (SUCCEEDED(pin->QueryPinInfo(&pinInfo)))
					{
						if (pinInfo.dir == direction)
						{
							// Ignore the pin name
							if (!inPartialName)
							{
								pin->AddRef();
								foundPin = pin;
							}
							else
							{
								char pinName[128];
								::WideCharToMultiByte(CP_ACP, 0, pinInfo.achName, 
									-1,	pinName, 128, NULL, NULL);
								if (::strstr(pinName, inPartialName))
								{
									pin->AddRef();
									foundPin = pin;
								}
							}
						}
						pinInfo.pFilter->Release();
					}
					pin->Release();
				}
			}
			pinEnum->Release();
		}
	}

	// We don't keep outstanding reference count
	if (foundPin)
	{
		foundPin->Release();
	}
	return foundPin;
}

IPin		* DSGraph::getUnconnectedPin( IBaseFilter *pFilter, BOOL inInPut /*= TRUE*/ )
{
	IEnumPins *pEnum = 0;
	IPin *pPin = 0;
	PIN_DIRECTION PinDir;
	HRESULT hr = pFilter->EnumPins(&pEnum);

	if (FAILED(hr))
	{
		return NULL;
	}
	if(inInPut == TRUE)
		PinDir = PINDIR_INPUT;
	else
		PinDir = PINDIR_OUTPUT;

	while (pEnum->Next(1, &pPin, NULL) == S_OK)
	{
		PIN_DIRECTION ThisPinDir;
		pPin->QueryDirection(&ThisPinDir);
		if (ThisPinDir == PinDir)
		{
			IPin *pTmp = 0;
			hr = pPin->ConnectedTo(&pTmp);
			if (SUCCEEDED(hr))  // Already connected, not the pin we want.
			{
				pTmp->Release();
			}
			else  // Unconnected, this is the pin we want.
			{
				pEnum->Release();
				return pPin;
			}
		}
		pPin->Release();
	}
	pEnum->Release();
	// Did not find a matching pin.
	return NULL;
}

void		* DSGraph::getFilterInterface( IBaseFilter *pInFilter,REFIID inInterfaceID )
{
	HRESULT	hr;
	void *pInterface;
	hr = pInFilter->QueryInterface(inInterfaceID,&pInterface);
	return pInterface;
}

void		* DSGraph::getFilterInterface( const char * pInFilterName,REFIID inInterfaceID )
{
	IBaseFilter *pFilter = getFilter(pInFilterName);
	if(!pFilter)
	{
		return NULL;
	}
	return getFilterInterface(pFilter,inInterfaceID);
}

void		* DSGraph::getPinInterface( IPin *pInPin,REFIID inInterfaceID )
{
	HRESULT hr;
	void *pInterface;
	hr = pInPin->QueryInterface(inInterfaceID,&pInterface);
	return pInterface;
}

int DSGraph::delGraph()
{
	if(!m_pGraph)
		return 1;
	if(dw_debug)
		removeFromRot();
	//删除整个filter Graph
	IEnumFilters *pEnum = NULL;
	HRESULT hr = m_pGraph->EnumFilters(&pEnum);
	if (SUCCEEDED(hr))
	{
		IBaseFilter *pFilter = NULL;
		while (S_OK == pEnum->Next(1, &pFilter, NULL))
		{
			// Remove the filter.
			m_pGraph->RemoveFilter(pFilter);
			// Reset the enumerator.
			pEnum->Reset();
			pFilter->Release();
		}
		pEnum->Release();
	}

	//剩余的释放工作
	m_pGraph->Release();
	m_pGraph = NULL;
	m_pMediaCtrl->Release();
	m_pMediaEvent->Release();
	m_pMediaSeek->Release();
	if(m_pVMRCtrl)
		m_pVMRCtrl->Release();
	if(m_pVolumeCtrl)
		m_pVolumeCtrl->Release();
	m_pMediaEvent = NULL;
	m_pMediaCtrl = NULL;
	m_pMediaSeek = NULL;
	m_pVMRCtrl = NULL;
	m_pVolumeCtrl = NULL;

	dw_debug = 0;
	m_pState = m_pInitState;
	return 1;
}

HRESULT DSGraph::addToRot()
{
	IMoniker * pMoniker;
	IRunningObjectTable *pROT;
	if (FAILED(GetRunningObjectTable(0, &pROT))) {
		return E_FAIL;
	}
	WCHAR wsz[256];
	wsprintfW(wsz, L"FilterGraph %08x pid %08x", (DWORD_PTR)m_pGraph, GetCurrentProcessId());
	HRESULT hr = CreateItemMoniker(L"!", wsz, &pMoniker);
	if (SUCCEEDED(hr)) {
		hr = pROT->Register(ROTFLAGS_REGISTRATIONKEEPSALIVE, m_pGraph,
			pMoniker, &dw_debug);
		pMoniker->Release();
	}
	pROT->Release();
	return hr;
}

void DSGraph::removeFromRot()
{
	IRunningObjectTable *pROT;
	if (SUCCEEDED(GetRunningObjectTable(0, &pROT))) {
		pROT->Revoke(dw_debug);
		pROT->Release();
	}
}