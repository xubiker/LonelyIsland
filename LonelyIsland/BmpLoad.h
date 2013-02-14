#ifndef __BMPLOAD_H

#define __BMPLOAD_H

#include <iostream>
#include <fstream>
using namespace std;

struct TGAHEADER {
  unsigned char  identsize;              // размер ID field that follows header (0)
    unsigned char  colorMapType;           // 0 = None, 1 = paletted
    unsigned char  imageType;              // 0 = none, 1 = индекс, 2 = rgb, 3 = серый, +8=rle  // RLE - кодирование длин серий(метод сжатия)
    unsigned short  colorMapStart;          // First colour map entry
    unsigned short  colorMapLength;         // Number of colors
    unsigned char   colorMapBits;      // bits per palette entry
    unsigned short  xstart;                 // image x origin
    unsigned short  ystart;                 // image y origin
    unsigned short  width;                  // ширина в пикселях
    unsigned short  height;                 // высота в пикселях
    unsigned char  bits;                   // количество бит на пиксель (8 16, 24, 32)
    unsigned char  descriptor;             // дескриптор рисунка
};// TGAHEADER

unsigned char *LoadIndexedBMPFile(const char *path,int *width,int *height);
unsigned char *LoadTrueColorBMPFile(const char *path,int *width,int *height);
unsigned char* bLoadTGA(const char *fileName, int *iWidth, int *iHeight);

#endif