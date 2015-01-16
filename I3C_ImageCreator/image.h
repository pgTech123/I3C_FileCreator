/*********************************************************
 * Image.h
 * Author:      Pascal Gendron
 * Version:     0.0.1
 * *******************************************************/

#ifndef IMAGE_H
#define IMAGE_H

#include <iostream>
#include <fstream>
#include <string>
#include <math.h>
#include "i3cFileStruct.h"
#include "pixmaplayerstack.h"
#include "gvbinaryfunctions.h"
#include "pixel.h"
#include "constants.h"

/*********************************************************************
 * The purpose of this class is to provide the algorithm to convert
 * Images to a Layer Stack and the opposite. The class also makes the
 * interactions with the file(open&save).
 * *******************************************************************/

//TODO: Make sure of position of map

using namespace std;
class Image
{
public:
    Image();
    ~Image();

    /* File Functions */
    bool save();
    bool save(const char* path);
    int open(const char* path);

    /* Image Modif */
    bool setSideSize(int sideSize);

    /* Utils */
    ChildCorners generateChildCorners(CubeMap map, int mapSize);

    /* Conversion */
    int convertLayerStackToImage(PixmapLayerStack *layerStack);
    int convertImageToLayerStack(PixmapLayerStack **ptrLayerStack);

private:
    void writeExtension();
    bool writeI3CFile();

    int convertReferencesLS2Img(PixmapLayerStack *layerStack, int level);
    int convertPixelsLS2Img(PixmapLayerStack *layerStack);

    int setChildMapViaParentMap(PixmapLayerStack *layerStack, int level);
    int setChildMap(PixmapLayerStack *layerStack, int x, int y, int z, int level, int sideSize);
    CubeMap getMapFromLayerStack(PixmapLayerStack *layerStack, int x, int y, int z, int sideSize);
    int setCubePixels(PixmapLayerStack *layerStack, int x, int y, int z);

    void readHeader(ifstream *file);
    void readPixels(ifstream *file);
    void readReferences(ifstream *file);

    void writeHeader(ofstream* file);
    void writePixels(ofstream* file);
    void writeReferences(ofstream* file);

private:
    /* File Variables */
    string m_strFilePath;
    i3cFile m_i3cFile;
};

#endif // IMAGE_H
