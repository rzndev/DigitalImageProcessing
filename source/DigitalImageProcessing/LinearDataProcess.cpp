#include "StdAfx.h"
#include "LinearDataProcess.h"


CLinearDataProcess::CLinearDataProcess(void)
{
	m_typeProcess = 4;
	SCoefs coef;
	coef.x_begin = 0;
	coef.k = 1;
	coef.b = 0;
	m_coefs.push_back(coef);

	coef.x_begin = 100;
	coef.k = 1;
	coef.b = 50;
	m_coefs.push_back(coef);

	coef.x_begin = 200;
	coef.k = 1;
	coef.b = 0;
	m_coefs.push_back(coef);
}


CLinearDataProcess::~CLinearDataProcess(void)
{
}


// кусочно-линейное преобразование
double CLinearDataProcess::Process(double x)
{
	double result = 0.0;
	vector<SCoefs>::iterator it;
	vector<SCoefs>::iterator it_prev = m_coefs.begin();
	for(it = m_coefs.begin(); it != m_coefs.end(); it++)
	{
		if (x <= it->x_begin)
			break;
		it_prev = it;
	}
	if (m_coefs.size() > 0)
	{
		result = it_prev->k * x + it_prev->b;
		if (result < 0.0) result = 0.0;
		if (result > 255.0) result = 255.0;
	}
	return result;
}


// Получить информацию о настройках
CString CLinearDataProcess::GetInfo(void)
{
	return CString(_T("Кусочно-линейное"));
}


// Инициализирующий конструктор
CLinearDataProcess::CLinearDataProcess(CLinearDataProcess* dataProcess)
{
	if (NULL == dataProcess)
	{
		m_typeProcess = 4;
		SCoefs coef;
		coef.x_begin = 0;
		coef.k = 1;
		coef.b = 0;
		m_coefs.push_back(coef);

		coef.x_begin = 100;
		coef.k = 1;
		coef.b = 50;
		m_coefs.push_back(coef);

		coef.x_begin = 200;
		coef.k = 1;
		coef.b = 0;
		m_coefs.push_back(coef);
	} else
	{
		m_typeProcess = 4;
		vector<SCoefs>::iterator it;
		for(it = dataProcess->m_coefs.begin(); it != dataProcess->m_coefs.end(); it++)
		{
			m_coefs.push_back(*it);
		}
	}
}


// функция сортировки коэффициентов
void CLinearDataProcess::SortCoefs(void)
{
	SCoefs tmp_val; // значение, используемое для обмена данными
	int sz = m_coefs.size();
	int i,j,mx;
	for (i=0; i < sz-1; i++)
	{
		mx = i;
		for(j = i + 1; j < sz; j++)
		{
			if (m_coefs[j].x_begin < m_coefs[mx].x_begin) mx = j;
		}
		tmp_val = m_coefs[mx];
		m_coefs[mx] = m_coefs[i];
		m_coefs[i] = tmp_val;
	}
}


void CLinearDataProcess::AddCoefs(SCoefs& val)
{
	// проверяем на допустимость значения
	if (val.x_begin < 0) return;
	if (val.x_begin > 255) return;
	// добавляем элемент в список
	m_coefs.push_back(val);
	// сортируем список
	SortCoefs();
}


// удалить параметры, хранящиеся в элементе с индексом id
void CLinearDataProcess::RemoveAtID(int id)
{
	// проверяем на допустимость значения
	if ((id < 0) || (id >= m_coefs.size())) return;
	vector<SCoefs>::iterator it;
	it = m_coefs.begin() + id;
	m_coefs.erase(it);
}
