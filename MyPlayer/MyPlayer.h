// MyPlayer.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CMyPlayerApp:
// �йش����ʵ�֣������ MyPlayer.cpp
//

class CMyPlayerApp : public CWinApp
{
public:
	CMyPlayerApp();

// ��д
	public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CMyPlayerApp theApp;