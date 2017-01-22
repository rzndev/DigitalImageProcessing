#pragma once
#include <vector>
#include "DataProcess.h"
using namespace std;

class CImageData
{
public:
	CImageData(void);
	~CImageData(void);
	// Загрузить bitmap файл в класс
	bool ReadDIBFile(LPCTSTR lpszFileName);
protected:
	WORD DIBNumColors(LPBITMAPINFOHEADER lpBmpInfo);
public:
	// Регион, занимаемый изображением
	CRect m_region;
	// Размер изображения
	LONG m_lHeight;
	// Ширина изображения
	LONG m_lWidth;
	// набор цветовых каналов изображения
	unsigned char* m_colorData[3];
	// Освободить ресурсы, занимаемые классом
	void Clear(void);
	void CreateBitmap(void);
	// Данные о хранимом изображении
	BITMAPINFO m_bitmapinfo;
	void* m_pBits;
	// Получить фрагмент изображения из указанной области
	CImageData* GetImageFromRegion(CRect region);
	// Перенести фрагмент изображения из image в изображение, хранящееся в данном классе
	void CombineImage(CImageData* image);
	// обработать изображение функцией, определяемой классом func
	void Process(CDataProcess* func);
	// Создать изображение в масштабе
	bool CreateSizedBitmap(CSize size, CDC *pDC);
	void CreateDIBFile(LPCTSTR lpFileName);
};

