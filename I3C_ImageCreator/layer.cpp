/*********************************************************
 * Layer.cpp
 * Author:      Pascal Gendron
 * Version:     0.0.1
 * *******************************************************/


#include "layer.h"

Layer::Layer()
{
    m_PixmapLayer = NULL;
    m_transparencyMap = NULL;
    m_Painter = NULL;
}

Layer::~Layer()
{
    if(m_PixmapLayer != NULL){
        delete m_PixmapLayer;
    }
    if(m_transparencyMap != NULL){
        delete[] m_transparencyMap;
    }
    if(m_Painter != NULL){
        delete m_Painter;
    }
}

void Layer::setSideSize(int sideSize)
{
    m_iSideSize = sideSize;
    m_PixmapLayer = new QPixmap(m_iSideSize, m_iSideSize);
    m_transparencyMap = new unsigned char[m_iSideSize*m_iSideSize];
    m_PixmapLayer->fill(Qt::black);
    m_Painter = new QPainter();
}

QPixmap* Layer::getPixmapPtr()
{
    return m_PixmapLayer;
}

unsigned char* Layer::getTransparencyMapPtr()
{
    return m_transparencyMap;
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
    m_Painter->begin(m_PixmapLayer);
    m_Painter->setPen(QPen(QBrush(QColor(r, g, b)),1));
    m_Painter->drawPoint(x,y);
    m_transparencyMap[x + m_iSideSize * y] = 255;
    m_Painter->end();
}
