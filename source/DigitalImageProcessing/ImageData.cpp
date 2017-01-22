#include "StdAfx.h"
#include "ImageData.h"

#include "ImageResize.h"

/*
 * ������ ������ bitmap ����� - ������������ ��� ������ bitmap  � ����
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


// ��������� bitmap ���� � �����
bool CImageData::ReadDIBFile(LPCTSTR lpszFileName)
{
	int row, col, i;
	int idx; // ������ ����� � ������� m_colorData[k]
	CFile file;
	CFileException fe;
	Clear(); // ����� ������� ������ ������ ������� ����� ������������ �����������
	if (!file.Open(lpszFileName, CFile::modeRead | CFile::shareDenyWrite, &fe))
	{
		return false; // ������ ��� �������� �����
	}

	BITMAPFILEHEADER bmfHeader;
	UINT nBitsSize;
	
	unsigned char* pDIB; // ��������� �� bitmap ����������� � ������

	/*
	 * get length of DIB in bytes for use when reading
	 */

	nBitsSize = (UINT)file.GetLength();

	/*
	 * ��������� ��������� bitmap ����� � ��������� ��� ������������.
	 */
	if (file.Read((LPSTR)&bmfHeader, sizeof(bmfHeader)) != sizeof(bmfHeader))
		return false;

	if (bmfHeader.bfType != DIB_HEADER_MARKER)
		return false;

	/*
	 * �������� ������ ��� bitmap
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

	// ��������� bmp �����
	LPBITMAPINFOHEADER lpBmpInfo = (LPBITMAPINFOHEADER)pDIB;
	
	// ���������� ������ ������� ������
	WORD PaletteSize = DIBNumColors(lpBmpInfo) * sizeof(RGBQUAD);

	RGBQUAD* paletteData = NULL; // ������ �������

	if (PaletteSize != 0)
	{
		paletteData = &((LPBITMAPINFO)lpBmpInfo)->bmiColors[0];
	}

	// �������� ������ ��� �����������
	m_lHeight = abs(lpBmpInfo->biHeight);
	m_lWidth = lpBmpInfo->biWidth;
	for(i = 0; i < 3; i++)
	{
		m_colorData[i] = new unsigned char[m_lHeight * m_lWidth];
	}

	// ��������� ���������� ������ ���������� �����������
	long stride = stride = ((((lpBmpInfo->biWidth * lpBmpInfo->biBitCount) + 31) & ~31) >> 3); // ������ ������ ����������� � �������� �����. ����� �� �������� � MSDN
	unsigned char* pBitmap = (unsigned char*)lpBmpInfo + *(LPDWORD)lpBmpInfo + PaletteSize;     // ������ �������� ������ � �����������  
	
	for(row = 0; row < m_lHeight; row++)
	{
		unsigned char* row_data; // ������ ������
		if (lpBmpInfo->biHeight < 0)
		{
			// ���� biHeight ������������, bitmap �������� "������ ����"
			row_data = pBitmap + row*stride; // ��������� ����� ������ row

		} else
		{
			// ���� biHeight ������������, bitmap �������� "����� �����"
			row_data = pBitmap + (m_lHeight-row-1)*stride; // ��������� ����� ������ row
		}
		// ��������� ������ �� ������
		for(col = 0; col < m_lWidth; col++)
		{
			// ����������� ������ ������� �� ���������� ��� �� ����� � bitmap �����
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
	m_region = CRect(0,0,m_lWidth-1, m_lHeight-1); // ��� ������������ ����������� �������, ��� ��� �������� ��� �������
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

	/* ���������� ���������� ������ � ����������� �� ����� �� ����� */
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


// ���������� �������, ���������� �������
void CImageData::Clear(void)
{
	m_lHeight = 0;
	m_lWidth = 0;
	m_region = CRect(0,0,0,0);
	// ���������� ������� ����� �����������, ���� ����
	if(NULL != m_pBits)
	{
		delete[] m_pBits;
		m_pBits= NULL;
	}
	// ����������� ������, ������� ������� � ����� �����������
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
	// ��������� ������ �������
	for(row = 0; row < m_lHeight; row++)
	{
		row_data = bits + (m_lHeight-row-1)*stride; // ��������� ����� ������ row
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


// �������� �������� ����������� �� ��������� �������
CImageData* CImageData::GetImageFromRegion(CRect region)
{
	int row, col;
	CImageData* data = new CImageData;
	// ���������� �������, �������������� ����� region � m_region
	region.right = min(region.right, m_region.right);
	region.left = max(region.left, m_region.left);
	region.top = max(region.top, m_region.top);
	region.bottom = min(region.bottom, m_region.bottom);

	// �������� ������ � ��������� ��������� ������ ��� ��������� data
	data->m_lHeight = region.Height()+1;
	data->m_lWidth = region.Width()+1;
	data->m_colorData[0] = new unsigned char[data->m_lHeight * data->m_lWidth];
	data->m_colorData[1] = new unsigned char[data->m_lHeight * data->m_lWidth];
	data->m_colorData[2] = new unsigned char[data->m_lHeight * data->m_lWidth];
	data->m_pBits = NULL;
	memset(&data->m_bitmapinfo, 0,sizeof(data->m_bitmapinfo));
	data->m_region = region; // � �������������� ��������� ������ ��������� ���������� � �������, �� �������� ������� �������� �����������
	// ��������� ������ ��� �������
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


// ��������� �������� ����������� �� image � �����������, ���������� � ������ ������
void CImageData::CombineImage(CImageData* image)
{
	int row, col;
	int idx_source, idx_destination;
	if ((m_lWidth == 0) || (m_lHeight ==0)) return;
	CRect region = image->m_region;
	for(row = 0; row < image->m_lHeight; row++)
		for(col = 0; col < image->m_lWidth; col++)
		{
			// ������������� ������ �������� �� ���������� ������� 
			if ((row + region.top) >= m_lHeight) continue;
			if ((col + region.left) >= m_lWidth) continue;
			idx_source = row * image->m_lWidth + col;
			idx_destination = (row + region.top) * m_lWidth + (col + region.left);
			m_colorData[0][idx_destination] = image->m_colorData[0][idx_source];
			m_colorData[1][idx_destination] = image->m_colorData[1][idx_source];
			m_colorData[2][idx_destination] = image->m_colorData[2][idx_source];
		}
}


// ���������� ����������� ��������, ������������ ������� func
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


// ������� ����������� � �������� � ���������� ��� 
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
	// ��������� ������ �������
	for(row = 0; row < m_lHeight; row++)
	{
		row_data = bits + (m_lHeight-row-1)*stride; // ��������� ����� ������ row
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

	// ������� ����, ��� ������ ������
	vector<unsigned char> file(sizeof(fileinfo) + sizeof(BITMAPINFOHEADER) + stride * m_lHeight);
	memcpy(&file[0], &fileinfo, sizeof(BITMAPFILEHEADER));
	memcpy(&file[sizeof(BITMAPFILEHEADER)], &bmpinfo, sizeof(BITMAPINFOHEADER));
	memcpy(&file[sizeof(BITMAPFILEHEADER)+ sizeof(BITMAPINFOHEADER)], bits, stride * m_lHeight);
	// �������� ������ ����������� ���������� ��������
	
	CImageResize imgResize;
	imgResize.m_newSize = size;
	bResult = imgResize.ScaleImage(file);
	if (bResult)
		imgResize.DrawBitmapData(&imgResize.m_bmpfile_memory[0],pDC); // ��������� ����������� ��������� ������ � ������ ��������� ��������� ������� �����������
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
	// ��������� ������ �������
	for(row = 0; row < m_lHeight; row++)
	{
		row_data = bits + (m_lHeight-row-1)*stride; // ��������� ����� ������ row
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

	// ������� ����, ��� ������ ������
	vector<unsigned char> file(sizeof(fileinfo) + sizeof(BITMAPINFOHEADER) + stride * m_lHeight);
	memcpy(&file[0], &fileinfo, sizeof(BITMAPFILEHEADER));
	memcpy(&file[sizeof(BITMAPFILEHEADER)], &bmpinfo, sizeof(BITMAPINFOHEADER));
	memcpy(&file[sizeof(BITMAPFILEHEADER)+ sizeof(BITMAPINFOHEADER)], bits, stride * m_lHeight);

	CFile ofile;
	CFileException fe;
	if (!ofile.Open(lpFileName,CFile::modeCreate | CFile::modeWrite | CFile::typeBinary , &fe))
	{
		return; // ������ ��� �������� �����
	}

	ofile.Write(&file[0], file.size());
	ofile.Close();
}
