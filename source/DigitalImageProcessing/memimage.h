/*
 #
 #  File        : vrml.h
 #                ( C++ header file - CImg plug-in )
 #
 #  Description : CImg plugin that provide functions to load/save memory data.
 #                This file is a part of the CImg Library project.
 #                ( http://cimg.sourceforge.net )
 #
 #  Copyright   : Alexandr Barantsev
 #                ( appdeveloper (at) yandex.ru )
 #
 #  License     : CeCILL v2.0
 #                ( http://www.cecill.info/licences/Licence_CeCILL_V2-en.html )
 #
 #  This software is governed by the CeCILL  license under French law and
 #  abiding by the rules of distribution of free software.  You can  use,
 #  modify and/ or redistribute the software under the terms of the CeCILL
 #  license as circulated by CEA, CNRS and INRIA at the following URL
 #  "http://www.cecill.info".
 #
 #  As a counterpart to the access to the source code and  rights to copy,
 #  modify and redistribute granted by the license, users are provided only
 #  with a limited warranty  and the software's author,  the holder of the
 #  economic rights,  and the successive licensors  have only  limited
 #  liability.
 #
 #  In this respect, the user's attention is drawn to the risks associated
 #  with loading,  using,  modifying and/or developing or reproducing the
 #  software by the user in light of its specific status of free software,
 #  that may mean  that it is complicated to manipulate,  and  that  also
 #  therefore means  that it is reserved for developers  and  experienced
 #  professionals having in-depth computer knowledge. Users are therefore
 #  encouraged to load and test the software's suitability as regards their
 #  requirements in conditions enabling the security of their systems and/or
 #  data to be ensured and,  more generally, to use and operate it in the
 #  same conditions as regards security.
 #
 #  The fact that you are presently reading this means that you have had
 #  knowledge of the CeCILL license and that you accept its terms.
 #
*/

#ifndef cimg_plugin_memimage
#define cimg_plugin_memimage

CImg<T>& load_stream(std::vector<unsigned char> bmp)
{
	unsigned char* bitmap = &bmp[0];
	size_t filesize = bmp.size();
	if (!bitmap)
	{
		throw CImgArgumentException(_cimg_instance
                                    "load_stream(): Specified memory stream is (null).",
                                    cimg_instance);
	}
	unsigned char header[64] = { 0 };
	memcpy(header, bitmap, 54);
	      if (*header!='B' || header[1]!='M') {
        throw CImgIOException(_cimg_instance
                              "load_stream(): Invalid stream data file.",
                              cimg_instance);
      }

      // Read header and pixel buffer
      int
        file_size = header[0x02] + (header[0x03]<<8) + (header[0x04]<<16) + (header[0x05]<<24),
        offset = header[0x0A] + (header[0x0B]<<8) + (header[0x0C]<<16) + (header[0x0D]<<24),
        dx = header[0x12] + (header[0x13]<<8) + (header[0x14]<<16) + (header[0x15]<<24),
        dy = header[0x16] + (header[0x17]<<8) + (header[0x18]<<16) + (header[0x19]<<24),
        compression = header[0x1E] + (header[0x1F]<<8) + (header[0x20]<<16) + (header[0x21]<<24),
        nb_colors = header[0x2E] + (header[0x2F]<<8) + (header[0x30]<<16) + (header[0x31]<<24),
        bpp = header[0x1C] + (header[0x1D]<<8);

      if (!file_size || file_size==offset) {
		  //bitmap->seekg(0,std::ios_base::end);
        file_size = filesize;
        
      }
	  bitmap += 54;

      const int
        cimg_iobuffer = 12*1024*1024,
        //dx_bytes = (bpp==1)?(dx/8+(dx%8?1:0)):((bpp==4)?(dx/2+(dx%2?1:0)):(dx*bpp/8)),
		dx_bytes = ((((dx * bpp) + 31) & ~31) >> 3),
        align_bytes = (4-dx_bytes%4)%4,
        buf_size = cimg::min(cimg::abs(dy)*(dx_bytes + align_bytes),file_size - offset);
      CImg<intT> colormap; // intT
      if (bpp<16) { if (!nb_colors) nb_colors = 1<<bpp; } else nb_colors = 0;
      if (nb_colors) { colormap.assign(nb_colors); memcpy(colormap._data, bitmap,4*nb_colors); bitmap += 4*nb_colors;}
      const int xoffset = offset - 54 - 4*nb_colors;
	  if (xoffset>0) bitmap += xoffset;

	  CImg<ucharT> buffer;
      if (buf_size<cimg_iobuffer) { buffer.assign(buf_size); memcpy(buffer._data,bitmap, buf_size); bitmap += buf_size;}
      else buffer.assign(dx_bytes + align_bytes);
      unsigned char *ptrs = buffer;

	        // Decompress buffer (if necessary)
      if (compression) {
          throw CImgIOException(_cimg_instance
                                "load_bmp(): Unable to load compressed data from stream.",
                                cimg_instance);
      }
       // Read pixel data
      assign(dx,cimg::abs(dy),1,3);
      switch (bpp) {
      case 1 : { // Monochrome
        for (int y = height()-1; y>=0; --y) {
			if (buf_size>=cimg_iobuffer) { memcpy((ptrs=buffer._data),bitmap,dx_bytes); bitmap += dx_bytes; bitmap += align_bytes; }
          unsigned char mask = 0x80, val = 0;
          cimg_forX(*this,x) {
            if (mask==0x80) val = *(ptrs++);
            const unsigned char *col = (unsigned char*)(colormap._data + (val&mask?1:0));
            (*this)(x,y,2) = (T)*(col++);
            (*this)(x,y,1) = (T)*(col++);
            (*this)(x,y,0) = (T)*(col++);
            mask = cimg::ror(mask);
          }
          ptrs+=align_bytes;
        }
      } break;
      case 4 : { // 16 colors
        for (int y = height()-1; y>=0; --y) {
          if (buf_size>=cimg_iobuffer) { memcpy((ptrs=buffer._data),bitmap,dx_bytes); bitmap += dx_bytes; bitmap += align_bytes; }
          unsigned char mask = 0xF0, val = 0;
          cimg_forX(*this,x) {
            if (mask==0xF0) val = *(ptrs++);
            const unsigned char color = (unsigned char)((mask<16)?(val&mask):((val&mask)>>4));
            const unsigned char *col = (unsigned char*)(colormap._data + color);
            (*this)(x,y,2) = (T)*(col++);
            (*this)(x,y,1) = (T)*(col++);
            (*this)(x,y,0) = (T)*(col++);
            mask = cimg::ror(mask,4);
          }
          ptrs+=align_bytes;
        }
      } break;
      case 8 : { //  256 colors
        for (int y = height()-1; y>=0; --y) {
			if (buf_size>=cimg_iobuffer) { memcpy((ptrs=buffer._data),bitmap,dx_bytes); bitmap += dx_bytes; bitmap += align_bytes; }
          cimg_forX(*this,x) {
            const unsigned char *col = (unsigned char*)(colormap._data + *(ptrs++));
            (*this)(x,y,2) = (T)*(col++);
            (*this)(x,y,1) = (T)*(col++);
            (*this)(x,y,0) = (T)*(col++);
          }
          ptrs+=align_bytes;
        }
      } break;
      case 16 : { // 16 bits colors
        for (int y = height()-1; y>=0; --y) {
          if (buf_size>=cimg_iobuffer) { memcpy((ptrs=buffer._data),bitmap,dx_bytes); bitmap += dx_bytes; bitmap += align_bytes;}
          cimg_forX(*this,x) {
            const unsigned char c1 = *(ptrs++), c2 = *(ptrs++);
            const unsigned short col = (unsigned short)(c1|(c2<<8));
            (*this)(x,y,2) = (T)(col&0x1F);
            (*this)(x,y,1) = (T)((col>>5)&0x1F);
            (*this)(x,y,0) = (T)((col>>10)&0x1F);
          }
          ptrs+=align_bytes;
        }
      } break;
      case 24 : { // 24 bits colors
        for (int y = height()-1; y>=0; --y) {
          if (buf_size>=cimg_iobuffer) { memcpy((ptrs=buffer._data),bitmap,dx_bytes); bitmap += dx_bytes; bitmap += align_bytes; }
          cimg_forX(*this,x) {
            (*this)(x,y,2) = (T)*(ptrs++);
            (*this)(x,y,1) = (T)*(ptrs++);
            (*this)(x,y,0) = (T)*(ptrs++);
          }
          ptrs+=align_bytes;
        }
      } break;
      case 32 : { // 32 bits colors
        for (int y = height()-1; y>=0; --y) {
          if (buf_size>=cimg_iobuffer) { memcpy((ptrs=buffer._data),bitmap,dx_bytes); bitmap += dx_bytes; bitmap += align_bytes;}
          cimg_forX(*this,x) {
            (*this)(x,y,2) = (T)*(ptrs++);
            (*this)(x,y,1) = (T)*(ptrs++);
            (*this)(x,y,0) = (T)*(ptrs++);
            ++ptrs;
          }
          ptrs+=align_bytes;
        }
      } break;
      }
      if (dy<0) mirror('y');
      return *this;
}

const CImg<T>& save_stream(std::vector<unsigned char>& bmp)
{
      if (is_empty()) {
		  bmp.resize(0);
		  return *this; }
      if (_depth>1)
        cimg::warn(_cimg_instance
                   "save_stream(): Instance is volumetric, only the first slice will be saved in stream.",
                   cimg_instance);
      if (_spectrum>3)
        cimg::warn(_cimg_instance
                   "save_stream(): Instance is multispectral, only the three first channels will be saved in stream.",
                   cimg_instance);
	  bmp.resize(54);
	  unsigned char *bitmap = &bmp[0];
      unsigned char header[54] = { 0 }, align_buf[4] = { 0 };
      const unsigned int
        align = (4 - (3*_width)%4)%4,
        buf_size = (3*_width + align)*height(),
        file_size = 54 + buf_size;
	    bmp.resize(file_size);
		bitmap = &bmp[0];
      header[0] = 'B'; header[1] = 'M';
      header[0x02] = file_size&0xFF;
      header[0x03] = (file_size>>8)&0xFF;
      header[0x04] = (file_size>>16)&0xFF;
      header[0x05] = (file_size>>24)&0xFF;
      header[0x0A] = 0x36;
      header[0x0E] = 0x28;
      header[0x12] = _width&0xFF;
      header[0x13] = (_width>>8)&0xFF;
      header[0x14] = (_width>>16)&0xFF;
      header[0x15] = (_width>>24)&0xFF;
      header[0x16] = _height&0xFF;
      header[0x17] = (_height>>8)&0xFF;
      header[0x18] = (_height>>16)&0xFF;
      header[0x19] = (_height>>24)&0xFF;
      header[0x1A] = 1;
      header[0x1B] = 0;
      header[0x1C] = 24;
      header[0x1D] = 0;
      header[0x22] = buf_size&0xFF;
      header[0x23] = (buf_size>>8)&0xFF;
      header[0x24] = (buf_size>>16)&0xFF;
      header[0x25] = (buf_size>>24)&0xFF;
      header[0x27] = 0x1;
      header[0x2B] = 0x1;
	 
      memcpy(bitmap,(char*)header,54);
	  bitmap += 54;

      const T
        *ptr_r = data(0,_height-1,0,0),
        *ptr_g = (_spectrum>=2)?data(0,_height-1,0,1):0,
        *ptr_b = (_spectrum>=3)?data(0,_height-1,0,2):0;

      switch (_spectrum) {
      case 1 : {
        cimg_forY(*this,y) {
          cimg_forX(*this,x) {
            const unsigned char val = (unsigned char)*(ptr_r++);
            *bitmap = val; bitmap++;
			*bitmap = val; bitmap++;
			*bitmap = val; bitmap++;
          }
          memcpy(bitmap,(char*)align_buf,align);
		  bitmap += align;
          ptr_r-=2*_width;
        }
      } break;
      case 2 : {
        cimg_forY(*this,y) {
          cimg_forX(*this,x) {
            *bitmap = (unsigned char)0; bitmap++;
			*bitmap = (*(ptr_g++)); bitmap++;
			*bitmap = (*(ptr_r++)); bitmap++;
          }
          memcpy(bitmap,align_buf,align);
		  bitmap += align;
          ptr_r-=2*_width; ptr_g-=2*_width;
        }
      } break;
      default : {
        cimg_forY(*this,y) {
          cimg_forX(*this,x) {
			 *bitmap = (*(ptr_b++)); bitmap++;
            *bitmap = (*(ptr_g++)); bitmap++;
            *bitmap = (*(ptr_r++)); bitmap++;
          }
          memcpy(bitmap,align_buf,align);
		  bitmap += align;
          ptr_r-=2*_width; ptr_g-=2*_width; ptr_b-=2*_width;
        }
      }
      }

      return *this;
}

#endif