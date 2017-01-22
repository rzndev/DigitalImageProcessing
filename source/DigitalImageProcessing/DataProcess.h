#pragma once
class CDataProcess
{
public:
	CDataProcess(void);
	virtual ~CDataProcess(void);
	// Функция обработки значения яркости x. Возвращает обработанное значение
	virtual double Process(double x);
	// тип обработки
	int m_typeProcess;
	// Получить информацию о настройках
	virtual CString GetInfo(void);
protected:
	CString m_textInfo;
public:
	// Построить таблицу преобразований для 0<=x<=255 и целочисленных x
	void BuildTableConversion(void);
protected:
	int* m_table;
public:
	// Преобразования с использованием таблицы
	virtual double ProcessTable(double x);
};

