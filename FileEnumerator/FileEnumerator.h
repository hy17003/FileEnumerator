
// FileEnumerator.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CFileEnumeratorApp: 
// �йش����ʵ�֣������ FileEnumerator.cpp
//

class CFileEnumeratorApp : public CWinApp
{
public:
	CFileEnumeratorApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CFileEnumeratorApp theApp;