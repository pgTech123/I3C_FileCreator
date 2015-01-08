#ifndef I3CFILESTRUCT_H
#define I3CFILESTRUCT_H

#include <iostream>
using namespace std;

#include "gvbinaryfunctions.h"

#define NO_ERRORS               0
#define INDEX_OUT_OF_RANGE      1
#define LEVEL_NOT_ACCESSIBLE    2
#define LEVEL_LOCKED            3

class i3cFile{
public:
    i3cFile();
    ~i3cFile();

    /* Image Status */
    bool isInitialized();
    bool isImageFullyWritten();

    /* Write & Rewrite Image */
    void resetImage();

    /* Accessors */
    bool setSideSize(int sideSize); //Can only be called once
    int getSideSize();
    int getNumOfLevel();

    int setMap(int level, unsigned char map);
    unsigned char getMap(int level, int index);

    int setPixel(unsigned char map, unsigned char red[8], unsigned char green[8], unsigned char blue[8]);
    unsigned char getRed(int index, int posInMap);
    unsigned char getGreen(int index, int posInMap);
    unsigned char getBlue(int index, int posInMap);

    /* Image Statistics */
    int countTotalCubes();
    int countTotalCubesAtLevel(int level);

private:
    void allocateChilds(int size);

private:
    /* Image */
    int m_iSideSize;
    int m_iNumOfLevels;
    unsigned char **m_ucMaps;
    unsigned char **m_ucRed;
    unsigned char **m_ucGreen;
    unsigned char **m_ucBlue;

    /* Statistics */
    int *m_iptrTotalCubesAtLevel;
    int *m_iptrNumberOfChilds;

    /* Map Settings */
    int m_iCurrentLevel;
    int m_iIndexLevel;
};

#endif // I3CFILESTRUCT_H
