#pragma once
#include "dataprocess.h"
class CPowDataProcess :
	public CDataProcess
{
public:
	CPowDataProcess(void);
	virtual ~CPowDataProcess(void);
	// степенное преобразование
	virtual double Process(double x);
	// константа C
	double m_C;
	// константа гамма
	double m_gamma;
	// Получить информацию о настройках
	virtual CString GetInfo(void);
	// конструктор, инициализирующий параметрыъ
	CPowDataProcess(CPowDataProcess* dataProcess);
};

