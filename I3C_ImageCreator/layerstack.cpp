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
    m_Painter = new QPainter();
    m_bSideSizeSet = false;
    m_bMouseButtonDwn = false;
    m_iCurrentLayer = 0;

    /* Init Color to Black */
    m_iRed = 0;
    m_iGreen = 0;
    m_iBlue = 0;
}

LayerStack::~LayerStack()
{
    if(m_frame != NULL){
        delete m_frame;
    }
    delete m_Painter;
    if(m_LayerArray != NULL){
        delete[] m_LayerArray;
    }

}

void LayerStack::resizeEvent(QResizeEvent*)
{
    if(m_bSideSizeSet){
        /* x 0.8 to be able to size down */
        int newHeight = this->height()*0.8;
        int newWidth = this->width()*0.8;

        /* As we draw on this image, we dont want to unzoom too much */
        if(newWidth >= m_iSideSize && newHeight >= m_iSideSize){
            m_PixmapScaled = m_frame->scaled(newWidth, newHeight, Qt::KeepAspectRatio);

            /* Adjust scale factor between real size of 1 pixel and the size of 1 pix displayed */
            m_dPixelToPixelFactor = (double)m_iSideSize / (double)m_PixmapScaled.width();
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

void LayerStack::mousePressEvent(QMouseEvent *event)
{
    m_bMouseButtonDwn = true;

    /* Draw on Layer */
    m_LayerArray[m_iCurrentLayer].writePixel(event->x()*m_dPixelToPixelFactor,
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

void LayerStack::mouseReleaseEvent(QMouseEvent*)
{
    m_bMouseButtonDwn = false;
}

void LayerStack::mouseMoveEvent(QMouseEvent *event)
{
    if(m_bMouseButtonDwn){
        /* Draw on Layer */
        m_LayerArray[m_iCurrentLayer].writePixel(event->x()*m_dPixelToPixelFactor,
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
        setWorkspaceAsCurrentLayer();

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

bool LayerStack::isAPixelWritten(int x, int y, int z, int w, int h, int d)
{
    int maxX = x + w;
    int maxY = y + h;
    int maxZ = z + d;

    if(x >= m_iSideSize || y >= m_iSideSize || z >= m_iSideSize ||
            maxX > m_iSideSize || maxY > m_iSideSize || maxZ > m_iSideSize){
        cout << "boundary error layer stack" << endl;
        cout << "max X" << maxX <<endl;
        cout << "max Y" << maxY <<endl;
        cout << "max Z" << maxZ <<endl;
        cout << "X" << x <<endl;
        cout << "Y" << y <<endl;
        cout << "Z" << z <<endl;//*/

        return false;
    }

    unsigned char *transparencyMap;
    for(int posZ = z; posZ < maxZ; posZ++){
        transparencyMap = m_LayerArray[posZ].getTransparencyMapPtr();
        for(int posX = x; posX < maxX; posX++){
            for(int posY = y; posY < maxY; posY++){
                if(transparencyMap[posX + posY*m_iSideSize] != 0){
                    return true;
                }
            }
        }
    }
    return false;
}

Pixel LayerStack::getPixelAt(int x, int y, int z)
{
     return m_LayerArray[z].getPixel(x,y);
}

void LayerStack::setActiveColor(int red, int green, int blue)
{
    m_iRed = red;
    m_iGreen = green;
    m_iBlue = blue;
}

void LayerStack::updateDisplayedLayer(int x, int y, int r, int g, int b)
{
    m_Painter->begin(m_frame);
    m_Painter->setPen(QPen(QBrush(QColor(r, g, b)),1));
    m_Painter->drawPoint(x,y);
    m_Painter->end();
    resizeEvent(NULL);
}

void LayerStack::addPixmapInTransparency(QPixmap *layer)
{
    m_Painter->begin(m_frame);
    m_Painter->setOpacity(0.3);
    m_Painter->drawPixmap(0,0, *layer);
    m_Painter->end();
}
