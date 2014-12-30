/*********************************************************
 * Layer.cpp
 * Author:      Pascal Gendron
 * Version:     0.0.1
 * *******************************************************/


#include "layer.h"

Layer::Layer()
{
    m_PixmapLayer = NULL;
    m_Painter = NULL;
}

Layer::~Layer()
{
    if(m_PixmapLayer != NULL){
        delete m_PixmapLayer;
    }
    if(m_Painter != NULL){
        delete m_Painter;
    }
}

void Layer::setSideSize(int sideSize)
{
    m_iSideSize = sideSize;
    m_PixmapLayer = new QPixmap(m_iSideSize, m_iSideSize);
    m_PixmapLayer->fill(Qt::black);
    m_Painter = new QPainter(m_PixmapLayer);
}

QPixmap* Layer::getPixmapPtr()
{
    return m_PixmapLayer;
}

void Layer::writePixel(int x, int y, int r, int g, int b)
{
    m_Painter->setPen(QPen(QBrush(QColor(r, g, b)),1));
    m_Painter->drawPoint(x,y);
}
