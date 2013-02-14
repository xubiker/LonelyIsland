#ifndef __FORMATS_H
#include"formats.h"
#endif

#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include "BmpLoad.h"

unsigned char *LoadIndexedBMPFile(const char *path,int *width,int *height)
{
	unsigned char *result = NULL;
    FILE *in = fopen(path,"rb");
	if(!in)
		return NULL;
    BMPHeader hdr;
    fread(&hdr,sizeof(hdr),1,in);
    if(hdr.type != 0x4D42)           // Not a bitmap file at all
		return NULL;
	BMPInfoHeader	infoHdr;
	fread(&infoHdr,sizeof(infoHdr),1,in);
	if(infoHdr.bitsPerPixel != 8)
		return NULL;
	if(infoHdr.compression)
		return NULL;
	if((result = new unsigned char[infoHdr.width*infoHdr.height*3]) == NULL)
		return NULL;
	BMPPaletteItem palette[256];
	fread(palette,256,sizeof(BMPPaletteItem),in);
    fseek(in,hdr.offset-sizeof(hdr)-sizeof(infoHdr)-1024,SEEK_CUR);   
	unsigned char *dst = result;
	for(int y=0;y<infoHdr.height;y++) {
		for(int x=0;x<infoHdr.width;x++) {
			int idx = (unsigned char)fgetc(in);
			*dst++ = palette[idx].red;
			*dst++ = palette[idx].green;
			*dst++ = palette[idx].blue;
		}
        for(int x=0;x<((4-(infoHdr.width&3))&3);x++)  // Skip alignment bytes
           fgetc(in);
    }
    fclose(in);
	*width = infoHdr.width;
	*height = infoHdr.height;
	return result;
}

unsigned char *LoadTrueColorBMPFile(const char *path,int *width,int *height)
{
	unsigned char *result = NULL;
    FILE *in = fopen(path,"rb");
	if(!in)
		return NULL;
    BMPHeader hdr;
    fread(&hdr,sizeof(hdr),1,in);
    if(hdr.type != 0x4D42)           // Not a bitmap file at all
		return NULL;
	BMPInfoHeader	infoHdr;
	fread(&infoHdr,sizeof(infoHdr),1,in);
	if(infoHdr.bitsPerPixel != 24)   // Not a truecolor bitmap
		return NULL;
	if(infoHdr.compression)          // Compressed bitmap
		return NULL;
	if((result = new unsigned char[infoHdr.width*infoHdr.height*3]) == NULL)
		return NULL;
    fseek(in,hdr.offset-sizeof(hdr)-sizeof(infoHdr),SEEK_CUR);   
	unsigned char *dst = result;
	for(int y=0;y<infoHdr.height;y++) {
		for(int x=0;x<infoHdr.width;x++) {
			dst[2] = fgetc(in);
			dst[1] = fgetc(in);
			dst[0] = fgetc(in);
			dst += 3;
		}
        for(int x=0;x<((4-(infoHdr.width&3))&3);x++)  // Skip alignment bytes
           fgetc(in);
    }
    fclose(in);
	*width = infoHdr.width;
	*height = infoHdr.height;
	return result;
}

unsigned char* bLoadTGA(const char *fileName, int *iWidth, int *iHeight) 
{
	ifstream  iFile;
    TGAHEADER tgaHeader;    // TGA file header
    unsigned long lImageSize;  // размер текстуры в байтах
    unsigned char *pBits = NULL;      // ссылка на биты данных
 
  // Открытие файла
	iFile.open(fileName,ios::in | ios::binary);
  if(!iFile.is_open()){
    cout<<"Невозможно открыть файл";
        return NULL;
  }
  // Запись структуры TGA (binary)
  // &tgaHeader - указатель на область данных в которую записываются данные, считанные из файла
  // 18/* sizeof(TGAHEADER)*/ - количество байтов
  // 1 - количество блоков, считываемых при чтении
  // iFile - указатель на открытый файл
    //fread(&tgaHeader, 18/* sizeof(TGAHEADER)*/, 1, iFile);
  iFile.read((char *) &tgaHeader,sizeof(TGAHEADER));
 
  // Получаем ширину и высоту текстуры
    *iWidth = tgaHeader.width;
    *iHeight = tgaHeader.height;
 cerr << sizeof(unsigned short) << endl;
  lImageSize = tgaHeader.width * tgaHeader.height * tgaHeader.bits / 8; // ширина * высота * кол. байтов
 
  // Динамическое выделение памяти
    pBits = new unsigned char [lImageSize]; // возвр. указатель на первый бит выделенной памяти
    if(pBits == NULL)
        return NULL;
    
    // Запись битов текстуры из файла в выделеную память
  iFile.read((char *) pBits, lImageSize);
    if( pBits == NULL){
        delete [] pBits;  // освобождаем выделенную память
        return NULL;    // ошибка
        }
 
  // закрытие файла
    iFile.close();
        
    // возвращаем биты данных
    return pBits;
}