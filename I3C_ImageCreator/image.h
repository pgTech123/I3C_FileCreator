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

#define NO_ERRORS   0
#define ERROR_SIZE  1
#define NO_IMAGE    2

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
    Image(const char* path);
    ~Image();

    /* File Functions */
    bool save();
    bool save(const char* path);
    int open(const char* path);

    /* Image Modif */
    bool setSideSize(int sideSize);
    void initializeEmpty();

    /* Conversion */
    int convertLayerStackToImage(LayerStack *layerStack);
    int convertImageToLayerStack(LayerStack **ptrLayerStack);

private:
    void initialization();
    void writeExtension();
    bool writeI3CFile();

private:
    /* File Variables */
    string m_strFilePath;
    i3cFile m_i3cFile;

    /* Image Variable */
    bool m_bNoImageContained;
};

#endif // IMAGE_H
