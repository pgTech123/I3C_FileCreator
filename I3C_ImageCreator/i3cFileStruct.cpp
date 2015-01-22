#include "i3cFileStruct.h"

i3cFile::i3cFile()
{
    initMembers();
}

i3cFile::~i3cFile()
{
   deleteImageData();
}

bool i3cFile::isInitialized()
{
    if(m_iSideSize != 0){
        return true;
    }
    return false;
}

void i3cFile::resetImage()
{
    int tmpSideSize = m_iSideSize;
    deleteImageData();
    setSideSize(tmpSideSize);
}

bool i3cFile::setSideSize(int sideSize)
{
    if(isBase2(sideSize) && sideSize > 0 && m_iSideSize == 0){
        /* Image Data */
        m_iSideSize = sideSize;
        m_iNumOfLevels = firstHighBit(sideSize);


        /* Allocation */
        m_iptrTotalCubesAtLevel = new int[m_iNumOfLevels];  /*Count variable*/
        m_MapsAndPos = new CubeMap* [m_iNumOfLevels];       /*Allocate pointers to map array*/
        m_bMapLocked = new bool [m_iNumOfLevels];


        /* Initialisation */
        for(int i = 0; i < m_iNumOfLevels-1; i++){
            /* Image Data */
            m_MapsAndPos[i] = NULL;
            /* Statistics */
            m_iptrTotalCubesAtLevel[i] = 0;
            /* Safety Lock */
            m_bMapLocked[i] = true;
        }
        /* Image Data */
        m_MapsAndPos[m_iNumOfLevels-1] = NULL;
        /* Statistics */
        m_iptrTotalCubesAtLevel[m_iNumOfLevels-1] = 1; /* Always 1 */
        /* Safety Lock */
        m_bMapLocked[m_iNumOfLevels-1] = false;


        /* Allocate first cube */
        m_iCurrentLevel = m_iNumOfLevels;
        allocateChilds(1);
        return true;
    }
    return false;
}

int i3cFile::getSideSize()
{
    return m_iSideSize;
}

int i3cFile::getNumOfLevel()
{
    return m_iNumOfLevels;
}

int i3cFile::setMapAndPos(int level, CubeMap mapAndPos)
{
    if(level >= 2 && level <= m_iNumOfLevels && m_iCurrentLevel == level){
        /*Update Count */
        m_iptrTotalCubesAtLevel[level-2] += numberHighBits(mapAndPos.map);    /* Level-- is the index in the array, and we want its child so -2 */

        /* Set Map */
        m_MapsAndPos[level-1][m_iIndexLevel] = mapAndPos;

        /* Update Index */
        m_iIndexLevel ++;

        /* Level Manager */
        if(m_iptrTotalCubesAtLevel[level-1] == m_iIndexLevel){
            m_iIndexLevel = 0;
            m_iCurrentLevel --;
            allocateChilds(m_iptrTotalCubesAtLevel[level-2]);
        }
        return NO_ERRORS;
    }
    return LEVEL_NOT_ACCESSIBLE;
}

int i3cFile::setMapAndPos(int level, int index, CubeMap mapAndPos)
{
    if(level <= m_iNumOfLevels && level > 0){
       if(index < m_iptrTotalCubesAtLevel[level-1] && index >= 0){
           m_MapsAndPos[level-1][index] = mapAndPos;
           return NO_ERRORS;
       }
       return INDEX_OUT_OF_RANGE;
    }
    return LEVEL_OUT_OF_RANGE;
}

CubeMap i3cFile::getMapAndPos(int level, int index)
{
    if(!m_bMapLocked[level-1]){
        return m_MapsAndPos[level-1][index];
    }
    cout << "ERROR I3CFile: locked" << endl;
    CubeMap mapNull;
    mapNull.map = 0;
    mapNull.x = 0;
    mapNull.y = 0;
    mapNull.z = 0;
    return mapNull;
}

int i3cFile::setPixel(CubeMap map, Pixel pixels[8])
{
    if(m_iptrTotalCubesAtLevel[0] > m_iIndexLevel){
        /* Allocate Pixels */
        m_ucRed[m_iIndexLevel] = new unsigned char [8];
        m_ucGreen[m_iIndexLevel] = new unsigned char [8];
        m_ucBlue[m_iIndexLevel] = new unsigned char [8];

        /* Set Data */
        m_MapsAndPos[0][m_iIndexLevel] = map;
        for(int i = 0; i < 8; i++){
            m_ucRed[m_iIndexLevel][i] = pixels[i].red;
            m_ucGreen[m_iIndexLevel][i] = pixels[i].green;
            m_ucBlue[m_iIndexLevel][i] = pixels[i].blue;
        }

        /* Update Index */
        m_iIndexLevel ++;

        return NO_ERRORS;
    }
    return LEVEL_LOCKED;
}

unsigned char i3cFile::getRed(int index, int posInMap)
{
    if(posInMap >= 8 || m_ucRed[index] == NULL){
        return 0;
    }
    else{
        return m_ucRed[index][posInMap];
    }
}

unsigned char i3cFile::getGreen(int index, int posInMap)
{
    if(posInMap >= 8 || m_ucGreen[index] == NULL){
        return 0;
    }
    else{
        return m_ucGreen[index][posInMap];
    }
}

unsigned char i3cFile::getBlue(int index, int posInMap)
{
    if(posInMap >= 8 || m_ucBlue[index] == NULL){
        return 0;
    }
    else{
        return m_ucBlue[index][posInMap];
    }
}

int i3cFile::countTotalCubes()
{
    int count = 0;
    for(int i = 1; i <= m_iNumOfLevels; i++){
        count += countTotalCubesAtLevel(i);
    }
    return count;
}

int i3cFile::countTotalCubesAtLevel(int level)
{
    /* To respect convention */
    level--;

    if(level < m_iNumOfLevels && level >= 0){
        return m_iptrTotalCubesAtLevel[level];
    }
    else{
        return 0;
    }
}

void i3cFile::setTotalCubesAtLevel(int level, int numberOfCubes)
{
    if(level < 1 || level > m_iNumOfLevels){
        return;
    }

    m_iptrTotalCubesAtLevel[level - 1] = numberOfCubes;
    m_iCurrentLevel = level;
    allocateChilds(numberOfCubes);
}

void i3cFile::initMembers()
{
    /*Init image data*/
    m_iSideSize = 0;
    m_iNumOfLevels = 0;
    m_MapsAndPos = NULL;
    m_ucRed = NULL;
    m_ucGreen = NULL;
    m_ucBlue = NULL;

    /* Init Safety Lock */
    m_bMapLocked = NULL;

    /* Init Statistics */
    m_iptrTotalCubesAtLevel = NULL;

    /* Init cursor */
    m_iIndexLevel = 0;
    m_iCurrentLevel = 0;
}

void i3cFile::allocateChilds(int size)
{
    if(m_MapsAndPos[m_iCurrentLevel-1] == NULL){
        m_MapsAndPos[m_iCurrentLevel-1] = new CubeMap[size];
    }
    if(m_bMapLocked[m_iCurrentLevel-1]){
        m_bMapLocked[m_iCurrentLevel-1] = false;
    }

    if(m_iCurrentLevel == 1 && m_ucRed == NULL &&
            m_ucGreen == NULL && m_ucBlue == NULL){
        /* Allocate space for pixels */
        m_ucRed = new unsigned char* [size];
        m_ucGreen = new unsigned char* [size];
        m_ucBlue = new unsigned char* [size];
        for(int i = 0; i < size; i++){
            m_ucRed[i] = NULL;
            m_ucGreen[i] = NULL;
            m_ucBlue[i] = NULL;
        }
    }
}

void i3cFile::deleteImageData()
{
    /* Delete Image Data Arrays(2D) */
    if(m_MapsAndPos != NULL){
         for(int i = 0; i < m_iNumOfLevels; i++){
             if(m_MapsAndPos[i] != NULL){
                 delete[] m_MapsAndPos[i];
             }
         }
         delete[] m_MapsAndPos;
     }
     delete2DUCArray(m_ucRed, 8);
     delete2DUCArray(m_ucGreen, 8);
     delete2DUCArray(m_ucBlue, 8);

     /* Delete Safety Lock Array */
     if(m_bMapLocked != NULL){
         delete[] m_bMapLocked;
     }

     /* Delete Statistics Array */
     if(m_iptrTotalCubesAtLevel != NULL){
         delete[] m_iptrTotalCubesAtLevel;
     }
     initMembers();
}

void i3cFile::delete2DUCArray(unsigned char **array, int arraySize2ndD)
{
    if(array != NULL){
        for(int i = 0; i < arraySize2ndD; i++){
            if(array[i] != NULL){
                delete[] array[i];
            }
        }
        delete[] array;
    }
}
