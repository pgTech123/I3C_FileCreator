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
    m_LayerArray = NULL;
    m_bSideSizeSet = false;
    m_bMouseButtonDwn = false;
    m_iCurrentLayer = 0;
}

LayerStack::~LayerStack()
{
    if(m_frame != NULL){
        delete m_frame;
    }
    if(m_LayerArray != NULL){
        delete[] m_LayerArray;
    }
}

void LayerStack::resizeEvent(QResizeEvent*)
{
    /* x 0.8 to be able to size down */
    int newHeight = this->height()*0.8;
    int newWidth = this->width()*0.8;

    /* As we draw on this image, we dont want to unzoom too much */
    if(newWidth >= m_iSideSize && newHeight >= m_iSideSize){
        (*m_frame) = m_frame->scaled(newWidth, newHeight, Qt::KeepAspectRatio);
        QLabel::setPixmap(*m_frame);

        m_dPixelToPixelFactor = (double)m_iSideSize / (double)m_frame->width();
    }
}

void LayerStack::mousePressEvent(QMouseEvent *event)
{
    m_bMouseButtonDwn = true;

    /* Draw on Layer */
    m_LayerArray[m_iCurrentLayer].writePixel(event->x()*m_dPixelToPixelFactor,
                                             event->y()*m_dPixelToPixelFactor,
                                             m_iRed,
                                             m_iGreen,
                                             m_iBlue);
    updateDisplayedLayer(event->x()*m_dPixelToPixelFactor,
                         event->y()*m_dPixelToPixelFactor,
                         m_iRed,
                         m_iGreen,
                         m_iBlue);
}

void LayerStack::mouseReleaseEvent(QMouseEvent*)
{
    m_bMouseButtonDwn = false;
}

void LayerStack::mouseMoveEvent(QMouseEvent *event)
{
    if(m_bMouseButtonDwn){
        /* Draw on Layer */
        m_LayerArray[m_iCurrentLayer].writePixel(event->x()*m_dPixelToPixelFactor,
                                                 event->y()*m_dPixelToPixelFactor,
                                                 m_iRed,
                                                 m_iGreen,
                                                 m_iBlue);
        updateDisplayedLayer(event->x()*m_dPixelToPixelFactor,
                             event->y()*m_dPixelToPixelFactor,
                             m_iRed,
                             m_iGreen,
                             m_iBlue);
    }
}

void LayerStack::setSideSize(int sideSize)
{
    if(!m_bSideSizeSet){
        /* Set side size */
        m_bSideSizeSet = true;
        m_iSideSize = sideSize;

        /* Create Layer Stack */
        m_LayerArray = new Layer[m_iSideSize];
        for(int i = 0; i < m_iSideSize; i++){
            m_LayerArray[i].setSideSize(m_iSideSize);
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
    if(m_iCurrentLayer < m_iSideSize-1){
            m_iCurrentLayer++;
            setWorkspaceAsCurrentLayer();
    }
}

void LayerStack::previousLayer()
{
    if(m_iCurrentLayer > 0){
        m_iCurrentLayer--;
        setWorkspaceAsCurrentLayer();
    }
}

void LayerStack::goToLayer(int layer)
{
    if(layer >= 0 && layer < m_iSideSize){
            m_iCurrentLayer = layer;
            setWorkspaceAsCurrentLayer();
    }
}

void LayerStack::setWorkspaceAsCurrentLayer()
{
    *(m_frame) = *(m_LayerArray[m_iCurrentLayer].getPixmapPtr());

    /* If not last layer, add in transparency */
    if(m_iCurrentLayer < (m_iSideSize-1)){
        addPixmapInTransparency(m_LayerArray[m_iCurrentLayer+1].getPixmapPtr());
    }

    /* If not first layer, add in transparency */
    if(m_iCurrentLayer > 0){
        addPixmapInTransparency(m_LayerArray[m_iCurrentLayer-1].getPixmapPtr());
    }

    /* Resize and display */
    resizeEvent(NULL);
}


void LayerStack::setActiveColor(int red, int green, int blue)
{
    m_iRed = red;
    m_iGreen = green;
    m_iBlue = blue;
}

void LayerStack::updateDisplayedLayer(int x, int y, int r, int g, int b)
{
    //TODO

    QLabel::setPixmap(*m_frame);
}

void LayerStack::addPixmapInTransparency(QPixmap *layer)
{
    //TODO
}
