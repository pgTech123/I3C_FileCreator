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
        ofstream file;
        file.open(m_strFilePath.c_str());
        if(file.is_open()){
            /* Write Header */
            file << m_i3cFile.sideSize << endl;

            int totalCubes = countTotalCubes();
            file << totalCubes << endl;

            int *cubesAtLevel = new int[m_i3cFile.numOfLevels];
            for(int i = 0; i < m_i3cFile.numOfLevels; i++){
                cubesAtLevel[i] = countTotalCubesAtLevel(i+1);
                file << cubesAtLevel[i] << endl;
            }

            /* Write Data */
            for(int i = 0; i < cubesAtLevel[0]; i++){
                /* Write Pixel Data */
                //TODO
            }
            for(int level = 1; level < m_i3cFile.numOfLevels; level++){
                for(int i = 0; i < cubesAtLevel[level]; i++){
                    /* Write References */
                    //TODO
                }
            }

            delete[] cubesAtLevel;
            file.close();
            return true;
        }
        return false;
    }
    return false;
}

int Image::countTotalCubes()
{
    //TODO
    return 0;
}

int Image::countTotalCubesAtLevel(int level)
{
    //TODO
    return 0;
}
