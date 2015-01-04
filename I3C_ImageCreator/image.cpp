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
    int convertingLevel = m_i3cFile.numOfLevels;
    while(convertingLevel > 1){
        convertReferencesLS2Img(layerStack, convertingLevel);
        convertingLevel--;
    }
    convertPixelsLS2Img(layerStack);

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
    if(!m_bNoImageContained){
        fstream file;
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
    if(level > m_i3cFile.numOfLevels){
        return;
    }

    if(level == m_i3cFile.numOfLevels){
        /* Create first map */
        //TODO
    }
    else{
        /* Look previous maps and try to find childs */
        //TODO
    }
}

void Image::convertPixelsLS2Img(LayerStack *layerStack)
{
    /* Look previous maps and try to find childs */
    //TODO
}

int Image::countTotalCubes()
{
    int count = 0;
    for(int i = 1; i <= m_i3cFile.numOfLevels; i++){
        count += countTotalCubesAtLevel(i);
    }

    return count;
}

int Image::countTotalCubesAtLevel(int level)
{
    int count = 0;
    //TODO
    return count;
}

void Image::writeHeader(fstream *file)
{
    *file << m_i3cFile.sideSize << endl;
    *file << countTotalCubes() << endl;

    for(int i = 1; i <= m_i3cFile.numOfLevels; i++){
        *file << countTotalCubesAtLevel(i) << endl;
    }
}

void Image::writePixels(fstream *file)
{

}

void Image::writeReferences(fstream *file)
{

}
