/*********************************************************
 * LayerStack.h
 * Author:      Pascal Gendron
 * Version:     0.1.0
 * *******************************************************/

#ifndef LAYERSTACK_H
#define LAYERSTACK_H

#include "gvbinaryfunctions.h"
#include "layer.h"
#include "pixel.h"

#define NO_ERRORS               0
#define SIDE_SIZE_ALREADY_SET   100
#define SIDE_SIZE_NOT_BASE_2    101

/********************************************************************************
 * The purpose of this class is to represent a layer stack, where a layer is a
 * 2D Image.
 * ******************************************************************************/

class LayerStack
{
public:
    LayerStack();
    ~LayerStack();

    /* Cube Properties */
    bool isSideSizeSet();
    int setSideSize(int sideSize);
    int getSideSize();

    /* Navigation */
    void nextLayer();
    void previousLayer();
    void goToLayer(int layer);
    int getCurrentLayer();

    /* Access Data */
    bool pixelWritten(int x, int y, int z, int w, int h, int d);
    Pixel getPixelAt(int x, int y, int z);
    Layer* getLayer(int index);

protected:
    /* Workspace functions */
    virtual void setWorkspaceAsCurrentLayer() {};

private:
    /*Empty method to be overiden if necessary*/
    virtual void layerStackCreated(int sideSize) {};

    Pixel emptyPixel();

private:
    Layer *m_LayerArray;
    int m_iCurrentLayer;

    bool m_bSideSizeSet;
    int m_iSideSize;
};

#endif // LAYERSTACK_H
