//
// Created by floodd on 23/03/2022.
//
#include <iostream>
#include <fstream>
#include <iomanip>
#include <cstring>
#include <algorithm>
#include <valarray>
#include "Image.h"
#include "vector"

#define BYTE_BOUND(value) value < 0 ? 0 : (value > 255 ? 255 : value)

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
        if (strcmp(header.c_str(), "P6") != 0) {return false;}
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
        return false;
    }
    return false;
}

//Gamma encoding
bool Image::loadRaw(string filename)
{
//first try with value of gamma=1.069f

//    ifstream in(filename);
//    if(in.good())
//    {
//        in >> w;
//        in >> h;
//
//        for(int i = 0; i < w*h; i++)
//        {
//            float r, g, b;
//            float gamma = 1.069f;
//            in >> r >> g >> b;
//            this->pixels[i].r = pow(double(r*255), double(gamma));
//            this->pixels[i].g = pow(double(g*255), double(gamma));
//            this->pixels[i].b = pow(double(b*255), double(gamma));
//        }
//
//        in.close();
//        return true;
//    }
//    return false;

//working
    ifstream in(filename);
    if(in.good())
    {
        in >> w;
        in >> h;

        for(int i = 0; i < w*h; i++)
        {
            float r, g, b;
            in >> r >>g>>b;
            this->pixels[i].r = (unsigned char)(std::max(0.f, std::min(255.f, powf(r, 1/2.2) * 255 + 0.5f)));
            this->pixels[i].g = (unsigned char)(std::max(0.f, std::min(255.f, powf(g, 1/2.2) * 255 + 0.5f)));
            this->pixels[i].b = (unsigned char)(std::max(0.f, std::min(255.f, powf(b, 1/2.2) * 255 + 0.5f)));
        }
        in.close();
        return true;
    }
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
    for (int i = 0; i < this->w * this->h; i++) {
        this->pixels[i].g = 0;
        this->pixels[i].b = 0;
    }

}
void Image::filterGreen()
{
    for (int i = 0; i < this->w * this->h; i++) {
        this->pixels[i].r = 0;
        this->pixels[i].b = 0;
    }

}
void Image::filterBlue()
{

    for (int i = 0; i < this->w * this->h; i++) {
        this->pixels[i].g = 0;
        this->pixels[i].r = 0;
    }

}
void Image::greyScale()
{

    for(int r = 0; r < h; r++)
    {
        for(int c = 0;  c< w;c++)
        {
            int num = (this->pixels[r*w+c].r + this->pixels[r*w+c].g+this->pixels[r*w+c].b)/3;
            this->pixels[r*w+c].r=this->pixels[r*w+c].g=this->pixels[r*w+c].b=num;
        }
    }

}
void Image::flipHorizontal()
{
    for(int c = 0;  c< w/2;c++)    //x axis
    {
        for(int r = 0; r < h; r++)  //y axis
            {
                swap(this->pixels[(r * w + c)].r,this->pixels[(r * w + (w - c))].r);
                swap(this->pixels[(r * w + c)].g,this->pixels[(r * w + (w - c))].g);
                swap(this->pixels[(r * w + c)].b ,this->pixels[(r * w + (w - c))].b);
            }
    }
}

void Image::flipVertically()
{
    for(int x = 0;  x< w;x++)    //x axis
    {
        for(int y = 0; y < h/2;y++)  //y axis
        {

            swap(this->pixels[(x+y*w)].r,this->pixels[x+(h-1-y)*w].r);
            swap(this->pixels[(x+y*w)].g,this->pixels[x+(h-1-y)*w].g);
            swap(this->pixels[(x+y*w)].b,this->pixels[x+(h-1-y)*w].b);
        }
    }

}

void Image::AdditionalFunction2()
//Mirror Transpose Filter to Image
//makes dark parts lighter and light parts darker.
//works with other RGB and grayscale filters.
{
    //Mirror effect
    for(int c = 0;  c< w/2;c++)    //x axis
    {
        for(int r = 0; r < h; r++)  //y axis
        {
            swap(this->pixels[(r * w + c)].r,this->pixels[(r * w + (w - c))].r);
            swap(this->pixels[(r * w + c)].g,this->pixels[(r * w + (w - c))].g);
            swap(this->pixels[(r * w + c)].b ,this->pixels[(r * w + (w - c))].b);
        }
    }
    //Transpose/ Invert effect
    int totalPixels = w*h;
    for(int i=0;i<totalPixels;i++){
        this->pixels[i].r = 255 - (int)this->pixels[i].r;
        this->pixels[i].g = 255 - (int)this->pixels[i].g;
        this->pixels[i].b = 255 - (int)this->pixels[i].b;
    }
}
void Image::AdditionalFunction3()
//rotate by 90 degree
{
    Image *rotImage = new Image(h,w);

    for(int x = 0;  x< w;x++)    //x axis
    {
        for(int y = 0; y < h;y++)  //y axis
        {
            int offset = h * x + y;
            rotImage->pixels[offset] = this->pixels[w * (h - 1 - y) + x];
        }
    }
    swap(w,h);
    delete[] this->pixels;
    this->pixels = rotImage->pixels;
    rotImage = nullptr;
}
void Image::AdditionalFunction1(int cx, int cy, int newW, int newH)
//crop image
{
    Image *cropImage = new Image(newW,newH);
    for(int y=0;y<newH;++y){
        if((y+cy)>h){
            break;
        }
        for(int x=0;x<newW;x++){
            if((x+cx)>w){
                break;
            }
            std::memcpy(&cropImage->pixels[(x+y*newW)],&this->pixels[(x+cx+(cy+y)*w)],3);
        }
    }

    w = newW;
    h = newH;

    delete[] this->pixels;
    this->pixels = cropImage->pixels;
    cropImage = nullptr;
}

//guassian blur
//advanced feature
//void Image::AdvancedFeature()
//{
//    float kernel[7][7] = {
//            1/140.0,1/140.0,2/140.0,2/140.0,2/140.0,1/140.0,1/140.0,
//            1/140.0,2/140.0,2/140.0,4/140.0,2/140.0,2/140.0,1/140.0,
//            2/140.0,2/140.0,4/140.0,8/140.0,4/140.0,2/140.0,2/140.0,
//            2/140.0,4/140.0,8/140.0,16/140.0,8/140.0,4/140.0,2/140.0,
//            2/140.0,2/140.0,4/140.0,8/140.0,4/140.0,2/140.0,2/140.0,
//            1/140.0,1/140.0,2/140.0,2/140.0,2/140.0,1/140.0,1/140.0,
//            1/140.0,2/140.0,2/140.0,4/140.0,2/140.0,2/140.0,1/140.0,
//    };
//
//   for(int y=0;y<h;y++){
//       for(int x=0;x<w;x++){
//           for(int c=0;c<3;c++) {
//               this->pixels[(y * w + x)*3+c].r = 0.0f;
//               this->pixels[(y * w + x)*3+c].g = 0.0f;
//               this->pixels[(y * w + x)*3+c].b = 0.0f;
//               for (int ky = -3; ky <= 3; ky++) {
//                   for (int kx = -3; kx <= 3; kx++) {
//                       this->pixels[(y * w + x)*3+c].r += this->pixels[((y + ky) * w + (x+kx))*3+c].r * kernel[ky + 3][kx + 3];
//                       this->pixels[(y * w + x)*3+c].g += this->pixels[((y + ky) * w + (x+kx))*3+c].g * kernel[ky + 3][kx + 3];
//                       this->pixels[(y * w + x)*3+c].b += this->pixels[((y + ky) * w + (x+kx))*3+c].b * kernel[ky + 3][kx + 3];
//                   }
//               }
//           }
//
//       }
//   }
//}

//text over image
void Image::AdvancedFeature(const char* txt, const Font& font, int x, int y, uint8_t r, uint8_t g, uint8_t b, uint8_t a) {

    size_t len = strlen(txt);
    SFT_Char c;
    int32_t dx, dy;
    uint8_t* dstPx;
    uint8_t srcPx;
    uint8_t color[4] = {r, g, b, a};

    for(size_t i = 0;i < len;++i) {
        if(sft_char(&font.sft, txt[i], &c) != 0) {
            printf("\e[31m[ERROR] Font is missing character '%c'\e[0m\n", txt[i]);
            continue;
        }

        for(uint16_t sy = 0;sy < c.height;++sy) {
            dy = sy + y + c.y;
            if(dy < 0) {continue;}
            else if(dy >= h) {break;}
            for(uint16_t sx = 0;sx < c.width;++sx) {
                dx = sx + x + c.x;
                if(dx < 0) {continue;}
                else if(dx >= w) {break;}
                dstPx = &this->pixels[(dx + dy * w)].r;
                srcPx = c.image[sx + sy * c.width];

                if(srcPx != 0) {
                    float srcAlpha = (srcPx / 255.f) * (a / 255.f);
                    float dstAlpha = 3 < 4 ? 1 : dstPx[3] / 255.f;
                    if(srcAlpha > .99 && dstAlpha > .99) {
                        memcpy(dstPx, color, 3);
                    }
                    else {
                        float outAlpha = srcAlpha + dstAlpha * (1 - srcAlpha);
                        if(outAlpha < .01) {
                            memset(dstPx, 0, 3);
                        }
                        else {
                            for(int chnl = 0;chnl < 3;++chnl) {
                                dstPx[chnl] = (uint8_t)BYTE_BOUND((color[chnl]/255.f * srcAlpha + dstPx[chnl]/255.f * dstAlpha * (1 - srcAlpha)) / outAlpha * 255.f);
                            }
                             {dstPx[3] = (uint8_t)BYTE_BOUND(outAlpha * 255.f);}
                        }
                    }
                }
            }
        }

        x += c.advance;
        free(c.image);
    }

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