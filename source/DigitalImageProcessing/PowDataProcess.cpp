#include "StdAfx.h"
#include "PowDataProcess.h"
#include <math.h>

CPowDataProcess::CPowDataProcess(void)
	: m_C(0.06)
	, m_gamma(1.5)
{
	m_typeProcess = 3;
}


CPowDataProcess::~CPowDataProcess(void)
{
}


// степенное преобразование
double CPowDataProcess::Process(double x)
{
	x = x / 255.0;
	double result = m_C*pow(x,m_gamma);
	result = result * 255.0;
	if (result < 0.0) result = 0.0;
	if (result > 255.0) result = 255.0;
	return result;
}


// ѕолучить информацию о настройках
CString CPowDataProcess::GetInfo(void)
{
	m_textInfo.Format(_T("степенное, C=%f, gamma=%f"), m_C, m_gamma);
	return m_textInfo;
}


// конструктор, инициализирующий параметры
CPowDataProcess::CPowDataProcess(CPowDataProcess* dataProcess)
{
	if (NULL == dataProcess)
	{
		m_C = 0.06;
		m_gamma = 1.5;
		m_typeProcess = 3;
	} else
	{
		m_C = dataProcess->m_C;
		m_gamma = dataProcess->m_gamma;
		m_typeProcess = 3;
	}
}
