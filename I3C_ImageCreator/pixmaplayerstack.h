#ifndef PIXMAPLAYERSTACK_H
#define PIXMAPLAYERSTACK_H

#include "QWidget"
#include "layerstack.h"
#include "layerstackui.h"
#include "layer.h"
#include "pixel.h"


/*****************************************************************************
 * The purpose of this class is to contain a stack of pixmap layer and to provide
 * the ui that allow to edit them.
 * ***************************************************************************/

class PixmapLayerStack: public LayerStack
{
public:
    explicit PixmapLayerStack(QWidget *parent =  0);
    virtual ~PixmapLayerStack();

    layerStackUI* getUIQLabel();

    void setWorkspaceAsCurrentLayer();

private:
    void layerStackCreated(int sideSize);

private:
    layerStackUI *m_qlabelUI;

};

#endif // PIXMAPLAYERSTACK_H
