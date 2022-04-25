//
// Created by floodd on 22/03/2022.
//

// Created by floodd on 11/02/2022.
//


#ifndef SDLTEST_TGA_FILEHANDLER_H
#define SDLTEST_TGA_FILEHANDLER_H
#include <iostream>
#include <fstream>
#include <iomanip>
#include "schrift.h"

using namespace std;
struct Rgb {
    Rgb() : r(1), g(1), b(1) {}

    Rgb(unsigned char rr) : r(rr), g(rr), b(rr) {}

    Rgb(unsigned char rr, unsigned char gg, unsigned char bb) : r(rr), g(gg), b(bb) {}

    unsigned char r, g, b;
    float kernel;
};
struct Font {
    SFT sft = {NULL, 12, 12, 0, 0, SFT_DOWNWARD_Y|SFT_RENDER_IMAGE};
    Font(const char* fontfile, uint16_t size) {
        if((sft.font = sft_loadfile(fontfile)) == NULL) {
            printf("\e[31m[ERROR] Failed to load %s\e[0m\n", fontfile);
            return;
        }
        setSize(size);
    }
    ~Font() {
        sft_freefont(sft.font);
    }
    void setSize(uint16_t size) {
        sft.xScale = size;
        sft.yScale = size;
    }
};

class Image {


    Image(const unsigned int _w, const unsigned int _h) :
            w(_w), h(_h), pixels(nullptr)
    {
        pixels = new Rgb[w * h];
    }

    unsigned int w, h;
    Rgb *pixels;
public:
    Image() : w(300), h(400)
    {
        pixels = new Rgb[w * h];
    }

    bool savePPM(string filename);
    bool load(string filename);
    bool loadRaw(string filename);
    void filterRed();
    void filterGreen();
    void filterBlue();
    void greyScale();
    void flipHorizontal();
    void flipVertically();
    void AdditionalFunction1(int x, int y, int newWidth, int newHeight);
    void AdditionalFunction2();
    void AdditionalFunction3();
    void AdvancedFeature(const char* txt, const Font& font, int x, int y, uint8_t r = 255, uint8_t g = 255, uint8_t b = 255, uint8_t a = 255);
    /*Functions used in the GUI - DO NOT MODIFY */
    Rgb* getImage();
    int getWidth();
    int getHeight();
    ~Image() { if (pixels != nullptr) delete [] pixels; }

};


#endif //SDLTEST_TGA_FILEHANDLER_H

