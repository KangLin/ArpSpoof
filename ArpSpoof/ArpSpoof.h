// ArpSpoof.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CArpSpoofApp:
// �йش����ʵ�֣������ ArpSpoof.cpp
//

class CArpSpoofApp : public CWinApp
{
public:
	CArpSpoofApp();

// ��д
	public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CArpSpoofApp theApp;