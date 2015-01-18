/*********************************************************
 * Layer.cpp
 * Author:      Pascal Gendron
 * Version:     0.0.1
 * *******************************************************/


#include "layer.h"

Layer::Layer()
{
    cout << "Layer Created" << endl;
    m_PixmapLayer = NULL;
    m_transparencyMap = NULL;
    m_Painter = new QPainter();
}

Layer::~Layer()
{
    if(m_PixmapLayer != NULL){
        delete m_PixmapLayer;
    }
    if(m_transparencyMap != NULL){
        delete[] m_transparencyMap;
    }
    delete m_Painter;
}

void Layer::setSideSize(int sideSize)
{
    cout << "Layer Created with side size" << endl;
    m_iSideSize = sideSize;
    m_PixmapLayer = new QPixmap(m_iSideSize, m_iSideSize);
    m_transparencyMap = new unsigned char[m_iSideSize*m_iSideSize];
    setImageEmpty();
    m_PixmapLayer->fill(Qt::black);
}

QPixmap* Layer::getPixmapPtr()
{
    return m_PixmapLayer;
}

unsigned char* Layer::getTransparencyMapPtr()
{
    return m_transparencyMap;
}

Pixel Layer::getPixel(int x, int y)
{
    QRgb rgb =  m_PixmapLayer->toImage().pixel(x,y);
    Pixel pix;
    pix.red = QColor(rgb).red();
    pix.green = QColor(rgb).green();
    pix.blue = QColor(rgb).blue();

    return pix;
}

void Layer::setImageEmpty()
{
    int transparencyMapSize = m_iSideSize * m_iSideSize;
    for(int i = 0; i < transparencyMapSize; i++)
    {
        m_transparencyMap[i] = 0;
    }
}

void Layer::writePixel(int x, int y, int r, int g, int b)
{
    cout<< "Write Pixel called" << endl;
    /*m_Painter->begin(m_PixmapLayer);
    m_Painter->setPen(QPen(QBrush(QColor(r, g, b)),1));
    m_Painter->drawPoint(x,y);
    m_transparencyMap[x + m_iSideSize * y] = 255;
    m_Painter->end();*/
}
