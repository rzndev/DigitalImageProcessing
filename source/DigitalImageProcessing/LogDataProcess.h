#pragma once
#include "dataprocess.h"
class CLogDataProcess :
	public CDataProcess
{
public:
	CLogDataProcess(void);
	virtual ~CLogDataProcess(void);
	// логарифмическое преобразование
	virtual double Process(double x);
	// Коэффициент логарифмического преобразования
	double m_C;
	// Получить информацию о настройках
	virtual CString GetInfo(void);
	// инициализирующий конструктор
	CLogDataProcess(CLogDataProcess* dataProcess);
};

