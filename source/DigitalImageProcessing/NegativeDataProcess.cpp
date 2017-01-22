#include "StdAfx.h"
#include "NegativeDataProcess.h"


CNegativeDataProcess::CNegativeDataProcess(void)
{
	m_typeProcess = 1;
}


CNegativeDataProcess::~CNegativeDataProcess(void)
{
}


double CNegativeDataProcess::Process(double x)
{
	if (x < 0) x = 0.0;
	if (x > 255.0) x = 255.0;
	return 255.0 - x;
}


// Получить параметры
CString CNegativeDataProcess::GetInfo(void)
{
	return CString(_T("негатив"));
}
