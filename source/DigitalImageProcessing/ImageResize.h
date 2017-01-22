#pragma once
// ����� ����������� ��������� ������� �����������
#include <vector>
#define cimg_display 0
#define cimg_plugin "memimage.h"
#include "CImg.h"

using namespace std;
class CImageResize
{
public:
	CImageResize(void);
	~CImageResize(void);
	bool ScaleImage(vector<unsigned char>& bmpdata);
	// ����� ������ �����������
	CSize m_newSize;
	// ��������� �����������
	cil::CImg<unsigned char> m_image;
	// ������� ������, � ������� �������� ����� �����������
	std::vector<unsigned char> m_bmpfile_memory;
	// ���������������� �����������
	std::vector<unsigned char> m_scaledImage;
	// �������� �����������
	std::vector<unsigned char> m_srcImage;
	void DrawBitmapData(unsigned char* bitmap , CDC* pDC);
};

