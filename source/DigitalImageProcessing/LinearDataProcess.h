#pragma once
#include "dataprocess.h"
#include <math.h>
#include <vector>

using namespace std;

struct SCoefs
{
	int x_begin; // ������ ��������� �������� ������������� ��������� ���������
	double k,b; // ������������ ��������� ���������
};

class CLinearDataProcess :
	public CDataProcess
{
public:
	CLinearDataProcess(void);
	virtual ~CLinearDataProcess(void);
	// �������-�������� ��������������
	virtual double Process(double x);
	// �������� ���������� � ����������
	virtual CString GetInfo(void);
	// ���������������� �����������
	CLinearDataProcess(CLinearDataProcess* dataProcess);
	// �������� �����������
	vector<SCoefs> m_coefs;
	// ������� ���������� �������������
	void SortCoefs(void);
	void AddCoefs(SCoefs& val);
	// ������� ��������� � �������� id
	void RemoveAtID(int id);
};

