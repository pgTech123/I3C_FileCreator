#include "image.h"

Image::Image()
{
}
Image::Image(const char *path)
{

}

Image::~Image()
{

}

void Image::setPath(const char* path)
{
    m_strPath = path;
}

bool Image::save()
{
    //TODO Write file
    return true;
}

bool Image::isPath()
{
    if(!m_strPath.empty()){
        return true;
    }
    return false;
}

int Image::open()
{
    return 0;
}

void Image::initializeEmpty()
{
    /* Create Minimum Viable File */
}

void Image::setSideSize(int sideSize)
{

}

int Image::convertLayerStackToImage(LayerStack *layerStack)
{

}

int Image::convertImageToLayerStack(LayerStack **ptrLayerStack)
{
    (*ptrLayerStack)->setSideSize(16);
}


