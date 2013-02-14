#ifndef __BMPLOAD_H

#define __BMPLOAD_H

#include <iostream>
#include <fstream>
using namespace std;

struct TGAHEADER {
  unsigned char  identsize;              // ������ ID field that follows header (0)
    unsigned char  colorMapType;           // 0 = None, 1 = paletted
    unsigned char  imageType;              // 0 = none, 1 = ������, 2 = rgb, 3 = �����, +8=rle  // RLE - ����������� ���� �����(����� ������)
    unsigned short  colorMapStart;          // First colour map entry
    unsigned short  colorMapLength;         // Number of colors
    unsigned char   colorMapBits;      // bits per palette entry
    unsigned short  xstart;                 // image x origin
    unsigned short  ystart;                 // image y origin
    unsigned short  width;                  // ������ � ��������
    unsigned short  height;                 // ������ � ��������
    unsigned char  bits;                   // ���������� ��� �� ������� (8 16, 24, 32)
    unsigned char  descriptor;             // ���������� �������
};// TGAHEADER

unsigned char *LoadIndexedBMPFile(const char *path,int *width,int *height);
unsigned char *LoadTrueColorBMPFile(const char *path,int *width,int *height);
unsigned char* bLoadTGA(const char *fileName, int *iWidth, int *iHeight);

#endif