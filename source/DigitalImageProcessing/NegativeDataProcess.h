#pragma once
#include "dataprocess.h"
// �����, �������������� �������� �����
class CNegativeDataProcess :
	public CDataProcess
{
public:
	CNegativeDataProcess(void);
	virtual ~CNegativeDataProcess(void);
	virtual double Process(double x);
	// �������� ���������
	virtual CString GetInfo(void);
};

