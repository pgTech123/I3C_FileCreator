#include "i3cFileStruct.h"

i3cFile::i3cFile()
{
    m_iSideSize = 0;
    m_iNumOfLevels = 0;
    m_iIndexLevel = 0;
    m_iCurrentLevel = 0;
    m_iptrTotalCubesAtLevel = NULL;
    m_ucMaps = NULL;
}

i3cFile::~i3cFile()
{
    if(m_iptrTotalCubesAtLevel != NULL){
        delete[] m_iptrTotalCubesAtLevel;
    }
    //DELETE ALL CHILD
    //TODO
}

bool i3cFile::isInitialized()
{
    if(m_iSideSize != 0){
        return true;
    }
    return false;
}

bool i3cFile::setSideSize(int sideSize)
{
    if(isBase2(sideSize) && sideSize > 0 && m_iSideSize == 0){
        /* Image Data */
        m_iSideSize = sideSize;
        m_iNumOfLevels = firstHighBit(sideSize);

        /* Allocation */
        m_iptrTotalCubesAtLevel = new int[m_iNumOfLevels];  /*Count variable*/
        m_ucMaps = new unsigned char* [m_iNumOfLevels];     /*Allocate pointers to map array*/

        /* Initialisation */
        for(int i = 0; i < m_iNumOfLevels-1; i++){
            m_iptrTotalCubesAtLevel[i] = 0;
        }
        m_iptrTotalCubesAtLevel[m_iNumOfLevels-1] = 1; /* Always 1 */

        /**/
        m_iCurrentLevel = m_iNumOfLevels;
        allocateChilds(1);
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

int i3cFile::setMap(int level, unsigned char map)
{
    if(level >= 2 && level <= m_iNumOfLevels && m_iCurrentLevel == level){
        /*Update Count */
        m_iptrTotalCubesAtLevel[level-2] += numberHighBits(map);    /* Level-- is the index in the array, and we want its child so -2 */

        /* Set Map */
        m_ucMaps[level-1][m_iIndexLevel] = map;

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

unsigned char i3cFile::getMap(int level, int index)
{
    return m_ucMaps[level-1][index];
}

int i3cFile::setPixel(unsigned char map, unsigned char red[8], unsigned char green[8], unsigned char blue[8])
{

}

/*TO UNCOMMENT WHEN ALLOCATED FUNCTION WRITTEN*/
unsigned char i3cFile::getRed(int index, int posInMap)
{
    //return m_ucRed[index][posInMap];
}

unsigned char i3cFile::getGreen(int index, int posInMap)
{
    //return m_ucGreen[index][posInMap];
}

unsigned char i3cFile::getBlue(int index, int posInMap)
{
    //return m_ucBlue[index][posInMap];
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

void i3cFile::allocateChilds(int size)
{
    m_ucMaps[m_iCurrentLevel-1] = new unsigned char[size];
}
