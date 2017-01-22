#include "StdAfx.h"
#include "LogDataProcess.h"
#include <math.h>

CLogDataProcess::CLogDataProcess(void)
	: m_C(30)
{
	m_typeProcess = 2;
}


CLogDataProcess::~CLogDataProcess(void)
{
}


// логарифмическое преобразование
double CLogDataProcess::Process(double x)
{
	x = x / 255;
	double result = m_C*log(x+1)/log(2.0);
	result = result * 255.0;
	if (result < 0.0) result = 0.0;
	if (result > 255.0) result = 255.0;
	return result;
}


// Получить информацию о настройках
CString CLogDataProcess::GetInfo(void)
{
	m_textInfo.Format(_T("логарифмическое, C=%f"), m_C);
	return m_textInfo;
}


// инициализирующий конструктор
CLogDataProcess::CLogDataProcess(CLogDataProcess* dataProcess)
{
	if (NULL == dataProcess)
	{
		m_typeProcess = 2;
		m_C = 30;
	} else
	{
		m_typeProcess = 2;
		m_C = dataProcess->m_C;
	}
}
