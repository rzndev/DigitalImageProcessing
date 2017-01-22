#pragma once
#include "dataprocess.h"
// класс, обеспечивающий инверсию цвета
class CNegativeDataProcess :
	public CDataProcess
{
public:
	CNegativeDataProcess(void);
	virtual ~CNegativeDataProcess(void);
	virtual double Process(double x);
	// Получить параметры
	virtual CString GetInfo(void);
};

