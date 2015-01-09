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
    if(level > m_i3cFile.getNumOfLevel()){
        return;
    }

    MapAndPos mapAndPos;
    mapAndPos.map = 0;

    if(level == m_i3cFile.getNumOfLevel()){
        mapAndPos = getMapFromLayerStack(layerStack, 0, 0, 0, pow(2,level-1));
        m_i3cFile.setMapAndPos(level, mapAndPos);   //Possiblity to capture errors here
    }
    else{
        /* Look previous maps and try to find childs */
        MapAndPos parent_sMap;

        //TODO
        for(int i = 0; i < m_i3cFile.countTotalCubesAtLevel(level); i++){
            parent_sMap = m_i3cFile.getMapAndPos(level+1, i);
            mapAndPos = getMapFromLayerStack(layerStack,
                                             parent_sMap.x,
                                             parent_sMap.y,
                                             parent_sMap.z,
                                             pow(2,level-1));
            m_i3cFile.setMapAndPos(level, mapAndPos);   //Possiblity to capture errors here
        }
    }
}

void Image::convertPixelsLS2Img(LayerStack *layerStack)
{
    unsigned char map = 0;

    /* Look previous maps and try to find childs */

    /* Get pixels */
    //TODO
}

MapAndPos Image::getMapFromLayerStack(LayerStack *layerStack, int x, int y, int z, int sideSize)
{
    MapAndPos mapAndPos;
    mapAndPos.map = 0;

    if(layerStack->isAPixelWritten(x,y,z,sideSize,sideSize,sideSize)){
        mapAndPos.map = (mapAndPos.map | (0x0001));
        mapAndPos.x = x;
        mapAndPos.y = y;
        mapAndPos.z = z;
    }
    if(layerStack->isAPixelWritten(x+sideSize,y,z,sideSize,sideSize,sideSize)){
        mapAndPos.map = (mapAndPos.map | (0x0002));
        mapAndPos.x = x+sideSize;
        mapAndPos.y = y;
        mapAndPos.z = z;
    }
    if(layerStack->isAPixelWritten(x+sideSize,y+sideSize,z,sideSize,sideSize,sideSize)){
        mapAndPos.map = (mapAndPos.map | (0x0004));
        mapAndPos.x = x+sideSize;
        mapAndPos.y = y+sideSize;
        mapAndPos.z = z;
    }
    if(layerStack->isAPixelWritten(x,y+sideSize,z,sideSize,sideSize,sideSize)){
        mapAndPos.map = (mapAndPos.map | (0x0008));
        mapAndPos.x = x;
        mapAndPos.y = y+sideSize;
        mapAndPos.z = z;
    }
    if(layerStack->isAPixelWritten(x,y,z+sideSize,sideSize,sideSize,sideSize)){
        mapAndPos.map = (mapAndPos.map | (0x0010));
        mapAndPos.x = x;
        mapAndPos.y = y;
        mapAndPos.z = z+sideSize;
    }
    if(layerStack->isAPixelWritten(x+sideSize,y,z+sideSize,sideSize,sideSize,sideSize)){
        mapAndPos.map = (mapAndPos.map | (0x0020));
        mapAndPos.x = x+sideSize;
        mapAndPos.y = y;
        mapAndPos.z = z+sideSize;
    }
    if(layerStack->isAPixelWritten(x+sideSize,y+sideSize,z+sideSize,sideSize,sideSize,sideSize)){
        mapAndPos.map = (mapAndPos.map | (0x0040));
        mapAndPos.x = x+sideSize;
        mapAndPos.y = y+sideSize;
        mapAndPos.z = z+sideSize;
    }
    if(layerStack->isAPixelWritten(x,y+sideSize,z+sideSize,sideSize,sideSize,sideSize)){
        mapAndPos.map = (mapAndPos.map | (0x0080));
        mapAndPos.x = x;
        mapAndPos.y = y+sideSize;
        mapAndPos.z = z+sideSize;
    }
    return mapAndPos;
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

    /* Write Starting by Pixel Level */
    for(int i = 1; i <= m_i3cFile.getNumOfLevel(); i++){
        *file << m_i3cFile.countTotalCubesAtLevel(i) << endl;
    }
}

void Image::writePixels(ofstream *file)
{
    MapAndPos mapPos;
    for(int i = 0; i < m_i3cFile.countTotalCubesAtLevel(1); i++){
        mapPos = m_i3cFile.getMapAndPos(1,i);
        *file << mapPos.map; //TODO: Verify if no bug: unsigned char ???
        for(int pix = 0; pix < numberHighBits(mapPos.map) ; pix++){
            //TODO: Write Pixels
            //m_i3cFile.getRed...
        }
    }
}

void Image::writeReferences(ofstream *file)
{
    //TODO
}
