#ifndef PIXMAPLAYERSTACK_H
#define PIXMAPLAYERSTACK_H

#include "QLabel"
#include <QPixmap>
#include <QPainter>
#include <QMouseEvent>
#include <QResizeEvent>
#include "layerstack.h"
#include "layer.h"
#include "pixel.h"

/*DEBUG PURPOSE*/
#include <iostream>
using namespace std;
/*END DEBUG*/

/*****************************************************************************
 * The purpose of this class is to contain a stack of pixmap layer and to provide
 * the ui that allow to edit them.
 * ***************************************************************************/

/******************************
 * TODO List
 * -Create history (undo & redo fnc)
 *
 * ****************************/

class PixmapLayerStack: public LayerStack , public QLabel
{
public:
    Q_OBJECT
public:
    explicit PixmapLayerStack(QWidget *parent =  0);
    virtual ~PixmapLayerStack();

    /* Events */
    void resizeEvent(QResizeEvent*);
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent*);
    void mouseMoveEvent(QMouseEvent *event);

    /* Workspace functions */
    void setWorkspaceAsCurrentLayer();

private:
    void layerStackCreated(int sideSize);
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
    double m_dPixelToPixelFactor;

    /* Layers */
    QPixmap *m_frame;
    QPixmap m_PixmapScaled;
    QPainter *m_Painter;
    int m_iOffsetCorrection;

    /* Drawing */
    int m_iRed;
    int m_iGreen;
    int m_iBlue;

};

#endif // PIXMAPLAYERSTACK_H
