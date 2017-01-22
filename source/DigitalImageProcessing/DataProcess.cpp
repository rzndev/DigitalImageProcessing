#include "StdAfx.h"
#include "DataProcess.h"


CDataProcess::CDataProcess(void)
	: m_typeProcess(0)
	, m_textInfo(_T(""))
	
{
	int i;
	m_table = new int[256];
	for(i = 0; i < 256; i++) m_table[i] = 0.0;
}


CDataProcess::~CDataProcess(void)
{
	delete[] m_table;
}


// Функция обработки значения яркости x. Возвращает обработанное значение
double CDataProcess::Process(double x)
{
	return x;
}


// Получить информацию о настройках
CString CDataProcess::GetInfo(void)
{
	return CString(_T("Без обработки"));
}


// Построить таблицу преобразований для 0<=x<=255 и целочисленных x
void CDataProcess::BuildTableConversion(void)
{
	int i;
	for(i = 0; i < 256; i++) m_table[i] = (int)Process((double)i);
}


// Преобразования с использованием таблицы
double CDataProcess::ProcessTable(double x)
{
	if (x < 0.0) x = 0.0;
	if (x > 255.0) x = 255.0;
	return m_table[int(x)];
}
