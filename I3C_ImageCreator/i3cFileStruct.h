/*********************************************************
 * I3CFileStruct.h
 * Author:      Pascal Gendron
 * Version:     0.0.1
 * *******************************************************/

#ifndef I3CFILESTRUCT_H
#define I3CFILESTRUCT_H

#include <iostream>
using namespace std;

#include "pixel.h"
#include "cubemap.h"
#include "gvbinaryfunctions.h"
#include "constants.h"


class i3cFile{
public:
    i3cFile();
    ~i3cFile();

    /* Image Status */
    bool isInitialized();

    /* Write & Rewrite Image */
    void resetImage();
    void deleteImageData();

    /* Accessors */
    bool setSideSize(int sideSize); //Can only be called once
    int getSideSize();
    int getNumOfLevel();

    int setMapAndPos(int level, CubeMap mapAndPos);
    int setMapAndPos(int level, int index, CubeMap mapAndPos);
    CubeMap getMapAndPos(int level, int index);

    int setPixel(CubeMap map, Pixel pixels[8]);
    unsigned char getRed(int index, int posInMap);
    unsigned char getGreen(int index, int posInMap);
    unsigned char getBlue(int index, int posInMap);

    /* Image Statistics */
    int countTotalCubes();
    int countTotalCubesAtLevel(int level);
    void setTotalCubesAtLevel(int level, int numberOfCubes);

private:
    void initMembers();
    void allocateChilds(int size);
    void delete2DUCArray(unsigned char **array, int arraySize2ndD);

private:
    /* Image */
    int m_iSideSize;
    int m_iNumOfLevels;
    CubeMap **m_MapsAndPos;
    unsigned char **m_ucRed;
    unsigned char **m_ucGreen;
    unsigned char **m_ucBlue;

    /* Safety (access) */
    bool *m_bMapLocked;

    /* Statistics */
    int *m_iptrTotalCubesAtLevel;

    /* Cursors */
    int m_iCurrentLevel;
    int m_iIndexLevel;
};

#endif // I3CFILESTRUCT_H
