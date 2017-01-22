#pragma once
#include "dataprocess.h"
class CPowDataProcess :
	public CDataProcess
{
public:
	CPowDataProcess(void);
	virtual ~CPowDataProcess(void);
	// ��������� ��������������
	virtual double Process(double x);
	// ��������� C
	double m_C;
	// ��������� �����
	double m_gamma;
	// �������� ���������� � ����������
	virtual CString GetInfo(void);
	// �����������, ���������������� ����������
	CPowDataProcess(CPowDataProcess* dataProcess);
};

