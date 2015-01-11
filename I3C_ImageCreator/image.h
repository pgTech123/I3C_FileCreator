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
#include "layerstack.h"
#include "gvbinaryfunctions.h"
#include "pixel.h"
#include "constants.h"

/*********************************************************************
 * The purpose of this class is to provide the algorithm to convert
 * Images to a Layer Stack and the opposite. The class also makes the
 * interactions with the file(open&save).
 * *******************************************************************/

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
    int convertLayerStackToImage(LayerStack *layerStack);
    int convertImageToLayerStack(LayerStack **ptrLayerStack);

private:
    void writeExtension();
    bool writeI3CFile();

    int convertReferencesLS2Img(LayerStack *layerStack, int level);
    int convertPixelsLS2Img(LayerStack *layerStack);

    int setChildMapViaParentMap(LayerStack *layerStack, int level);
    int setChildMap(LayerStack *layerStack, int x, int y, int z, int level, int sideSize);
    CubeMap getMapFromLayerStack(LayerStack *layerStack, int x, int y, int z, int sideSize);
    int setCubePixels(LayerStack *layerStack, int x, int y, int z);

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
