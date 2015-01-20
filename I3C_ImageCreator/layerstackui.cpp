#include "layerstackui.h"

layerStackUI::layerStackUI(QWidget *parent, LayerStack* layerStack) :
    QLabel(parent)
{
    m_LayerStack = layerStack;
    m_frame = NULL;
    m_Painter = new QPainter();
    m_bMouseButtonDwn = false;

    /* Init Color to Black */
    m_iRed = 0;
    m_iGreen = 0;
    m_iBlue = 0;
}

layerStackUI::~layerStackUI()
{
    if(m_frame != NULL){
        delete m_frame;
    }
    delete m_Painter;
}

void layerStackUI::resizeEvent(QResizeEvent*)
{
    if(m_LayerStack->isSideSizeSet()){
        /* x 0.8 to be able to size down*/
        int newHeight = this->height()*0.8;
        int newWidth = this->width()*0.8;

        /* As we draw on this image, we dont want to unzoom too much*/
        if(newWidth >= m_LayerStack->getSideSize() && newHeight >= m_LayerStack->getSideSize()){
            m_PixmapScaled = m_frame->scaled(newWidth, newHeight, Qt::KeepAspectRatio);

            /* Adjust scale factor between real size of 1 pixel and the size of 1 pix displayed*/
            m_dPixelToPixelFactor = (double)m_LayerStack->getSideSize() / (double)m_PixmapScaled.width();
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

void layerStackUI::mousePressEvent(QMouseEvent *event)
{
    m_bMouseButtonDwn = true;
    //TODO: Condition is in drawable widget
    /* Draw on Layer*/
    m_LayerStack->getLayer(m_LayerStack->getCurrentLayer())->writePixel(event->x()*m_dPixelToPixelFactor,
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

void layerStackUI::mouseReleaseEvent(QMouseEvent*)
{
    m_bMouseButtonDwn = false;
}

void layerStackUI::mouseMoveEvent(QMouseEvent *event)
{
    if(m_bMouseButtonDwn){
        //TODO: Condition is in drawable widget
        /* Draw on Layer */
        m_LayerStack->getLayer(m_LayerStack->getCurrentLayer())->writePixel(event->x()*m_dPixelToPixelFactor,
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

void layerStackUI::layerStackCreated(int sideSize)
{
    /* Create Workspace Pixmap */
    if(m_frame != NULL){
        delete m_frame;
    }
    m_frame = new QPixmap(sideSize,sideSize);
    emit initLayerStackDisplay();
}

void layerStackUI::currentLayerChanged()
{
     putLayerInPixmap(m_LayerStack->getLayer(m_LayerStack->getCurrentLayer()), m_frame);

     /* Add adjacent layers in transparency */
     QPixmap *tmp = new QPixmap(m_LayerStack->getSideSize(), m_LayerStack->getSideSize());
     if(m_LayerStack->getCurrentLayer() < (m_LayerStack->getSideSize()-1)){
         putLayerInPixmap(m_LayerStack->getLayer(m_LayerStack->getCurrentLayer() + 1), tmp);
         addPixmapInTransparency(tmp);
     }
     if(m_LayerStack->getCurrentLayer() > 0){
         putLayerInPixmap(m_LayerStack->getLayer(m_LayerStack->getCurrentLayer() - 1), tmp);
         addPixmapInTransparency(tmp);
     }
     delete tmp;

     /* Resize and display */
     resizeEvent(NULL);

}

void layerStackUI::setActiveColor(int red, int green, int blue)
{
    m_iRed = red;
    m_iGreen = green;
    m_iBlue = blue;
}


void layerStackUI::updateDisplayedLayer(int x, int y, int r, int g, int b)
{
    m_Painter->begin(m_frame);
    m_Painter->setPen(QPen(QBrush(QColor(r, g, b)),1));
    m_Painter->drawPoint(x,y);
    m_Painter->end();
    resizeEvent(NULL);
}

void layerStackUI::addPixmapInTransparency(QPixmap *layer)
{
    m_Painter->begin(m_frame);
    m_Painter->setOpacity(0.3);
    m_Painter->drawPixmap(0,0, *layer);
    m_Painter->end();
}

 void layerStackUI::putLayerInPixmap(Layer *layer, QPixmap *pixmap)
 {
     int sideSize = layer->getSideSize();
     Pixel bufPixel;

     m_Painter->begin(pixmap);

     for(int x = 0; x < sideSize; x++){
         for(int y = 0; y < sideSize; y++){
             bufPixel = layer->getPixel(x, y);
             m_Painter->setPen(QPen(QBrush(QColor(bufPixel.red,
                                                  bufPixel.green,
                                                  bufPixel.blue)),1));
             m_Painter->drawPoint(x,y);
         }
     }
     m_Painter->end();
 }
