/*********************************************************
 * Image.cpp
 * Author:      Pascal Gendron
 * Version:     0.1.0
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
            m_i3cFile.deleteImageData();
            readHeader(&file);
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

ChildCorners Image::generateChildCorners(CubeMap map, int mapSize)
{
    /* Preset at 0 as we're not sure they'll be set */
    ChildCorners childCorners;
    for(int i = 0; i < 8; i++){
        childCorners.x[i] = 0;
        childCorners.y[i] = 0;
        childCorners.z[i] = 0;
    }

    int halfMapSize = mapSize / 2;

    if(map.map & 0x01){
        childCorners.x[0] = map.x;
        childCorners.y[0] = map.y;
        childCorners.z[0] = map.z;
    }
    if(map.map & 0x02){
        childCorners.x[1] = map.x + halfMapSize;
        childCorners.y[1] = map.y;
        childCorners.z[1] = map.z;
    }
    if(map.map & 0x04){
        childCorners.x[2] = map.x + halfMapSize;
        childCorners.y[2] = map.y + halfMapSize;
        childCorners.z[2] = map.z;
    }
    if(map.map & 0x08){
        childCorners.x[3] = map.x;
        childCorners.y[3] = map.y + halfMapSize;
        childCorners.z[3] = map.z;
    }
    if(map.map & 0x10){
        childCorners.x[4] = map.x;
        childCorners.y[4] = map.y;
        childCorners.z[4] = map.z + halfMapSize;
    }
    if(map.map & 0x20){
        childCorners.x[5] = map.x + halfMapSize;
        childCorners.y[5] = map.y;
        childCorners.z[5] = map.z + halfMapSize;
    }
    if(map.map & 0x40){
        childCorners.x[6] = map.x + halfMapSize;
        childCorners.y[6] = map.y + halfMapSize;
        childCorners.z[6] = map.z + halfMapSize;
    }
    if(map.map & 0x80){
        childCorners.x[7] = map.x;
        childCorners.y[7] = map.y + halfMapSize;
        childCorners.z[7] = map.z + halfMapSize;
    }
    return childCorners;
}

int Image::convertLayerStackToImage(LayerStack *layerStack)
{
    /* Make sure to clean everything that was done previously in  i3cFile */
    m_i3cFile.resetImage();

    int error = NO_ERRORS;
    int levelBeingConverted = m_i3cFile.getNumOfLevel();

    /* Convertion */
    while(levelBeingConverted > 1){
        error = convertReferencesLS2Img(layerStack, levelBeingConverted);
        levelBeingConverted--;

        if(error != NO_ERRORS){
            return error;
        }
    }
    error = convertPixelsLS2Img(layerStack);

    return error;
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
            writePixels(&file);
            writeReferences(&file);

            file.close();
            return true;
        }
        return false;
    }
    return false;
}

int Image::convertReferencesLS2Img(LayerStack* layerStack, int level)
{
    /* Verify parameters */
    if(level > m_i3cFile.getNumOfLevel()){
        return LEVLE_OUT_OF_RANGE;
    }

    if(level == m_i3cFile.getNumOfLevel()){
        CubeMap cubeMap;
        cubeMap = getMapFromLayerStack(layerStack, 0, 0, 0, pow(2,level));
        return m_i3cFile.setMapAndPos(level, cubeMap);
    }
    else{
        return setChildMapViaParentMap(layerStack, level);
    }
}

int Image::convertPixelsLS2Img(LayerStack *layerStack)
{
    int error;
    CubeMap parent_sMap;
    for(int i = 0; i < m_i3cFile.countTotalCubesAtLevel(2); i++){
        parent_sMap = m_i3cFile.getMapAndPos(2, i);
        ChildCorners childCorners = generateChildCorners(parent_sMap, 4);

        for(int count = 0; count < 8; count++){
            if(parent_sMap.map & (0x01 << count)){
                error = setCubePixels(layerStack,
                                 childCorners.x[count],
                                 childCorners.y[count],
                                 childCorners.z[count]);

                if(error != NO_ERRORS){
                    return error;
                }
            }
        }
    }
    return error;
}

int Image::setChildMapViaParentMap(LayerStack *layerStack, int level)
{
    CubeMap parent_sMap;
    ChildCorners childCorners;
    int sideSize = pow(2, level);
    int error;

    /* We loop through all parent's maps */
    for(int i = 0; i < m_i3cFile.countTotalCubesAtLevel(level+1); i++){
        parent_sMap = m_i3cFile.getMapAndPos(level+1, i);
        childCorners = generateChildCorners(parent_sMap, 2*sideSize);

        for(int count = 0; count < 8; count ++){
            if(parent_sMap.map & (0x01 << count)){
                error = setChildMap(layerStack,
                                    childCorners.x[count],
                                    childCorners.y[count],
                                    childCorners.z[count],
                                    level,
                                    sideSize);
                if(error != NO_ERRORS){
                    return error;
                }
            }
        }
    }
    return error;
}

int Image::setChildMap(LayerStack *layerStack, int x, int y, int z, int level, int sideSize)
{
    CubeMap mapAndPos = getMapFromLayerStack(layerStack, x, y, z, sideSize);
    return m_i3cFile.setMapAndPos(level, mapAndPos);
}

CubeMap Image::getMapFromLayerStack(LayerStack *layerStack, int x, int y, int z, int sideSize)
{
    CubeMap map;
    map.map = 0xFF;
    map.x = x;
    map.y = y;
    map.z = z;

    ChildCorners childCorners = generateChildCorners(map, sideSize);
    int halfSideSize = sideSize / 2;

    map.map = 0;

    for(int i = 0 ; i < 8; i++){
        if(layerStack->pixelWritten(childCorners.x[i],
                                       childCorners.y[i],
                                       childCorners.z[i],
                                       halfSideSize,
                                       halfSideSize,
                                       halfSideSize)){
            map.map = (map.map | (0x01 << i));
        }
    }

    return map;
}

int Image::setCubePixels(LayerStack *layerStack, int x, int y, int z)
{
    Pixel pixels[8];
    CubeMap map = getMapFromLayerStack(layerStack, x, y, z, 2);
    ChildCorners childCorners = generateChildCorners(map, 2);

    for(int i = 0; i < 8; i++){
        pixels[i] = layerStack->getPixelAt(childCorners.x[i],
                                           childCorners.y[i],
                                           childCorners.z[i]);
    }

    return m_i3cFile.setPixel(map, pixels);
}

void Image::readHeader(ifstream *file)
{
    int sideSize;
    *file >> sideSize;
    m_i3cFile.setSideSize(sideSize);

    int numOfLevels = firstHighBit(sideSize);
    int buffer = 0;

    for(int level = 1; level <= numOfLevels; level++){
        *file >> buffer;
        m_i3cFile.setTotalCubesAtLevel(level, buffer);
    }
}

void Image::readPixels(ifstream *file)
{
    int cubesAtLevel1 = m_i3cFile.countTotalCubesAtLevel(1);

    int i_map = 0;
    int red;
    int green;
    int blue;
    Pixel pixels[8];
    CubeMap map;

    for(int i = 0; i < cubesAtLevel1; i++){
        /* Map Reading */
        *file >> i_map;
        map.map = (unsigned char) i_map;
        map.x = 0;
        map.y = 0;
        map.z = 0;

        /* Pixel Reading */
        for(int pixel = 0; pixel < 8; pixel++){
            if(i_map & (0x01 << pixel)){
                *file >> red;
                *file >> green;
                *file >> blue;

                /* Convert to pixel */
                pixels[pixel].red = red;
                pixels[pixel].green = green;
                pixels[pixel].blue = blue;
            }
        }

        /* Set these data in m_i3cFile */
        m_i3cFile.setPixel(map, pixels);
    }
}

void Image::readReferences(ifstream *file)
{
    int numOfLevel = m_i3cFile.getNumOfLevel();
    int i_map = 0;

    for(int level = 2; level < numOfLevel; level ++){
        for(int i = 0; i < m_i3cFile.countTotalCubesAtLevel(level); i++){
            *file >> i_map;
            //TODO: insert in i3c
        }
    }
}

void Image::writeHeader(ofstream *file)
{
    *file << m_i3cFile.getSideSize() << endl;

    /* Starting by Pixel Level */
    for(int i = 1; i <= m_i3cFile.getNumOfLevel(); i++){
        *file << m_i3cFile.countTotalCubesAtLevel(i) << endl;
    }
}

void Image::writePixels(ofstream *file)
{
    CubeMap mapPos;
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
    CubeMap mapPos;
    for(int level = 2; level <= m_i3cFile.getNumOfLevel(); level++){
        for(int i = 0; i < m_i3cFile.countTotalCubesAtLevel(level); i++){
            mapPos = m_i3cFile.getMapAndPos(level, i);
            *file << (int)mapPos.map << endl;
        }
    }
}
