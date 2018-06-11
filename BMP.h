#pragma once

#include <cstdlib>
#include <iostream>
#include <cstdint>
#include <string>
#include <iostream>

namespace Structures {

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

    #define HEADERS_SIZE (sizeof(BITMAPFILEHEADER)+sizeof(BITMAPINFOHEADER))


    class BMP {

    private:

        BITMAPFILEHEADER fileh;
        BITMAPINFOHEADER infoh;

        std::unique_ptr<uint8_t *> metadata;
        std::unique_ptr<uint8_t *> data;

        uint8_t padding;
        uint64_t absoluteSize;

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
            uint64_t metadataSize = fileh.bfOffBits - HEADERS_SIZE;
            metadata = std::make_unique<uint8_t *>(new uint8_t[metadataSize]);
            file.read((char *) *metadata, metadataSize);

            if(infoh.biSizeImage == 0){
                infoh.biSizeImage = (infoh.biBitCount/8)*infoh.biWidth*infoh.biHeight;
            }

            padding = (uint8_t)(infoh.biWidth*(infoh.biBitCount/8)%4);
            absoluteSize = infoh.biSizeImage + padding*infoh.biHeight;

            data = std::make_unique<uint8_t *>(new uint8_t[absoluteSize]);
            file.read((char *) *data, absoluteSize);
            file.close();

        }

        BITMAPFILEHEADER &FileHeader() {
            return fileh;
        }

        BITMAPINFOHEADER &InfoHeader() {
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
        void Write(uint32_t x, uint32_t y, uint64_t d) {

            if (OutOfBounds(x, y))
                throw std::out_of_range("BMP index out of range");

            uint32_t idx = Index(x, y);

            for (int i = infoh.biBitCount/8 - 1; i >= 0 ; i--, d = d >> (uint8_t)8) {
                (*data)[idx + i] = (uint8_t) (((uint64_t)0x00FF) & d);
            }

        }

        /**
         * Writes data to the underlying array of bytes,
         * without skipping padding.
         * @param d
         * @param d
         */
        void Write(uint32_t p, uint8_t d){
            if (OutOfBounds(p))
                throw std::out_of_range("BMP underlying index our of range");
            (*data)[p] = d;
        }

        /**
         * Embeds in the BMP data, according to a specified
         * insertion mode.
         * @param data
         * @param mode
         */
        void Write(std::vector<uint8_t> &data, Config::StegoInsertion mode){
            std::vector<uint8_t>::iterator it = data.begin();
            uint32_t idx = 0;
            switch (mode){
                case Config::StegoInsertion::LSB1:
                    if(LSB1OutOfSize(data.size()))
                        throw std::out_of_range("Origin data bigger than available size");

                    for( ; it != data.end(); it++ , idx += 8){
                        uint8_t b = *it;
                        for(uint8_t delta = 0; delta < 8; delta++) {
                            (*this->data)[idx + delta] ^= ((uint8_t)0x01&(b>>(7-delta)));
                        }
                    };
                    break;

                case Config::StegoInsertion::LSB4:
                    if(LSB4OutOfSize(data.size()))
                        throw std::out_of_range("Origin data bigger than available size");

                    for( ; it != data.end(); it++ , idx += 2){
                        uint8_t b = *it;
                        for(uint8_t delta = 0; delta < 2; delta++) {
                            (*this->data)[idx + delta] ^= ((uint8_t)0x0F&(b>>((1-delta)*4)));
                        }
                    };
                    break;

                case Config::StegoInsertion::LSBE:
                    //TODO: Read paper
                    break;
                default:
                    throw std::invalid_argument("Illegal insertion mode");
            }
        }

        static std::vector<uint8_t> Diff(BMP& original, BMP& altered, Config::StegoInsertion mode){

            if(original.absoluteSize != altered.absoluteSize)
                throw std::invalid_argument("Images of different size");

            std::vector<uint8_t> ans;
            bool finish = false;

            switch (mode){
                case Config::StegoInsertion::LSB1:

                    for(uint32_t p = 0; !finish && p < original.absoluteSize; p+=8){
                        uint8_t b = 0;
                        finish = true;
                        for(uint8_t delta = 0; delta < 8; delta++){
                            uint8_t diff;
                            if((diff = (*original.data)[p+delta] ^ (*altered.data)[p+delta]) != 0){
                                finish = false;
                                b |= (uint8_t)(diff&(uint8_t)0x01)<<(uint8_t)(7-delta);
                            }
                        }
                        if(!finish)
                            ans.push_back(b);
                    }

                    break;

                case Config::StegoInsertion::LSB4:

                    for(uint32_t p = 0; !finish && p < original.absoluteSize; p+=2){
                        uint8_t b = 0;
                        finish = true;
                        for(uint8_t delta = 0; delta < 2; delta++){
                            uint8_t diff;
                            if((diff = (*original.data)[p+delta] ^ (*altered.data)[p+delta]) != 0){
                                finish = false;
                                b |= (uint8_t)(diff&(uint8_t)0x0F)<<(uint8_t)((1-delta)*4);
                            }
                        }
                        if(!finish)
                            ans.push_back(b);
                    }

                    break;

                case Config::StegoInsertion::LSBE:
                    //TODO: Read paper
                    break;
                default:
                    throw std::invalid_argument("Illegal insertion mode");
            }
            return ans;
        }

        /**
         * Reads a pixel from the BMP and stores it
         * in a 64 bit unsigned integer.
         * @param x
         * @param y
         * @return
         */
        uint64_t Read(uint32_t x, uint32_t y){
            if (OutOfBounds(x, y))
                throw std::out_of_range("BMP index out of range");

            uint64_t ans = 0;
            uint32_t idx = Index(x, y);
            for (int i = 0; i < infoh.biBitCount/8; i++) {
                ans |= (*data)[idx+i];
                if(i < infoh.biBitCount/8 - 1)
                    ans = ans << (uint8_t)8;
            }
            return ans;
        }


        /**
         * Reads a pixel from the underlying array of bytes.
         * @param p
         * @return
         */
        uint8_t Read(uint32_t p){
            if (OutOfBounds(p))
                throw std::out_of_range("BMP underlying index our of range");
            return (*data)[p];
        }


        uint64_t AbsoluteSize(){
            return absoluteSize;
        }

        void Save(const std::string &path){
            std::ofstream file (path, std::ios::binary);
            file.write((char *)&fileh, sizeof(BITMAPFILEHEADER));
            file.write((char *)&infoh, sizeof(BITMAPINFOHEADER));
            file.write((char *) *metadata, fileh.bfOffBits-sizeof(BITMAPFILEHEADER)-sizeof(BITMAPINFOHEADER));
            file.write((char *) *data, infoh.biSizeImage);
            file.close();
        }

    private:

        bool LSB1OutOfSize(uint64_t bytes){
            return bytes*8 > absoluteSize;
        }

        bool LSB4OutOfSize(uint64_t bytes){
            return bytes*2 > absoluteSize;
        }


        bool OutOfBounds(uint32_t x, uint32_t y){
            return x < 0 || y < 0 || x >= infoh.biWidth || y >= infoh.biHeight;
        }

        bool OutOfBounds(uint32_t p){
            return p < 0 || p >= absoluteSize;
        }

        uint32_t Index(uint32_t x, uint32_t y){
            x += y*padding;
            return infoh.biSizeImage - (infoh.biWidth*(1+y)-x-1)*infoh.biBitCount/8-infoh.biBitCount/8;
        }





    };


}