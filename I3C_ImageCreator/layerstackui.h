#ifndef LAYERSTACKUI_H
#define LAYERSTACKUI_H

#include <QLabel>
#include <QPixmap>
#include <QPainter>
#include <QMouseEvent>
#include <QResizeEvent>
#include <QRect>
#include <math.h>
#include "layerstack.h"
#include "layerstackuihistory.h"
#include "BrushType.h"

//DEBUG PURPOSE
#include <iostream>
using namespace std;
//END DEBUG PURPOSE

class layerStackUI : public QLabel
{
    Q_OBJECT
public:
    explicit layerStackUI(QWidget *parent = 0, LayerStack *layerStack = 0);
    ~layerStackUI();

    /* Events */
    void resizeEvent(QResizeEvent*);
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent*);
    void mouseMoveEvent(QMouseEvent *event);

    void layerStackCreated(int sideSize);
    void currentLayerChanged();

private:
    void eventEditImageTriggered(int x, int y);

    void draw(int x, int y);
    void erase(int x, int y);

    void updateDisplayedLayer();
    void saveInHistory(int x, int y, int r, int g, int b, int a = 255);
    void addPixmapInTransparency(QPixmap *layer);

    void putLayerInPixmap(Layer *layer, QPixmap *pixmap);

signals:
    void initLayerStackDisplay();

public slots:
    void setActiveColor(int red, int green, int blue);
    void setEraserSelected(int eraserSize);
    void setPenSelected(int penSize);

    void historyUndoCall(LocalHistory *history);
    void historyRedoCall(LocalHistory *history);

signals:
    void modificationMade(LocalHistory *history);

private:
    LayerStack *m_LayerStack;
    int m_iSideSize;

    /* Event */
    bool m_bMouseButtonDwn;

    /* Status */
    QRect m_RectDrawable;
    double m_dPixelToPixelFactor;

    /* Layers */
    QPixmap *m_frame;
    QPixmap m_PixmapScaled;
    QPainter *m_Painter;

    /* Last State */
    int m_iLastX;
    int m_iLastY;

    /* Drawing */
    int m_iRed;
    int m_iGreen;
    int m_iBlue;
    BrushType m_BrushType;
    int m_iBrushSize;

    /* History */
    QList<PixelData> historyData;
};

#endif // LAYERSTACKUI_H
