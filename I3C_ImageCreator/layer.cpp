/*********************************************************
 * Layer.cpp
 * Author:      Pascal Gendron
 * Version:     0.0.1
 * *******************************************************/


#include "layer.h"

Layer::Layer()
{
    m_PixelMap = NULL;
    m_transparencyMap = NULL;
}

Layer::~Layer()
{
    if(m_PixelMap != NULL){
        delete[] m_PixelMap;
    }
    if(m_transparencyMap != NULL){
        delete[] m_transparencyMap;
    }
}

void Layer::setSideSize(int sideSize)
{
    if(m_PixelMap != NULL){
        delete[] m_PixelMap;
        m_PixelMap = NULL;
    }
    if(m_transparencyMap != NULL){
        delete[] m_transparencyMap;
        m_transparencyMap = NULL;
    }

    m_iSideSize = sideSize;
    m_PixelMap = new Pixel[m_iSideSize * m_iSideSize];
    m_transparencyMap = new unsigned char[m_iSideSize*m_iSideSize];
    setImageEmpty();
}

int Layer::getSideSize()
{
    return m_iSideSize;
}

unsigned char* Layer::getTransparencyMapPtr()
{
    return m_transparencyMap;
}

unsigned char Layer::getTransparency(int x, int y)
{
    int indexInImage = x + (m_iSideSize * y);

    return m_transparencyMap[indexInImage];
}

Pixel Layer::getPixel(int x, int y)
{
    int indexInImage = x + (m_iSideSize * y);

    return m_PixelMap[indexInImage];
}

Pixel Layer::getPixel(int index)
{
    return m_PixelMap[index];
}

void Layer::setImageEmpty()
{
    int numberOfPixels = m_iSideSize * m_iSideSize;
    for(int i = 0; i < numberOfPixels; i++)
    {
        m_PixelMap[i].red = 0;
        m_PixelMap[i].green = 0;
        m_PixelMap[i].blue = 0;
        m_transparencyMap[i] = 0;
    }
}

void Layer::setPixelTransparent(int x, int y, unsigned char transparency)
{
    int indexInImage = x + (m_iSideSize * y);
    m_transparencyMap[indexInImage] = transparency;
}

void Layer::writePixel(int x, int y, int r, int g, int b)
{
    int indexInImage = x + (m_iSideSize * y);

    m_PixelMap[indexInImage].red = r;
    m_PixelMap[indexInImage].green = g;
    m_PixelMap[indexInImage].blue = b;
    m_transparencyMap[indexInImage] = 255;
}
