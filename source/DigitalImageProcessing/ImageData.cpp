#include "StdAfx.h"
#include "ImageData.h"

#include "ImageResize.h"

/*
 * Маркер начала bitmap файла - используется при записи bitmap  в файл
 */
#define DIB_HEADER_MARKER   ((WORD) ('M' << 8) | 'B')

CImageData::CImageData(void)
	: m_lHeight(0)
	, m_lWidth(0)
	, m_pBits(NULL)
{
	m_colorData[0] = NULL;
	m_colorData[1] = NULL;
	m_colorData[2] = NULL;
}


CImageData::~CImageData(void)
{
	Clear();
}


// Загрузить bitmap файл в класс
bool CImageData::ReadDIBFile(LPCTSTR lpszFileName)
{
	int row, col, i;
	int idx; // индекс точки в массиве m_colorData[k]
	CFile file;
	CFileException fe;
	Clear(); // перед началом чтения данных очищаем ранее содержащееся изображение
	if (!file.Open(lpszFileName, CFile::modeRead | CFile::shareDenyWrite, &fe))
	{
		return false; // ошибка при открытии файла
	}

	BITMAPFILEHEADER bmfHeader;
	UINT nBitsSize;
	
	unsigned char* pDIB; // указатель на bitmap изображение в памяти

	/*
	 * get length of DIB in bytes for use when reading
	 */

	nBitsSize = (UINT)file.GetLength();

	/*
	 * Загружаем заголовок bitmap файла и проверяем его правильность.
	 */
	if (file.Read((LPSTR)&bmfHeader, sizeof(bmfHeader)) != sizeof(bmfHeader))
		return false;

	if (bmfHeader.bfType != DIB_HEADER_MARKER)
		return false;

	/*
	 * Выделяем память для bitmap
	 */
	pDIB = new unsigned char[nBitsSize - sizeof(BITMAPFILEHEADER)];

	/*
	 * Go read the bits.
	 */
	if (file.Read(pDIB, nBitsSize - sizeof(BITMAPFILEHEADER)) !=
		nBitsSize - sizeof(BITMAPFILEHEADER) )
	{
		delete[] pDIB;
		return false;
	}

	// заголовок bmp файла
	LPBITMAPINFOHEADER lpBmpInfo = (LPBITMAPINFOHEADER)pDIB;
	
	// определяем размер палитры цветов
	WORD PaletteSize = DIBNumColors(lpBmpInfo) * sizeof(RGBQUAD);

	RGBQUAD* paletteData = NULL; // данные палитры

	if (PaletteSize != 0)
	{
		paletteData = &((LPBITMAPINFO)lpBmpInfo)->bmiColors[0];
	}

	// выделяем память под изображение
	m_lHeight = abs(lpBmpInfo->biHeight);
	m_lWidth = lpBmpInfo->biWidth;
	for(i = 0; i < 3; i++)
	{
		m_colorData[i] = new unsigned char[m_lHeight * m_lWidth];
	}

	// заполняем содержимое класса содержимым изображения
	long stride = stride = ((((lpBmpInfo->biWidth * lpBmpInfo->biBitCount) + 31) & ~31) >> 3); // размер строки изображения в исходном файле. Взято из описания в MSDN
	unsigned char* pBitmap = (unsigned char*)lpBmpInfo + *(LPDWORD)lpBmpInfo + PaletteSize;     // начало битового образа в изображении  
	
	for(row = 0; row < m_lHeight; row++)
	{
		unsigned char* row_data; // данные строки
		if (lpBmpInfo->biHeight < 0)
		{
			// Если biHeight отрицательна, bitmap сохранен "сверху вниз"
			row_data = pBitmap + row*stride; // вычилсяем адрес строки row

		} else
		{
			// Если biHeight отрицательна, bitmap сохранен "снизу вверх"
			row_data = pBitmap + (m_lHeight-row-1)*stride; // вычилсяем адрес строки row
		}
		// считываем данные из строки
		for(col = 0; col < m_lWidth; col++)
		{
			// Организация данных зависит от количества бит на точку в bitmap файле
			idx = row * m_lWidth + col;
			switch(lpBmpInfo->biBitCount)
			{
			case 8:
				m_colorData[0][idx] = paletteData[row_data[col]].rgbRed;
				m_colorData[1][idx] = paletteData[row_data[col]].rgbGreen;
				m_colorData[2][idx] = paletteData[row_data[col]].rgbBlue;
				break;
			case 16:
				m_colorData[0][idx] = (row_data[col*2] && 0xF8) >> 3;
				m_colorData[0][idx] = (row_data[col*2] && 0x07) << 3 | row_data[col*2+1] && 0xC0 >> 6;
				m_colorData[0][idx] = (row_data[col*2+1] && 0x3E) >> 1;
				break;
			case 24:
				m_colorData[0][idx] = row_data[col*3];
				m_colorData[1][idx] = row_data[col*3+1];
				m_colorData[2][idx] = row_data[col*3+2];
				break;
			case 32:
				m_colorData[0][idx] = row_data[col*4];
				m_colorData[1][idx] = row_data[col*4+1];
				m_colorData[2][idx] = row_data[col*4+2];
				break;
			}
		}
	}
	delete[] pDIB;
	m_region = CRect(0,0,m_lWidth-1, m_lHeight-1); // для загруженного изображения считаем, что оно занимает всю область
	return true;
}


WORD CImageData::DIBNumColors(LPBITMAPINFOHEADER lpBmpInfo)
{
	WORD wBitCount;  // DIB bit count

	/*  If this is a Windows-style DIB, the number of colors in the
	 *  color table can be less than the number of bits per pixel
	 *  allows for (i.e. lpbi->biClrUsed can be set to some value).
	 *  If this is the case, return the appropriate value.
	 */

	DWORD dwClrUsed;
	dwClrUsed = lpBmpInfo->biClrUsed;
		if (dwClrUsed != 0)
			return (WORD)dwClrUsed;
	

	/*  Calculate the number of colors in the color table based on
	 *  the number of bits per pixel for the DIB.
	 */
	wBitCount = lpBmpInfo->biBitCount;

	/* Возвращает количество цветов в зависимости от битов на точку */
	switch (wBitCount)
	{
		case 1:
			return 2;

		case 4:
			return 16;

		case 8:
			return 256;

		default:
			return 0;
	}
}


// Освободить ресурсы, занимаемые классом
void CImageData::Clear(void)
{
	m_lHeight = 0;
	m_lWidth = 0;
	m_region = CRect(0,0,0,0);
	// осовбодить битовый образ изображения, если есть
	if(NULL != m_pBits)
	{
		delete[] m_pBits;
		m_pBits= NULL;
	}
	// освобождаем память, занятую данными о цвете изображения
	for(int i = 0; i < 3; i++)
		if (NULL != m_colorData[i])
		{
			delete[] m_colorData[i];
			m_colorData[i] = NULL;
		}
}


void CImageData::CreateBitmap(void)
{
	int row, col, idx;
	unsigned char *bits;
	unsigned char* row_data;
	if(NULL != m_pBits)
	{
		delete[] m_pBits;
		m_pBits= NULL;
	}

	BITMAPINFOHEADER bmpinfo;
	memset(&bmpinfo, 0, sizeof(BITMAPINFOHEADER));
	bmpinfo.biSize = sizeof(BITMAPINFOHEADER);
	bmpinfo.biWidth = m_lWidth;
	bmpinfo.biHeight = m_lHeight;
	bmpinfo.biBitCount = 24;
	bmpinfo.biPlanes = 1;
	bmpinfo.biCompression = BI_RGB;

	long stride = ((((bmpinfo.biWidth * bmpinfo.biBitCount) + 31) & ~31) >> 3);
	bits = new unsigned char[stride * m_lHeight];
	// заполняем массив данными
	for(row = 0; row < m_lHeight; row++)
	{
		row_data = bits + (m_lHeight-row-1)*stride; // вычилсяем адрес строки row
		for(col = 0; col < m_lWidth; col++)
		{
			idx = row * m_lWidth + col;
			row_data[3*col] = m_colorData[0][idx];
			row_data[3*col+1] = m_colorData[1][idx];
			row_data[3*col+2] = m_colorData[2][idx];
		}
	}
	m_pBits = bits;
	memcpy(&m_bitmapinfo, &bmpinfo, sizeof(BITMAPINFOHEADER));
}


// Получить фрагмент изображения из указанной области
CImageData* CImageData::GetImageFromRegion(CRect region)
{
	int row, col;
	CImageData* data = new CImageData;
	// определяем область, пересекающуюся между region и m_region
	region.right = min(region.right, m_region.right);
	region.left = max(region.left, m_region.left);
	region.top = max(region.top, m_region.top);
	region.bottom = min(region.bottom, m_region.bottom);

	// выделяем память и заполняем служебные данные для структуры data
	data->m_lHeight = region.Height()+1;
	data->m_lWidth = region.Width()+1;
	data->m_colorData[0] = new unsigned char[data->m_lHeight * data->m_lWidth];
	data->m_colorData[1] = new unsigned char[data->m_lHeight * data->m_lWidth];
	data->m_colorData[2] = new unsigned char[data->m_lHeight * data->m_lWidth];
	data->m_pBits = NULL;
	memset(&data->m_bitmapinfo, 0,sizeof(data->m_bitmapinfo));
	data->m_region = region; // в сформированную структуру данных сохраняем информацию о регионе, из которого получен фрагмент изображения
	// формируем данные для региона
	int idx_source, idx_destination;
	for(row = 0; row < data->m_lHeight; row++)
		for(col = 0; col < data->m_lWidth; col++)
		{
			idx_destination = row * data->m_lWidth + col;
			idx_source = (row + region.top) * m_lWidth + (col + region.left);
			data->m_colorData[0][idx_destination] = m_colorData[0][idx_source];
			data->m_colorData[1][idx_destination] = m_colorData[1][idx_source];
			data->m_colorData[2][idx_destination] = m_colorData[2][idx_source];
		}
	return data;
}


// Перенести фрагмент изображения из image в изображение, хранящееся в данном классе
void CImageData::CombineImage(CImageData* image)
{
	int row, col;
	int idx_source, idx_destination;
	if ((m_lWidth == 0) || (m_lHeight ==0)) return;
	CRect region = image->m_region;
	for(row = 0; row < image->m_lHeight; row++)
		for(col = 0; col < image->m_lWidth; col++)
		{
			// предотвращаем выходы индексов за допустимые пределы 
			if ((row + region.top) >= m_lHeight) continue;
			if ((col + region.left) >= m_lWidth) continue;
			idx_source = row * image->m_lWidth + col;
			idx_destination = (row + region.top) * m_lWidth + (col + region.left);
			m_colorData[0][idx_destination] = image->m_colorData[0][idx_source];
			m_colorData[1][idx_destination] = image->m_colorData[1][idx_source];
			m_colorData[2][idx_destination] = image->m_colorData[2][idx_source];
		}
}


// обработать изображение функцией, определяемой классом func
void CImageData::Process(CDataProcess* func)
{
	if ((m_lHeight == 0) || (m_lWidth ==0) || (NULL == func)) return;
	int row, col;
	int idx;
	for(row = 0; row < m_lHeight; row++)
		for(col = 0; col < m_lWidth; col++)
		{
			idx = row * m_lWidth + col;
			m_colorData[0][idx] = (unsigned char)func->Process(m_colorData[0][idx]);
			m_colorData[1][idx] = (unsigned char)func->Process(m_colorData[1][idx]);
			m_colorData[2][idx] = (unsigned char)func->Process(m_colorData[2][idx]);
		}
}


// Создать изображение в масштабе и отрисовать его 
bool CImageData::CreateSizedBitmap(CSize size, CDC *pDC)
{
	bool bResult = false;
	BITMAPFILEHEADER fileinfo;
	memset(&fileinfo, 0, sizeof(BITMAPFILEHEADER));

		int row, col, idx;
	unsigned char *bits;
	unsigned char* row_data;
	if(NULL != m_pBits)
	{
		delete[] m_pBits;
		m_pBits= NULL;
	}

	BITMAPINFOHEADER bmpinfo;
	memset(&bmpinfo, 0, sizeof(BITMAPINFOHEADER));
	bmpinfo.biSize = sizeof(BITMAPINFOHEADER);
	bmpinfo.biWidth = m_lWidth;
	bmpinfo.biHeight = m_lHeight;
	bmpinfo.biBitCount = 32;
	bmpinfo.biPlanes = 1;
	bmpinfo.biCompression = BI_RGB;

	long stride = ((((bmpinfo.biWidth * bmpinfo.biBitCount) + 31) & ~31) >> 3);
	fileinfo.bfType = 'MB';
	fileinfo.bfSize = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + stride * m_lHeight;
	fileinfo.bfOffBits = sizeof(BITMAPINFOHEADER) + sizeof(BITMAPFILEHEADER);
	bits = new unsigned char[stride * m_lHeight];
	// заполняем массив данными
	for(row = 0; row < m_lHeight; row++)
	{
		row_data = bits + (m_lHeight-row-1)*stride; // вычилсяем адрес строки row
		for(col = 0; col < m_lWidth; col++)
		{
			idx = row * m_lWidth + col;
			row_data[4*col] = m_colorData[0][idx];
			row_data[4*col+1] = m_colorData[1][idx];
			row_data[4*col+2] = m_colorData[2][idx];
		}
	}
	m_pBits = bits;
	memcpy(&m_bitmapinfo, &bmpinfo, sizeof(BITMAPINFOHEADER));

	// создаем файл, для записи данных
	vector<unsigned char> file(sizeof(fileinfo) + sizeof(BITMAPINFOHEADER) + stride * m_lHeight);
	memcpy(&file[0], &fileinfo, sizeof(BITMAPFILEHEADER));
	memcpy(&file[sizeof(BITMAPFILEHEADER)], &bmpinfo, sizeof(BITMAPINFOHEADER));
	memcpy(&file[sizeof(BITMAPFILEHEADER)+ sizeof(BITMAPINFOHEADER)], bits, stride * m_lHeight);
	// изменяем размер изображения улучшенным способом
	
	CImageResize imgResize;
	imgResize.m_newSize = size;
	bResult = imgResize.ScaleImage(file);
	if (bResult)
		imgResize.DrawBitmapData(&imgResize.m_bmpfile_memory[0],pDC); // отрисовку изображения выполняем только в случае успешного изменения размера изображения
	delete[] bits;
	m_pBits = NULL;
	return bResult;
}


void CImageData::CreateDIBFile(LPCTSTR lpFileName)
{
		bool bResult = false;
	BITMAPFILEHEADER fileinfo;
	memset(&fileinfo, 0, sizeof(BITMAPFILEHEADER));

		int row, col, idx;
	unsigned char *bits;
	unsigned char* row_data;
	if(NULL != m_pBits)
	{
		delete[] m_pBits;
		m_pBits= NULL;
	}

	BITMAPINFOHEADER bmpinfo;
	memset(&bmpinfo, 0, sizeof(BITMAPINFOHEADER));
	bmpinfo.biSize = sizeof(BITMAPINFOHEADER);
	bmpinfo.biWidth = m_lWidth;
	bmpinfo.biHeight = m_lHeight;
	bmpinfo.biBitCount = 32;
	bmpinfo.biPlanes = 1;
	bmpinfo.biCompression = BI_RGB;

	long stride = ((((bmpinfo.biWidth * bmpinfo.biBitCount) + 31) & ~31) >> 3);
	fileinfo.bfType = 'MB';
	fileinfo.bfSize = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + stride * m_lHeight;
	fileinfo.bfOffBits = sizeof(BITMAPINFOHEADER) + sizeof(BITMAPFILEHEADER);
	bits = new unsigned char[stride * m_lHeight];
	// заполняем массив данными
	for(row = 0; row < m_lHeight; row++)
	{
		row_data = bits + (m_lHeight-row-1)*stride; // вычилсяем адрес строки row
		for(col = 0; col < m_lWidth; col++)
		{
			idx = row * m_lWidth + col;
			row_data[4*col] = m_colorData[0][idx];
			row_data[4*col+1] = m_colorData[1][idx];
			row_data[4*col+2] = m_colorData[2][idx];
		}
	}
	m_pBits = bits;
	memcpy(&m_bitmapinfo, &bmpinfo, sizeof(BITMAPINFOHEADER));

	// создаем файл, для записи данных
	vector<unsigned char> file(sizeof(fileinfo) + sizeof(BITMAPINFOHEADER) + stride * m_lHeight);
	memcpy(&file[0], &fileinfo, sizeof(BITMAPFILEHEADER));
	memcpy(&file[sizeof(BITMAPFILEHEADER)], &bmpinfo, sizeof(BITMAPINFOHEADER));
	memcpy(&file[sizeof(BITMAPFILEHEADER)+ sizeof(BITMAPINFOHEADER)], bits, stride * m_lHeight);

	CFile ofile;
	CFileException fe;
	if (!ofile.Open(lpFileName,CFile::modeCreate | CFile::modeWrite | CFile::typeBinary , &fe))
	{
		return; // ошибка при открытии файла
	}

	ofile.Write(&file[0], file.size());
	ofile.Close();
}
