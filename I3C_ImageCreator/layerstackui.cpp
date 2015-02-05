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
    m_BrushType = Pen;
    m_iBrushSize = 1;

    m_iLastX = -1;
    m_iLastY = -1;
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
        /* x 0.8 to be able to size down */
        int newHeight = this->height()*0.8;
        int newWidth = this->width()*0.8;

        /* As we draw on this image, we dont want to unzoom too much */
        if(newWidth >= m_LayerStack->getSideSize() && newHeight >= m_LayerStack->getSideSize()){
            m_PixmapScaled = m_frame->scaled(newWidth, newHeight, Qt::KeepAspectRatio);

            /* Adjust scale factor between real size of 1 pixel and the size of 1 pix displayed */
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

    if(m_BrushType == Eraser){
        erase(event->x()*m_dPixelToPixelFactor,
              event->y()*m_dPixelToPixelFactor-m_iOffsetCorrection);
    }
    else if(m_BrushType ==Pen){
        draw(event->x()*m_dPixelToPixelFactor,
             event->y()*m_dPixelToPixelFactor-m_iOffsetCorrection);
    }
}

void layerStackUI::mouseReleaseEvent(QMouseEvent*)
{
    m_bMouseButtonDwn = false;
    m_iLastX = -1;
    m_iLastY = -1;

    /* Manage History */
    LayerStackUIHistory *historyElement = new LayerStackUIHistory();
    historyElement->setDelta(historyData);
    emit modificationMade(historyElement);
    historyData.clear();
}

void layerStackUI::mouseMoveEvent(QMouseEvent *event)
{
    if(m_bMouseButtonDwn){
        //TODO: Condition is in drawable widget

        if(m_BrushType == Eraser){
            erase(event->x()*m_dPixelToPixelFactor,
                  event->y()*m_dPixelToPixelFactor-m_iOffsetCorrection);
        }
        else if(m_BrushType ==Pen){
            draw(event->x()*m_dPixelToPixelFactor,
                 event->y()*m_dPixelToPixelFactor-m_iOffsetCorrection);
        }
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

void layerStackUI::setEraserSelected(int eraserSize)
{
    m_iBrushSize = eraserSize;
    m_BrushType = Eraser;
}

void layerStackUI::setPenSelected(int penSize)
{
    m_iBrushSize = penSize;
    m_BrushType = Pen;
}

void layerStackUI::draw(int x, int y)
{
    if(x != m_iLastX || y != m_iLastY){
        /****************************************************
         * Draw on Layer
         * Warning: history must be called before write pixel
         ****************************************************/
        int iHalfBrushSize = m_iBrushSize/2;
        int xMinHalfBrush = x - iHalfBrushSize;
        int yMinHalfBrush = y - iHalfBrushSize;

        for(int i = 0; i < m_iBrushSize; i++){
            for(int j = 0; j < m_iBrushSize; j++){
                int currentPixelX = xMinHalfBrush + i;
                int currentPixelY = yMinHalfBrush + j;

                saveInHistory(currentPixelX , currentPixelY, m_iRed, m_iGreen, m_iBlue);

                updateDisplayedLayer(currentPixelX, currentPixelY, m_iRed, m_iGreen, m_iBlue);

                m_LayerStack->getLayer(m_LayerStack->getCurrentLayer())->writePixel(currentPixelX,
                                                                       currentPixelY,
                                                                       m_iRed,
                                                                       m_iGreen,
                                                                       m_iBlue);
            }
        }

        m_iLastX = x;
        m_iLastY = y;

        currentLayerChanged();
    }
}

void layerStackUI::erase(int x, int y)
{
    if(x != m_iLastX || y != m_iLastY){
        /****************************************************
         * Erase part of Layer
         * Warning: history must be called before erase pixel
         ****************************************************/
        int iHalfBrushSize = m_iBrushSize/2;
        int xMinHalfBrush = x - iHalfBrushSize;
        int yMinHalfBrush = y - iHalfBrushSize;

        for(int i = 0; i < m_iBrushSize; i++){
            for(int j = 0; j < m_iBrushSize; j++){
                int currentPixelX = xMinHalfBrush + i;
                int currentPixelY = yMinHalfBrush + j;

                saveInHistory(currentPixelX, currentPixelY, 0);

                m_LayerStack->getLayer(m_LayerStack->getCurrentLayer())->setPixelTransparent(currentPixelX,
                                                                              currentPixelY,
                                                                              0);
            }
        }

        m_iLastX = x;
        m_iLastY = y;

        currentLayerChanged();
    }
}

void layerStackUI::updateDisplayedLayer(int x, int y, int r, int g, int b)
{
    m_Painter->begin(m_frame);
    m_Painter->setPen(QPen(QBrush(QColor(r, g, b)), 1));
    m_Painter->drawPoint(x,y);
    m_Painter->end();
}

void layerStackUI::saveInHistory(int x, int y, int r, int g, int b)
{
    /* Compute delta */
    Pixel previousPixel = m_LayerStack->getLayer(m_LayerStack->getCurrentLayer())->getPixel(x, y);
    unsigned char previousTransparency = m_LayerStack->getLayer(m_LayerStack->getCurrentLayer())->getTransparency(x, y);

    PixelData delta;
    delta.deltaPixelRed= r - (int)previousPixel.red;
    delta.deltaPixelGreen = g - (int)previousPixel.green;
    delta.deltaPixelBlue = b - (int)previousPixel.blue;
    delta.deltaTransparency = 255 - (int)previousTransparency;

    delta.x = x;
    delta.y = y;
    delta.z = m_LayerStack->getCurrentLayer();

    historyData.append(delta);
}

void layerStackUI::saveInHistory(int x, int y, int transparency)
{
    /* Compute delta */
    unsigned char previousTransparency = m_LayerStack->getLayer(m_LayerStack->getCurrentLayer())->getTransparency(x, y);

    PixelData delta;
    delta.deltaPixelRed= 0;
    delta.deltaPixelGreen = 0;
    delta.deltaPixelBlue = 0;
    delta.deltaTransparency = transparency - (int)previousTransparency;

    delta.x = x;
    delta.y = y;
    delta.z = m_LayerStack->getCurrentLayer();

    historyData.append(delta);
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
            if(layer->getTransparency(x,y) == 255){
                bufPixel = layer->getPixel(x, y);
                m_Painter->setPen(QPen(QBrush(QColor(bufPixel.red,
                                                     bufPixel.green,
                                                     bufPixel.blue)),1));
                m_Painter->drawPoint(x,y);
            }
            else{
                m_Painter->setPen(QPen(QBrush(QColor(0,0,0)),1));
                m_Painter->drawPoint(x,y);
            }
        }
    }
    m_Painter->end();
}

void layerStackUI::historyUndoCall(LocalHistory *history)
{
    if(history->getId() == LOCAL_HISTORY_ID){
        LayerStackUIHistory *historyPtr = (LayerStackUIHistory*)history;
        QList<PixelData> delta = historyPtr->getDelta();

        for(int i = 0; i < delta.size(); i++){
            PixelData pixelData = delta.at(i);

            // Compute old values with delta
            int oldTransparency = m_LayerStack->getLayer(pixelData.z)->getTransparency(pixelData.x, pixelData.y)
                    - pixelData.deltaTransparency;
            int oldRed = m_LayerStack->getLayer(pixelData.z)->getPixel(pixelData.x, pixelData.y).red
                    - pixelData.deltaPixelRed;
            int oldGreen = m_LayerStack->getLayer(pixelData.z)->getPixel(pixelData.x, pixelData.y).green
                    - pixelData.deltaPixelGreen;
            int oldBlue = m_LayerStack->getLayer(pixelData.z)->getPixel(pixelData.x, pixelData.y).blue
                    - pixelData.deltaPixelBlue;

            m_LayerStack->getLayer(pixelData.z)->setPixelTransparent(pixelData.x,
                                                                     pixelData.y,
                                                                     (unsigned char)oldTransparency);
            m_LayerStack->getLayer(pixelData.z)->writePixel(pixelData.x,
                                                     pixelData.y,
                                                     (unsigned char)oldRed,
                                                     (unsigned char)oldGreen,
                                                     (unsigned char)oldBlue);
        }
        currentLayerChanged();
    }
}

void layerStackUI::historyRedoCall(LocalHistory *history)
{
    if(history->getId() == LOCAL_HISTORY_ID){
        LayerStackUIHistory *historyPtr = (LayerStackUIHistory*)history;
        QList<PixelData> delta = historyPtr->getDelta();
        for(int i = 0; i < delta.size(); i++){
            PixelData pixelData = delta.at(i);

            // Compute old values with delta
            int oldTransparency = (int)m_LayerStack->getLayer(pixelData.z)->getTransparency(pixelData.x, pixelData.y)
                    + pixelData.deltaTransparency;
            int oldRed = m_LayerStack->getLayer(pixelData.z)->getPixel(pixelData.x, pixelData.y).red
                    + pixelData.deltaPixelRed;
            int oldGreen = m_LayerStack->getLayer(pixelData.z)->getPixel(pixelData.x, pixelData.y).green
                    + pixelData.deltaPixelGreen;
            int oldBlue = m_LayerStack->getLayer(pixelData.z)->getPixel(pixelData.x, pixelData.y).blue
                    + pixelData.deltaPixelBlue;

            m_LayerStack->getLayer(pixelData.z)->setPixelTransparent(pixelData.x,
                                                                     pixelData.y,
                                                                     (unsigned char)oldTransparency);
            m_LayerStack->getLayer(pixelData.z)->writePixel(pixelData.x,
                                                     pixelData.y,
                                                     (unsigned char)oldRed,
                                                     (unsigned char)oldGreen,
                                                     (unsigned char)oldBlue);
        }
        currentLayerChanged();
    }
}
