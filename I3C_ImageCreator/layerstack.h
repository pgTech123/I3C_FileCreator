#ifndef LAYERSTACK_H
#define LAYERSTACK_H

#include "QLabel"
#include <QPixmap>
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

class LayerStack: public QLabel
{
    Q_OBJECT
public:
    explicit LayerStack(QWidget *parent =  0);
    virtual ~LayerStack();
    void resizeEvent(QResizeEvent *event);

    void setSideSize(int sideSize);
    int getSideSize();

    int getCurrentLayer();

    void setWorkspaceAsCurrentLayer();

public slots:
    void nextLayer();
    void previousLayer();
    void goToLayer(int layer);

    void setActiveColor(int red, int green, int blue);

signals:
    void initLayerStackDisplay();

private:
    /* Status */
    bool m_bSideSizeSet;
    int m_iSideSize;

    /* Layers */
    int m_iCurrentLayer;
    QPixmap *m_frame;
    Layer *m_layerArray;

};

#endif // LAYERSTACK_H
