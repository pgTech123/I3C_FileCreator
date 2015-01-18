/*********************************************************
 * Layer.h
 * Author:      Pascal Gendron
 * Version:     0.0.1
 * *******************************************************/


#ifndef LAYER_H
#define LAYER_H

#include <QPixmap>
#include <QPainter>
#include <QImage>
#include <QColor>
#include <QRgb>
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
    QPixmap* getPixmapPtr();
    unsigned char* getTransparencyMapPtr();
    Pixel getPixel(int x, int y);

    void setImageEmpty();
    void writePixel(int x, int y, int r, int g, int b);

private:
    int m_iSideSize;
    QPixmap *m_PixmapLayer;
    unsigned char *m_transparencyMap;
    QPainter *m_Painter;
};

#endif // LAYER_H
