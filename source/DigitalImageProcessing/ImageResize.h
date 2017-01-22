#pragma once
// класс улучшенного изменения размера изображения
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
	// новый размер изображения
	CSize m_newSize;
	// хранилище изображения
	cil::CImg<unsigned char> m_image;
	// Область памяти, в которой хранится образ изображения
	std::vector<unsigned char> m_bmpfile_memory;
	// масштабированное изображение
	std::vector<unsigned char> m_scaledImage;
	// исходное изображение
	std::vector<unsigned char> m_srcImage;
	void DrawBitmapData(unsigned char* bitmap , CDC* pDC);
};

