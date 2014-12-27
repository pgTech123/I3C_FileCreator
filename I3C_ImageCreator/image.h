#ifndef IMAGE_H
#define IMAGE_H

#include <iostream>
#include <string>
#include "layerstack.h"
using namespace std;

#define NO_ERRORS   0
#define ERROR_SIZE  1

class Image
{
public:
    Image();
    Image(const char* path);
    ~Image();

    /* File Functions */
    void setPath(const char* path);
    bool save();
    bool isPath();

    /* Image Modif */
    void setSideSize(int sideSize);
    void initializeEmpty();

    /* Conversion */
    int convertLayerStackToImage(LayerStack *layerStack);
    int convertImageToLayerStack(LayerStack **ptrLayerStack);

private:
    /* File Variables */
    string m_strPath;
};

#endif // IMAGE_H
