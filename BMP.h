#pragma once

#define BITMAPHEADER_SIZE 54
#include <cstdlib>
#include <iostream>
#include <cstdint>
#include <string>
#include <iostream>

namespace structures {

    #pragma pack(push, 1)
    typedef struct tagBITMAPFILEHEADER {
        uint16_t bfType;  //specifies the file type
        uint32_t bfSize;  //specifies the size in bytes of the bitmap file
        uint16_t bfReserved1;  //reserved; must be 0
        uint16_t bfReserved2;  //reserved; must be 0
        uint32_t bfOffBits;  //species the offset in bytes from the bitmapfileheader to the bitmap bits
    } BITMAPFILEHEADER ;

    typedef struct tagBITMAPINFOHEADER {
        uint32_t biSize;  //specifies the number of bytes required by the struct
        uint32_t biWidth;  //specifies width in pixels
        uint32_t biHeight;  //species height in pixels
        uint16_t biPlanes; //specifies the number of color planes, must be 1
        uint16_t biBitCount; //specifies the number of bit per pixel
        uint32_t biCompression;//specifies the type of compression
        uint32_t biSizeImage;  //size of image in bytes
        uint32_t biXPelsPerMeter;  //number of pixels per meter in x axis
        uint32_t biYPelsPerMeter;  //number of pixels per meter in y axis
        uint32_t biClrUsed;  //number of colors used by the bitmap
        uint32_t biClrImportant;  //number of colors that are important
    } BITMAPINFOHEADER;
    #pragma pack(pop)


    class BMP {

    private:

        BITMAPFILEHEADER bitmapfileheader;
        BITMAPINFOHEADER bitmapinfoheader;
        uint8_t *metadata;
        uint8_t *data;

    public:

        BMP(const std::string & path){

            std::ifstream file (path, std::ios::binary);


            if(!file.is_open())
                throw std::invalid_argument("No such file");

            file.read((char *)&bitmapfileheader, sizeof(BITMAPFILEHEADER));

            if (bitmapfileheader.bfType !=0x4D42) {
                file.close();
                throw std::invalid_argument("Illegal BMP format");
            }

            file.read((char *)&bitmapinfoheader, sizeof(BITMAPINFOHEADER));


            //TODO: See if this data is relevant
            metadata = new uint8_t[bitmapfileheader.bfOffBits-sizeof(BITMAPFILEHEADER)-sizeof(BITMAPINFOHEADER)];
            file.read((char *)metadata, bitmapfileheader.bfOffBits-sizeof(BITMAPFILEHEADER)-sizeof(BITMAPINFOHEADER));

            data = new uint8_t[bitmapinfoheader.biSizeImage];
            file.read((char *)(data), bitmapinfoheader.biSizeImage);

            file.close();


        }

        ~BMP() {
            delete metadata;
            delete data;
        }

        BITMAPFILEHEADER & fileHeader(){
            return bitmapfileheader;
        }

        BITMAPINFOHEADER & infoHeader(){
            return bitmapinfoheader;
        }


        /**
         * Writes a pixel.
         * Remember data is stored upside down, i.e.
         * the first row in the BMP is the last in
         * the image, and vice versa.
         * @param x
         * @param y
         * @param data
         */
        void write(uint32_t x, uint32_t y, uint64_t data){


        }


        void save(const std::string & path){

            std::ofstream file (path, std::ios::binary);

            file.write((char *)&bitmapfileheader, sizeof(BITMAPFILEHEADER));
            file.write((char *)&bitmapinfoheader, sizeof(BITMAPINFOHEADER));
            file.write((char *)metadata, bitmapfileheader.bfOffBits-sizeof(BITMAPFILEHEADER)-sizeof(BITMAPINFOHEADER));
            file.write((char *)data, bitmapinfoheader.biSizeImage);

            file.close();

        }





    };


}