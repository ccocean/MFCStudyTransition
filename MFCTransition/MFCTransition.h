
// MFCTransition.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CMFCTransitionApp: 
// �йش����ʵ�֣������ MFCTransition.cpp
//

class CMFCTransitionApp : public CWinApp
{
public:
	CMFCTransitionApp();
	HANDLE m_hMutex;
// ��д
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CMFCTransitionApp theApp;