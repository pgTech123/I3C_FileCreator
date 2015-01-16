/*********************************************************
 * LayerStack.cpp
 * Author:      Pascal Gendron
 * Version:     0.1.0
 * *******************************************************/


#include "layerstack.h"

LayerStack::LayerStack()
{
    m_LayerArray = NULL;
    m_iCurrentLayer = 0;

    m_bSideSizeSet = false;
    m_iSideSize = 0;
}

LayerStack::~LayerStack()
{
    if(m_LayerArray != NULL){
        delete[] m_LayerArray;
    }
}

/* Cube Properties */
int LayerStack::setSideSize(int sideSize)
{
    if(!m_bSideSizeSet)
    {
        if(isBase2(sideSize)){
            m_iSideSize = sideSize;
            m_bSideSizeSet = true;

            /* Create Layer Stack */
            m_LayerArray = new Layer[m_iSideSize];
            for(int i = 0; i < m_iSideSize; i++){
                m_LayerArray[i].setSideSize(m_iSideSize);
            }
            layerStackCreated(m_iSideSize);

            return NO_ERRORS;
        }
        return SIDE_SIZE_NOT_BASE_2;
    }
    return SIDE_SIZE_ALREADY_SET;
}

int LayerStack::getSideSize()
{
    if(m_bSideSizeSet){
        return m_iSideSize;
    }
    else{
        return -1;
    }
}

/* Navigation */
void LayerStack::nextLayer()
{
    if(m_bSideSizeSet){
        if(m_iCurrentLayer < m_iSideSize-1){
                m_iCurrentLayer++;
                setWorkspaceAsCurrentLayer();
        }
    }
}

void LayerStack::previousLayer()
{
    if(m_bSideSizeSet){
        if(m_iCurrentLayer > 0){
            m_iCurrentLayer--;
            setWorkspaceAsCurrentLayer();
        }
    }
}

void LayerStack::goToLayer(int layer)
{
    if(m_bSideSizeSet){
        if(layer >= 0 && layer < m_iSideSize){
                m_iCurrentLayer = layer;
                setWorkspaceAsCurrentLayer();
        }
    }
}

int LayerStack::getCurrentLayer()
{
    if(m_bSideSizeSet){
        return m_iCurrentLayer;
    }
    else{
        return -1;
    }
}

/* Access Data */
bool LayerStack::pixelWritten(int x, int y, int z, int w, int h, int d)
{
    if(m_bSideSizeSet){
        int maxX = x + w;
        int maxY = y + h;
        int maxZ = z + d;

        /* Check for errors */
        if(x >= m_iSideSize || y >= m_iSideSize || z >= m_iSideSize ||
                maxX > m_iSideSize || maxY > m_iSideSize || maxZ > m_iSideSize){
            return false;
        }

        unsigned char *transparencyMap;
        for(int posZ = z; posZ < maxZ; posZ++){
            transparencyMap = m_LayerArray[posZ].getTransparencyMapPtr();

            for(int posX = x; posX < maxX; posX++){
                for(int posY = y; posY < maxY; posY++){
                    /* Convert to be compatible with layer */
                    if(transparencyMap[posX + posY*m_iSideSize] != 0){
                        return true;
                    }
                }
            }
        }
    }
    return false;
}

Pixel LayerStack::getPixelAt(int x, int y, int z)
{
    if(m_bSideSizeSet){
        return m_LayerArray[z].getPixel(x,y);
    }
    return emptyPixel();
}

Pixel LayerStack::emptyPixel()
{
    Pixel pixel;
    pixel.red = 0;
    pixel.green = 0;
    pixel.blue = 0;
    return pixel;
}
