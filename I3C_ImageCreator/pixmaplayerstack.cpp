/*********************************************************
 * PixmapLayerStack.cpp
 * Author:      Pascal Gendron
 * Version:     0.1.0
 * *******************************************************/


#include "pixmaplayerstack.h"

PixmapLayerStack::PixmapLayerStack(QWidget *parent):
    LayerStack()
{
    m_qlabelUI = new layerStackUI(parent, this);
}

PixmapLayerStack::~PixmapLayerStack()
{
    delete m_qlabelUI;
}

layerStackUI* PixmapLayerStack::getUIQLabel()
{
    return m_qlabelUI;
}


void PixmapLayerStack::setWorkspaceAsCurrentLayer()
{
    m_qlabelUI->currentLayerChanged();
}

void PixmapLayerStack::layerStackCreated(int sideSize)
{
    m_qlabelUI->layerStackCreated(sideSize);
    setWorkspaceAsCurrentLayer();
}
