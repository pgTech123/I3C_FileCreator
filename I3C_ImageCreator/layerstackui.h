#ifndef LAYERSTACKUI_H
#define LAYERSTACKUI_H

#include <QLabel>
#include <QPixmap>
#include <QPainter>
#include <QMouseEvent>
#include <QResizeEvent>
#include "layerstack.h"
#include "layerstackuihistory.h"

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
    void updateDisplayedLayer(int x, int y, int r, int g, int b);
    void addPixmapInTransparency(QPixmap *layer);

    void putLayerInPixmap(Layer *layer, QPixmap *pixmap);

signals:
    void initLayerStackDisplay();

public slots:
    void setActiveColor(int red, int green, int blue);

    void historyUndoCall(LocalHistory *history);
    void historyRedoCall(LocalHistory *history);

signals:
    void modificationMade(LocalHistory *history);

private:
    LayerStack *m_LayerStack;

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

    /* History */
    QList<PixelData> historyData;
};

#endif // LAYERSTACKUI_H
