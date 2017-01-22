#pragma once
#include "dataprocess.h"
#include <math.h>
#include <vector>

using namespace std;

struct SCoefs
{
	int x_begin; // начало диапазона действия коэффициентов линейного уравнения
	double k,b; // коэффициенты линейного уравнения
};

class CLinearDataProcess :
	public CDataProcess
{
public:
	CLinearDataProcess(void);
	virtual ~CLinearDataProcess(void);
	// кусочно-линейное преобразование
	virtual double Process(double x);
	// Получить информацию о настройках
	virtual CString GetInfo(void);
	// Инициализирующий конструктор
	CLinearDataProcess(CLinearDataProcess* dataProcess);
	// заданная зависимость
	vector<SCoefs> m_coefs;
	// функция сортировки коэффициентов
	void SortCoefs(void);
	void AddCoefs(SCoefs& val);
	// удалить параметры в элементе id
	void RemoveAtID(int id);
};

