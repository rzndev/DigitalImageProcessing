#pragma once
#include "dataprocess.h"
class CLogDataProcess :
	public CDataProcess
{
public:
	CLogDataProcess(void);
	virtual ~CLogDataProcess(void);
	// ��������������� ��������������
	virtual double Process(double x);
	// ����������� ���������������� ��������������
	double m_C;
	// �������� ���������� � ����������
	virtual CString GetInfo(void);
	// ���������������� �����������
	CLogDataProcess(CLogDataProcess* dataProcess);
};

