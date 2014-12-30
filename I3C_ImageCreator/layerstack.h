/*********************************************************
 * LayerStack.h
 * Author:      Pascal Gendron
 * Version:     0.0.1
 * *******************************************************/

#ifndef LAYERSTACK_H
#define LAYERSTACK_H

#include "QLabel"
#include <QPixmap>
#include <QPainter>
#include <QMouseEvent>
#include <QResizeEvent>
#include "layer.h"

/*DEBUG PURPOSE*/
#include <iostream>
using namespace std;
/*END DEBUG*/

/*****************************************************************************
 * The purpose of this class is to contain a stack of layer and to provide the
 * ui that allow to edit them.
 * ***************************************************************************/

/******************************
 * TODO List
 * -Create history (undo & redo fnc)
 *
 * ****************************/

class LayerStack: public QLabel
{
    Q_OBJECT
public:
    explicit LayerStack(QWidget *parent =  0);
    virtual ~LayerStack();

    /* Events */
    void resizeEvent(QResizeEvent*);
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent*);
    void mouseMoveEvent(QMouseEvent *event);

    /* Image size functions */
    void setSideSize(int sideSize);
    int getSideSize();

    /* Layer Functions */
    int getCurrentLayer();

    /* Workspace functions */
    void setWorkspaceAsCurrentLayer();

private:
    void updateDisplayedLayer(int x, int y, int r, int g, int b);
    void addPixmapInTransparency(QPixmap *layer);

public slots:
    void nextLayer();
    void previousLayer();
    void goToLayer(int layer);

    void setActiveColor(int red, int green, int blue);

signals:
    void initLayerStackDisplay();

private:
    /* Event */
    bool m_bMouseButtonDwn;

    /* Status */
    bool m_bSideSizeSet;
    int m_iSideSize;
    double m_dPixelToPixelFactor;

    /* Layers */
    int m_iCurrentLayer;
    QPixmap *m_frame;
    QPixmap m_PixmapScaled;
    QPainter *m_Painter;
    Layer *m_LayerArray;
    int m_iOffsetCorrection;

    /* Drawing */
    int m_iRed;
    int m_iGreen;
    int m_iBlue;

};

#endif // LAYERSTACK_H
