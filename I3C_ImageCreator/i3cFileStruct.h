#ifndef I3CFILESTRUCT_H
#define I3CFILESTRUCT_H

#include <iostream>
using namespace std;

#include "gvbinaryfunctions.h"


struct i3cPixelCube{
    unsigned char map;
    unsigned char red[8];
    unsigned char green[8];
    unsigned char blue[8];
};

struct i3cReferenceCube{
    unsigned char map;
    i3cReferenceCube *childs;
    i3cPixelCube *pixels;
};

class i3cFile{
public:
    i3cFile();
    ~i3cFile();

    /* Image Status */
    bool isInitialized();

    /* Accessors */
    bool setSideSize(int sideSize);
    int getSideSize();
    int getNumOfLevel();

    void setMap(int level, int index, unsigned char map);
    unsigned char getMap(int level, int index);

    /* Image Statistics */
    int countTotalCubes();
    int countTotalCubesAtLevel(int level);

private:
    /* Image */
    int m_iSideSize;
    int m_iNumOfLevels;
    i3cReferenceCube content;

    /* Statistics */
    int *m_iptrTotalCubesAtLevel;
};

#endif // I3CFILESTRUCT_H
