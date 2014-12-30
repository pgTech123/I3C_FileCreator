/*********************************************************
 * Image.cpp
 * Author:      Pascal Gendron
 * Version:     0.0.1
 * *******************************************************/

#include "image.h"

Image::Image()
{
    initialization();
}

Image::~Image()
{
    //TODO: if necessary...
}

void Image::initialization()
{
    m_bNoImageContained = true;
    m_i3cFile.sideSize = 0;
    m_i3cFile.numOfLevels = 0;
}

bool Image::save()
{
    if(m_strFilePath.empty()){
        return false;
    }
    return writeI3CFile();
}

bool Image::save(const char* path)
{
    m_strFilePath = path;
    writeExtension();
    return writeI3CFile();
}

int Image::open(const char* path)
{
    m_strFilePath = path;
    //TODO
    m_bNoImageContained = false;

    return NO_ERRORS;
}

void Image::initializeEmpty()
{
    if(m_i3cFile.sideSize != 0){
        /* Create Minimum Viable File */
        m_i3cFile.content.map = 0;

        m_bNoImageContained = false;
    }
}

bool Image::setSideSize(int sideSize)
{
    if(isBase2(sideSize)){
        m_i3cFile.sideSize = sideSize;
        m_i3cFile.numOfLevels = firstHighBit(sideSize);
        return true;
    }
    return false;
}

int Image::convertLayerStackToImage(LayerStack *layerStack)
{
    //TODO
    return NO_ERRORS;
}

int Image::convertImageToLayerStack(LayerStack **ptrLayerStack)
{
    if(!m_bNoImageContained){
        (*ptrLayerStack)->setSideSize(m_i3cFile.sideSize);

        //TODO

        return NO_ERRORS;
    }
    return NO_IMAGE;
}

void Image::writeExtension()
{
    if(!m_strFilePath.empty()){
        //Verify if ends by .i3c
        //if not write it
        //TODO
    }
}

bool Image::writeI3CFile()
{
    if(!m_bNoImageContained){
        fstream file;
        file.open(m_strFilePath.c_str());
        if(file.is_open()){
            /* Write Header */
            //TODO

            /* Write Data */
            //TODO

            file.close();
            return true;
        }
        return false;
    }
    return false;
}
