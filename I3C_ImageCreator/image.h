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
#include "i3cFileStruct.h"
#include "layerstack.h"
#include "gvbinaryfunctions.h"

#define NO_ERRORS               0
#define ERROR_SIZE              1
#define NO_IMAGE                2
#define IMAGE_ALREADY_LOADED    3
#define UNABLE_TO_OPEN_FILE     4

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

    /* Conversion */
    int convertLayerStackToImage(LayerStack *layerStack);
    int convertImageToLayerStack(LayerStack **ptrLayerStack);

private:
    void writeExtension();
    bool writeI3CFile();

    void convertReferencesLS2Img(LayerStack *layerStack, int level);
    void convertPixelsLS2Img(LayerStack *layerStack);

    unsigned char getMapFromLayerStack(LayerStack *layerStack, int x, int y, int z, int sideSize);

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
