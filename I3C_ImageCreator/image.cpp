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
    m_i3cFile.resetImage();
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

    if(level == m_i3cFile.getNumOfLevel()){
        MapAndPos mapAndPos;
        mapAndPos.map = 0;
        mapAndPos = getMapFromLayerStack(layerStack, 0, 0, 0, pow(2,level-1));
        m_i3cFile.setMapAndPos(level, mapAndPos);   //Possiblity to capture errors here
    }
    else{
        /* Cube has a parent and a child */
        setChildMapViaParentMap(layerStack, level);
    }
}

void Image::convertPixelsLS2Img(LayerStack *layerStack)
{
    MapAndPos parent_sMap;
    for(int i = 0; i < m_i3cFile.countTotalCubesAtLevel(2); i++){
        parent_sMap = m_i3cFile.getMapAndPos(2, i);

        if(parent_sMap.map & 0x01){
            setCubePixels(layerStack, parent_sMap.x, parent_sMap.y, parent_sMap.z);
        }
        if(parent_sMap.map & 0x02){
            setCubePixels(layerStack, parent_sMap.x + 2, parent_sMap.y, parent_sMap.z);
        }
        if(parent_sMap.map & 0x04){
            setCubePixels(layerStack, parent_sMap.x + 2, parent_sMap.y + 2, parent_sMap.z);
        }
        if(parent_sMap.map & 0x08){
            setCubePixels(layerStack, parent_sMap.x, parent_sMap.y + 2, parent_sMap.z);
        }
        if(parent_sMap.map & 0x10){
            setCubePixels(layerStack, parent_sMap.x, parent_sMap.y, parent_sMap.z + 2);
        }
        if(parent_sMap.map & 0x20){
            setCubePixels(layerStack, parent_sMap.x + 2, parent_sMap.y, parent_sMap.z + 2);
        }
        if(parent_sMap.map & 0x40){
            setCubePixels(layerStack, parent_sMap.x + 2, parent_sMap.y + 2, parent_sMap.z + 2);
        }
        if(parent_sMap.map & 0x80){
            setCubePixels(layerStack, parent_sMap.x, parent_sMap.y + 2, parent_sMap.z + 2);
        }
    }
}

void Image::setChildMapViaParentMap(LayerStack *layerStack, int level)
{
    MapAndPos parent_sMap;
    int sideSize = pow(2, level-1);
    /* We loop through all parent's maps */
    for(int i = 0; i < m_i3cFile.countTotalCubesAtLevel(level+1); i++){
        parent_sMap = m_i3cFile.getMapAndPos(level+1, i);

        /* Look at map and associate to position in cube */
        if(parent_sMap.map & 0x01){
            setChildMap(layerStack, parent_sMap.x, parent_sMap.y, parent_sMap.z, level, sideSize);
        }
        if(parent_sMap.map & 0x02){
            setChildMap(layerStack, parent_sMap.x+sideSize, parent_sMap.y, parent_sMap.z, level, sideSize);
        }
        if(parent_sMap.map & 0x04){
            setChildMap(layerStack, parent_sMap.x+sideSize, parent_sMap.y+sideSize, parent_sMap.z, level, sideSize);
        }
        if(parent_sMap.map & 0x08){
            setChildMap(layerStack, parent_sMap.x, parent_sMap.y+sideSize, parent_sMap.z, level, sideSize);
        }
        if(parent_sMap.map & 0x10){
            setChildMap(layerStack, parent_sMap.x, parent_sMap.y, parent_sMap.z+sideSize, level, sideSize);
        }
        if(parent_sMap.map & 0x20){
            setChildMap(layerStack, parent_sMap.x+sideSize, parent_sMap.y, parent_sMap.z+sideSize, level, sideSize);
        }
        if(parent_sMap.map & 0x40){
            setChildMap(layerStack, parent_sMap.x+sideSize, parent_sMap.y+sideSize, parent_sMap.z+sideSize, level, sideSize);
        }
        if(parent_sMap.map & 0x80){
            setChildMap(layerStack, parent_sMap.x, parent_sMap.y+sideSize, parent_sMap.z+sideSize, level, sideSize);
        }
    }
}

void Image::setChildMap(LayerStack *layerStack, int x, int y, int z, int level, int sideSize)
{
    MapAndPos mapAndPos = getMapFromLayerStack(layerStack, x, y, z, sideSize);
    m_i3cFile.setMapAndPos(level, mapAndPos);
}

MapAndPos Image::getMapFromLayerStack(LayerStack *layerStack, int x, int y, int z, int sideSize)
{
    MapAndPos mapAndPos;
    mapAndPos.map = 0;
    mapAndPos.x = x;
    mapAndPos.y = y;
    mapAndPos.z = z;

    if(layerStack->isAPixelWritten(x,y,z,sideSize,sideSize,sideSize)){
        mapAndPos.map = (mapAndPos.map | (0x0001));
    }
    if(layerStack->isAPixelWritten(x+sideSize,y,z,sideSize,sideSize,sideSize)){
        mapAndPos.map = (mapAndPos.map | (0x0002));
    }
    if(layerStack->isAPixelWritten(x+sideSize,y+sideSize,z,sideSize,sideSize,sideSize)){
        mapAndPos.map = (mapAndPos.map | (0x0004));
    }
    if(layerStack->isAPixelWritten(x,y+sideSize,z,sideSize,sideSize,sideSize)){
        mapAndPos.map = (mapAndPos.map | (0x0008));
    }
    if(layerStack->isAPixelWritten(x,y,z+sideSize,sideSize,sideSize,sideSize)){
        mapAndPos.map = (mapAndPos.map | (0x0010));
    }
    if(layerStack->isAPixelWritten(x+sideSize,y,z+sideSize,sideSize,sideSize,sideSize)){
        mapAndPos.map = (mapAndPos.map | (0x0020));
    }
    if(layerStack->isAPixelWritten(x+sideSize,y+sideSize,z+sideSize,sideSize,sideSize,sideSize)){
        mapAndPos.map = (mapAndPos.map | (0x0040));
    }
    if(layerStack->isAPixelWritten(x,y+sideSize,z+sideSize,sideSize,sideSize,sideSize)){
        mapAndPos.map = (mapAndPos.map | (0x0080));
    }
    return mapAndPos;
}

void Image::setCubePixels(LayerStack *layerStack, int x, int y, int z)
{
    Pixel pixels[8];
    MapAndPos mapAndPos = getMapFromLayerStack(layerStack, x, y, z, 1);

    pixels[0] = layerStack->getPixelAt(x  , y  , z  );
    pixels[1] = layerStack->getPixelAt(x+1, y  , z  );
    pixels[2] = layerStack->getPixelAt(x+1, y+1, z  );
    pixels[3] = layerStack->getPixelAt(x  , y+1, z  );
    pixels[4] = layerStack->getPixelAt(x  , y  , z+1);
    pixels[5] = layerStack->getPixelAt(x+1, y  , z+1);
    pixels[6] = layerStack->getPixelAt(x+1, y+1, z+1);
    pixels[7] = layerStack->getPixelAt(x  , y+1, z+1);

    m_i3cFile.setPixel(mapAndPos.map, pixels);
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
        *file << (int)mapPos.map << endl;
        for(int pix = 0; pix < 8 ; pix++){
            if(mapPos.map & (0x01 << pix)){
                *file << (int)m_i3cFile.getRed(i, pix) << endl;
                *file << (int)m_i3cFile.getGreen(i, pix) << endl;
                *file << (int)m_i3cFile.getBlue(i, pix) << endl;
            }
        }
    }
}

void Image::writeReferences(ofstream *file)
{
    MapAndPos mapPos;
    for(int level = 1; level <= m_i3cFile.getNumOfLevel(); level++){
        for(int i = 0; i < m_i3cFile.countTotalCubesAtLevel(level); i++){
            mapPos = m_i3cFile.getMapAndPos(level, i);
            *file << (int)mapPos.map << endl;
        }
    }
}
