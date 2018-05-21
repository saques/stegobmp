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

        BITMAPFILEHEADER fileh;
        BITMAPINFOHEADER infoh;
        uint8_t *metadata;
        uint8_t *data;

    public:

        BMP(const std::string &path) {

            std::ifstream file(path, std::ios::binary);


            if (!file.is_open())
                throw std::invalid_argument("No such file");

            file.read((char *) &fileh, sizeof(BITMAPFILEHEADER));

            if (fileh.bfType != 0x4D42) {
                file.close();
                throw std::invalid_argument("Illegal BMP format");
            }

            file.read((char *) &infoh, sizeof(BITMAPINFOHEADER));


            //TODO: See if this data is relevant
            metadata = new uint8_t[fileh.bfOffBits - sizeof(BITMAPFILEHEADER) - sizeof(BITMAPINFOHEADER)];
            file.read((char *) metadata, fileh.bfOffBits - sizeof(BITMAPFILEHEADER) - sizeof(BITMAPINFOHEADER));


            if(infoh.biSizeImage == 0){
                infoh.biSizeImage = (infoh.biBitCount/8)*infoh.biWidth*infoh.biHeight;
            }

            data = new uint8_t[infoh.biSizeImage];
            file.read((char *) (data), infoh.biSizeImage);

            file.close();


        }

        ~BMP() {
            delete metadata;
            delete data;
        }

        BITMAPFILEHEADER &fileHeader() {
            return fileh;
        }

        BITMAPINFOHEADER &infoHeader() {
            return infoh;
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
        void write(uint32_t x, uint32_t y, uint64_t d) {

            if (outOfBounds(x, y)) {
                throw std::out_of_range("BMP index out of range");
            }
            uint32_t idx = index(x, y);

            for (int i = infoh.biBitCount/8 - 1; i >= 0 ; i--, d = d >> (uint8_t)8) {
                data[idx + i] = (uint8_t) (((uint64_t)0x00FF) & d);
            }

        }

        /**
         * Reads a pixel from the BMP and stores it
         * in a 64 bit unsigned integer.
         * @param x
         * @param y
         * @return
         */
        uint64_t read(uint32_t x, uint32_t y){
            if (outOfBounds(x, y)) {
                throw std::out_of_range("BMP index out of range");
            }
            uint64_t ans = 0;
            uint32_t idx = index(x, y);
            for (int i = 0; i < infoh.biBitCount/8; i++) {
                ans |= data[idx+i];
                if(i < infoh.biBitCount/8 - 1)
                    ans = ans << (uint8_t)8;
            }
            return ans;
        }


        void save(const std::string & path){

            std::ofstream file (path, std::ios::binary);

            file.write((char *)&fileh, sizeof(BITMAPFILEHEADER));
            file.write((char *)&infoh, sizeof(BITMAPINFOHEADER));
            file.write((char *)metadata, fileh.bfOffBits-sizeof(BITMAPFILEHEADER)-sizeof(BITMAPINFOHEADER));
            file.write((char *)data, infoh.biSizeImage);

            file.close();

        }

    private:

        bool outOfBounds(uint32_t x, uint32_t y){
            return x < 0 || y < 0 || x > infoh.biWidth || y > infoh.biHeight;
        }

        uint32_t index(uint32_t x, uint32_t y){
            return infoh.biSizeImage - (infoh.biWidth*(1+y)-x-1)*infoh.biBitCount/8-infoh.biBitCount/8;
        }





    };


}