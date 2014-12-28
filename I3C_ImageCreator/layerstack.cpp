/*********************************************************
 * LayerStack.cpp
 * Author:      Pascal Gendron
 * Version:     0.0.1
 * *******************************************************/


#include "layerstack.h"

LayerStack::LayerStack(QWidget *parent):
    QLabel(parent)
{
    m_frame = NULL;
    m_layerArray = NULL;
    m_bSideSizeSet = false;
    m_iCurrentLayer = 1;
}

LayerStack::~LayerStack()
{
    if(m_frame != NULL){
        delete m_frame;
    }
    if(m_layerArray != NULL){
        delete[] m_layerArray;
    }
}

void LayerStack::resizeEvent(QResizeEvent *event)
{
    /* x 0.8 to be able to size down */
    int newHeight = event->size().height()*0.8;
    int newWidth = event->size().width()*0.8;

    /* As we draw on this image, we dont want to unzoom too much */
    if(newWidth >= m_iSideSize && newHeight >= m_iSideSize){
        (*m_frame) = m_frame->scaled(newWidth, newHeight, Qt::KeepAspectRatio);
        QLabel::setPixmap(*m_frame);
    }
}

void LayerStack::setSideSize(int sideSize)
{
    if(!m_bSideSizeSet){
        /* Set side size */
        m_bSideSizeSet = true;
        m_iSideSize = sideSize;

        /* Create Layer Stack */
        m_layerArray = new Layer[m_iSideSize];
        for(int i = 0; i < m_iSideSize; i++)
        {
            m_layerArray[i].setSideSize(m_iSideSize);
        }

        /* Create Workspace Pixmap */
        m_frame = new QPixmap(sideSize,sideSize);
        m_frame->fill(Qt::black);

        /* Set Initial Frame */
        QLabel::setPixmap(*m_frame);

        /* Display it in MainWindow */
        emit initLayerStackDisplay();
    }
}

int LayerStack::getCurrentLayer()
{
    return m_iCurrentLayer;
}

int LayerStack::getSideSize()
{
    return m_iSideSize;
}

void LayerStack::nextLayer()
{
    if(m_iCurrentLayer < m_iSideSize){
            m_iCurrentLayer++;
            setWorkspaceAsCurrentLayer();
    }
}

void LayerStack::previousLayer()
{
    if(m_iCurrentLayer > 1){
        m_iCurrentLayer--;
        setWorkspaceAsCurrentLayer();
    }
}

void LayerStack::goToLayer(int layer)
{
    if(layer > 0 && layer <= m_iSideSize){
            m_iCurrentLayer = layer;
            setWorkspaceAsCurrentLayer();
    }
}

void LayerStack::setWorkspaceAsCurrentLayer()
{
    //TODO
}


void LayerStack::setActiveColor(int red, int green, int blue)
{

}
