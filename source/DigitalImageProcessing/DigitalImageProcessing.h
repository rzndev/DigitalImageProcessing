
// DigitalImageProcessing.h : главный файл заголовка для приложения DigitalImageProcessing
//
#pragma once

#ifndef __AFXWIN_H__
	#error "включить stdafx.h до включения этого файла в PCH"
#endif

#include "resource.h"       // основные символы


// CDigitalImageProcessingApp:
// О реализации данного класса см. DigitalImageProcessing.cpp
//

class CDigitalImageProcessingApp : public CWinApp
{
public:
	CDigitalImageProcessingApp();


// Переопределение
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// Реализация
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CDigitalImageProcessingApp theApp;
