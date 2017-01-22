#pragma once
#include <vector>
#include "DataProcess.h"
using namespace std;

class CImageData
{
public:
	CImageData(void);
	~CImageData(void);
	// ��������� bitmap ���� � �����
	bool ReadDIBFile(LPCTSTR lpszFileName);
protected:
	WORD DIBNumColors(LPBITMAPINFOHEADER lpBmpInfo);
public:
	// ������, ���������� ������������
	CRect m_region;
	// ������ �����������
	LONG m_lHeight;
	// ������ �����������
	LONG m_lWidth;
	// ����� �������� ������� �����������
	unsigned char* m_colorData[3];
	// ���������� �������, ���������� �������
	void Clear(void);
	void CreateBitmap(void);
	// ������ � �������� �����������
	BITMAPINFO m_bitmapinfo;
	void* m_pBits;
	// �������� �������� ����������� �� ��������� �������
	CImageData* GetImageFromRegion(CRect region);
	// ��������� �������� ����������� �� image � �����������, ���������� � ������ ������
	void CombineImage(CImageData* image);
	// ���������� ����������� ��������, ������������ ������� func
	void Process(CDataProcess* func);
	// ������� ����������� � ��������
	bool CreateSizedBitmap(CSize size, CDC *pDC);
	void CreateDIBFile(LPCTSTR lpFileName);
};

