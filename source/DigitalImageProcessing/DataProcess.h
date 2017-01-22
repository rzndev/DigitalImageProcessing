#pragma once
class CDataProcess
{
public:
	CDataProcess(void);
	virtual ~CDataProcess(void);
	// ������� ��������� �������� ������� x. ���������� ������������ ��������
	virtual double Process(double x);
	// ��� ���������
	int m_typeProcess;
	// �������� ���������� � ����������
	virtual CString GetInfo(void);
protected:
	CString m_textInfo;
public:
	// ��������� ������� �������������� ��� 0<=x<=255 � ������������� x
	void BuildTableConversion(void);
protected:
	int* m_table;
public:
	// �������������� � �������������� �������
	virtual double ProcessTable(double x);
};

