/*********************************************************
 * Image.cpp
 * Author:      Pascal Gendron
 * Version:     0.0.1
 * *******************************************************/

#include "image.h"

Image::Image()
{
}

Image::~Image()
{
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

    if(!m_i3cFile.isInitialized()){
        ifstream file;
        file.open(m_strFilePath.c_str());
        if(file.is_open()){
            readHeader(&file);
            /* Read Data */
            readPixels(&file);
            readReferences(&file);

            file.close();
            return NO_ERRORS;
        }
        return UNABLE_TO_OPEN_FILE;
    }
    return IMAGE_ALREADY_LOADED;
}

bool Image::setSideSize(int sideSize)
{
    return m_i3cFile.setSideSize(sideSize);
}

int Image::convertLayerStackToImage(LayerStack *layerStack)
{
    int convertingLevel = m_i3cFile.getNumOfLevel();
    while(convertingLevel > 1){
        convertReferencesLS2Img(layerStack, convertingLevel);
        convertingLevel--;
    }
    convertPixelsLS2Img(layerStack);

    return NO_ERRORS;
}

int Image::convertImageToLayerStack(LayerStack **ptrLayerStack)
{
    if(m_i3cFile.isInitialized()){
        (*ptrLayerStack)->setSideSize(m_i3cFile.getSideSize());

        //TODO

        return NO_ERRORS;
    }
    return NO_IMAGE;
}

void Image::writeExtension()
{
    if(!m_strFilePath.empty()){
        /*Verify if ends by .i3c: if not, make it*/
        int strSize = m_strFilePath.size();
        if(strSize <= 4){
            m_strFilePath.append(".i3c");
        }
        else if(m_strFilePath.compare(strSize-4,4,".i3c") != 0 &&
                m_strFilePath.compare(strSize-4,4,".I3C") != 0){
            m_strFilePath.append(".i3c");
        }
    }
}

bool Image::writeI3CFile()
{
    if(m_i3cFile.isInitialized()){
        ofstream file;
        file.open(m_strFilePath.c_str());
        if(file.is_open()){
            writeHeader(&file);

            /* Write Data */
            writePixels(&file);
            writeReferences(&file);

            file.close();
            return true;
        }
        return false;
    }
    return false;
}

void Image::convertReferencesLS2Img(LayerStack* layerStack, int level)
{
    /* Verify parameters */
    if(level <= m_i3cFile.getNumOfLevel()){
        return;
    }
    unsigned char map = 0;
    if(level == m_i3cFile.getNumOfLevel()){
        /* Create first map */
        for(int i = 0; i < 8; i++){
            if(layerStack->isAPixelWritten(/*TODO*/)){
                map = (map | (0x0001 << i));
            }
        }
        m_i3cFile.setMap(level, 0, map);
    }
    else{
        /* Look previous maps and try to find childs
         * (count childs of the upper level and fill them) */
        //TODO
    }
}

void Image::convertPixelsLS2Img(LayerStack *layerStack)
{
    /* Look previous maps and try to find childs */
    //TODO
}

void Image::readHeader(ifstream *file)
{
    //TODO
}

void Image::readPixels(ifstream *file)
{
    //TODO:
}

void Image::readReferences(ifstream *file)
{
    //TODO
}

void Image::writeHeader(ofstream *file)
{
    *file << m_i3cFile.getSideSize() << endl;
    *file << m_i3cFile.countTotalCubes() << endl;

    for(int i = 1; i <= m_i3cFile.getNumOfLevel(); i++){
        *file << m_i3cFile.countTotalCubesAtLevel(i) << endl;
    }
}

void Image::writePixels(ofstream *file)
{
    //TODO
}

void Image::writeReferences(ofstream *file)
{
    //TODO
}
