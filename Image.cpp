//
// Created by floodd on 23/03/2022.
//
#include <iostream>
#include <fstream>
#include <iomanip>
#include <cstring>
#include "Image.h"

using namespace std;



bool Image::load(string filename)
{
    std::ifstream ifs;
    ifs.open(filename, std::ios::binary); // need to spec. binary mode for Windows users
    try {
        if (ifs.fail()) { return false;}
        std::string header;
        int w, h, b;
        ifs >> header;
        if (strcmp(header.c_str(), "P6") != 0) return false;
        ifs >> w >> h >> b;
        this->w = w; this->h = h;
        this->pixels = new Rgb[w * h]; // this is throw an exception if bad_alloc
        ifs.ignore(256, '\n'); // skip empty lines in necessary until we get to the binary data
        unsigned char pix[3];
        // read each pixel one by one and convert bytes to floats
        for (int i = 0; i < w * h; ++i) {
            ifs.read(reinterpret_cast<char *>(pix), 3);
            this->pixels[i].r = pix[0];
            this->pixels[i].g = pix[1];
            this->pixels[i].b = pix[2];
            if (this->pixels[i].r > 0.7) this->pixels[i].r;
            if (this->pixels[i].g > 0.7) this->pixels[i].g;
            if (this->pixels[i].b > 0.7) this->pixels[i].b;
        }
        ifs.close();
        return true;
    }
    catch (const char *err) {
        fprintf(stderr, "%s\n", err);
        ifs.close();
    }
    return false;
}
bool Image::loadRaw(string filename)
{
    return false;
}
bool Image::savePPM(string filename)
{
    if (this->w == 0 || this->h == 0) {return false;}
    std::ofstream ofs;
    try {
        ofs.open(filename, std::ios::binary); // need to spec. binary mode for Windows users
        if (ofs.fail()) return false;
        ofs << "P6\n"<< this->w << " " << this->h << "\n255\n";
        unsigned char r, g, b;
        // loop over each pixel in the image, clamp and convert to byte format
        for (int i = 0; i < this->w * this->h; ++i) {
            r = static_cast<unsigned char>(std::min(1.f, (float)this->pixels[i].r)* this->pixels[i].r);
            g = static_cast<unsigned char>(std::min(1.f, (float)this->pixels[i].g)* this->pixels[i].g);
            b = static_cast<unsigned char>(std::min(1.f, (float)this->pixels[i].b)* this->pixels[i].b);
            ofs << r << g << b;
        }
        ofs.close();
        return true;
    }
    catch (const char *err) {
        fprintf(stderr, "%s\n", err);
        ofs.close();
    }
    return false;
}


void Image::filterRed()
{
    for (int i = 0; i < this->w * this->h; ++i) {
        this->pixels[i].g = 0;
        this->pixels[i].b = 0;
    }

}
void Image::filterGreen()
{
    for (int i = 0; i < this->w * this->h; ++i) {
        this->pixels[i].r = 0;
        this->pixels[i].b = 0;
    }

}
void Image::filterBlue()
{
    for (int i = 0; i < this->w * this->h; ++i) {
        this->pixels[i].g = 0;
        this->pixels[i].r = 0;
    }

}
void Image::greyScale()
{

//    for (int i = 0; i < this->w * this->h; ++i) {
//        this->pixels[i].r = static_cast<unsigned char>((float)this->pixels[i].r*0.5);
//        this->pixels[i].g = static_cast<unsigned char>((float)this->pixels[i].g*0.5);
//        this->pixels[i].b = static_cast<unsigned char>((float)this->pixels[i].b*0.5);
//
//    }
}
void Image::flipHorizontal()
{

}
void Image::flipVertically()
{


}
void Image::AdditionalFunction2()
{

}
void Image::AdditionalFunction3()
{

}
void Image::AdditionalFunction1()
{

}

/* Functions used by the GUI - DO NOT MODIFY */
int Image::getWidth()
{
    return w;
}

int Image::getHeight()
{
    return h;
}

Rgb* Image::getImage()
{
    return pixels;
}