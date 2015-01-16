/*********************************************************
 * PixmapLayerStack.cpp
 * Author:      Pascal Gendron
 * Version:     0.1.0
 * *******************************************************/


#include "pixmaplayerstack.h"

PixmapLayerStack::PixmapLayerStack(QWidget *parent):
    LayerStack(), QLabel(parent)
{
    m_frame = NULL;
    m_Painter = new QPainter();
    m_bMouseButtonDwn = false;

    /* Init Color to Black */
    m_iRed = 0;
    m_iGreen = 0;
    m_iBlue = 0;
}

PixmapLayerStack::~PixmapLayerStack()
{
    if(m_frame != NULL){
        delete m_frame;
    }
    delete m_Painter;
}

void PixmapLayerStack::resizeEvent(QResizeEvent*)
{
    if(isSideSizeSet()){
        /* x 0.8 to be able to size down */
        int newHeight = this->height()*0.8;
        int newWidth = this->width()*0.8;

        /* As we draw on this image, we dont want to unzoom too much */
        if(newWidth >= getSideSize() && newHeight >= getSideSize()){
            m_PixmapScaled = m_frame->scaled(newWidth, newHeight, Qt::KeepAspectRatio);

            /* Adjust scale factor between real size of 1 pixel and the size of 1 pix displayed */
            m_dPixelToPixelFactor = (double)getSideSize() / (double)m_PixmapScaled.width();
            m_iOffsetCorrection = m_PixmapScaled.width() * 0.125 * m_dPixelToPixelFactor;
        }
        else{
            m_PixmapScaled = m_frame->scaled(m_PixmapScaled.width(), m_PixmapScaled.height(), Qt::KeepAspectRatio);
            m_dPixelToPixelFactor = 1;
            m_iOffsetCorrection = 0;
        }
        QLabel::setPixmap(m_PixmapScaled);
    }
}

void PixmapLayerStack::mousePressEvent(QMouseEvent *event)
{
    m_bMouseButtonDwn = true;

    /* Draw on Layer */
    getLayer(getCurrentLayer()).writePixel(event->x()*m_dPixelToPixelFactor,
                                             event->y()*m_dPixelToPixelFactor-m_iOffsetCorrection,
                                             m_iRed,
                                             m_iGreen,
                                             m_iBlue);
    updateDisplayedLayer(event->x()*m_dPixelToPixelFactor,
                         event->y()*m_dPixelToPixelFactor-m_iOffsetCorrection,
                         m_iRed,
                         m_iGreen,
                         m_iBlue);
}

void PixmapLayerStack::mouseReleaseEvent(QMouseEvent*)
{
    m_bMouseButtonDwn = false;
}

void PixmapLayerStack::mouseMoveEvent(QMouseEvent *event)
{
    if(m_bMouseButtonDwn){
        /* Draw on Layer */
        getLayer(getCurrentLayer()).writePixel(event->x()*m_dPixelToPixelFactor,
                                                 event->y()*m_dPixelToPixelFactor-m_iOffsetCorrection,
                                                 m_iRed,
                                                 m_iGreen,
                                                 m_iBlue);
        updateDisplayedLayer(event->x()*m_dPixelToPixelFactor,
                             event->y()*m_dPixelToPixelFactor-m_iOffsetCorrection,
                             m_iRed,
                             m_iGreen,
                             m_iBlue);
    }
}

void PixmapLayerStack::setWorkspaceAsCurrentLayer()
{
    *(m_frame) = *(getLayer(getCurrentLayer()).getPixmapPtr());

    /* If not last layer, add in transparency */
    if(getCurrentLayer() < (getSideSize()-1)){
        addPixmapInTransparency(getLayer(getCurrentLayer() + 1).getPixmapPtr());
    }

    /* If not first layer, add in transparency */
    if(getCurrentLayer() > 0){
        addPixmapInTransparency(getLayer(getCurrentLayer() - 1).getPixmapPtr());
    }

    /* Resize and display */
    resizeEvent(NULL);
}

void PixmapLayerStack::setActiveColor(int red, int green, int blue)
{
    m_iRed = red;
    m_iGreen = green;
    m_iBlue = blue;
}

void PixmapLayerStack::layerStackCreated(int sideSize)
{
    /* Create Workspace Pixmap */
    if(m_frame != NULL){
        delete m_frame;
    }
    m_frame = new QPixmap(sideSize,sideSize);
    setWorkspaceAsCurrentLayer();

    /* Display it in MainWindow */
    emit initLayerStackDisplay();
}

void PixmapLayerStack::updateDisplayedLayer(int x, int y, int r, int g, int b)
{
    m_Painter->begin(m_frame);
    m_Painter->setPen(QPen(QBrush(QColor(r, g, b)),1));
    m_Painter->drawPoint(x,y);
    m_Painter->end();
    resizeEvent(NULL);
}

void PixmapLayerStack::addPixmapInTransparency(QPixmap *layer)
{
    m_Painter->begin(m_frame);
    m_Painter->setOpacity(0.3);
    m_Painter->drawPixmap(0,0, *layer);
    m_Painter->end();
}
