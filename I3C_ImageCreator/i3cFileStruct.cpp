#include "i3cFileStruct.h"

i3cFile::i3cFile()
{
    m_iSideSize = 0;
    m_iNumOfLevels = 0;
    content.map = 0;
    m_iptrTotalCubesAtLevel = NULL;
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
    if(isBase2(sideSize) && sideSize > 0){
        m_iSideSize = sideSize;
        m_iNumOfLevels = firstHighBit(sideSize);
        m_iptrTotalCubesAtLevel = new int[m_iNumOfLevels];

        for(int i = 0; i < m_iNumOfLevels; i++){
            m_iptrTotalCubesAtLevel[i] = 0;
        }
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

void i3cFile::setMap(int level, int index, unsigned char map)
{
    //TODO
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
