
// DigitalImageProcessing.h : ������� ���� ��������� ��� ���������� DigitalImageProcessing
//
#pragma once

#ifndef __AFXWIN_H__
	#error "�������� stdafx.h �� ��������� ����� ����� � PCH"
#endif

#include "resource.h"       // �������� �������


// CDigitalImageProcessingApp:
// � ���������� ������� ������ ��. DigitalImageProcessing.cpp
//

class CDigitalImageProcessingApp : public CWinApp
{
public:
	CDigitalImageProcessingApp();


// ���������������
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// ����������
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CDigitalImageProcessingApp theApp;
