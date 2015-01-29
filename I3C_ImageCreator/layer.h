/*********************************************************
 * Layer.h
 * Author:      Pascal Gendron
 * Version:     0.0.1
 * *******************************************************/


#ifndef LAYER_H
#define LAYER_H

#include <pixel.h>

/*DEBUG PURPOSE*/
#include <iostream>
using namespace std;
/*END DEBUG*/


/******************************************************************
 * The purpose of this class is to represent a single layer of the
 * image.
 * ****************************************************************/

class Layer
{
public:
    Layer();
    ~Layer();

    void setSideSize(int sideSize);
    int getSideSize();

    unsigned char* getTransparencyMapPtr();
    unsigned char getTransparency(int x, int y);
    Pixel getPixel(int x, int y);
    Pixel getPixel(int index);

    void setImageEmpty();
    void setPixelTransparent(int x, int y, unsigned char transparency = 0);
    void writePixel(int x, int y, int r, int g, int b);

private:
    int m_iSideSize;
    Pixel *m_PixelMap;
    unsigned char *m_transparencyMap;
};

#endif // LAYER_H
