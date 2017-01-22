#include "StdAfx.h"
#include "ImageResize.h"


CImageResize::CImageResize(void)
{
}


CImageResize::~CImageResize(void)
{
}


bool CImageResize::ScaleImage(vector<unsigned char>& bmpdata)
{
	m_scaledImage.clear();
	m_bmpfile_memory.clear();
	if (bmpdata.size() == 0) return false; // если нет данных для загрузки - завершаем работу метода
	if ((m_newSize.cx == 0) || (m_newSize.cy==0)) return false;
	cil::CImg<unsigned char> m_scaledImage;
	m_image.load_stream(bmpdata);
	m_scaledImage = m_image;
	m_scaledImage.resize((int)(m_newSize.cx), (int)(m_newSize.cy), -100, -100, 3, 0,0,0,0,0);
	//int width = min( m_visible_region.Width(), m_scaledImage.width());
	//int height = min(m_visible_region.Height(), m_scaledImage.height());
	//m_scaledImage.crop(0, 0, 0,  m_visible_region.Width(),  m_visible_region.Height(), m_scaledImage.depth()-1,0);
	m_scaledImage.save_stream(m_bmpfile_memory);
	return true;
}


void CImageResize::DrawBitmapData(unsigned char* bitmap , CDC* pDC)
{
	BITMAPFILEHEADER* bmfh;
	bmfh = (BITMAPFILEHEADER*)bitmap;

	BITMAP bm;
	BITMAPINFOHEADER* bmih;
	bmih = (BITMAPINFOHEADER*)(bitmap + sizeof(BITMAPFILEHEADER));
	BITMAPINFO* bmi;
	bmi = (BITMAPINFO*)bmih;

	void* bits;
	bits = (void*)(bitmap + bmfh->bfOffBits);

	HDC hdc = ::GetDC(NULL);

	HBITMAP hbmp = CreateDIBitmap(hdc, bmih, CBM_INIT, bits, bmi, DIB_RGB_COLORS) ;

	::ReleaseDC(NULL, hdc);

	GetObject(hbmp, sizeof(bm), &bm);
 
	CDC hdcMem;
	hdcMem.CreateCompatibleDC(pDC);
	HGDIOBJ hOldBMP = hdcMem.SelectObject(hbmp);
	pDC->BitBlt(0, 0, bm.bmWidth, bm.bmHeight, &hdcMem, 0, 0, SRCCOPY);
	hdcMem.SelectObject(hOldBMP);
	DeleteDC(hdcMem);
	::DeleteObject(hbmp);
}
