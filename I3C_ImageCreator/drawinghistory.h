#ifndef DRAWINGHISTORY_H
#define DRAWINGHISTORY_H

#include <QList>
#include "pixel.h"
#include "historyelement.h"

struct PixelData{
    int x;
    int y;
    int z;
    int deltaPixelRed;
    int deltaPixelGreen;
    int deltaPixelBlue;
    int deltaTransparency;
};


class DrawingHistory: public HistoryElement
{
public:
    DrawingHistory();

    void setDelta(QList<PixelData> changeList);
    QList<PixelData> getDelta();

private:
    QList<PixelData> m_changeList;
};

#endif // DRAWINGHISTORY_H
