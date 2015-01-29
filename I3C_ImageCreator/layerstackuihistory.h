#ifndef LAYERSTACKUIHISTORY_H
#define LAYERSTACKUIHISTORY_H

#include <QList>
#include "pixel.h"
#include "localhistory.h"

#define LOCAL_HISTORY_ID    56

struct PixelData{
    int x;
    int y;
    int z;
    int deltaPixelRed;
    int deltaPixelGreen;
    int deltaPixelBlue;
    int deltaTransparency;
};

class LayerStackUIHistory : public LocalHistory
{
public:
    LayerStackUIHistory();

    void setDelta(QList<PixelData> changeList);
    QList<PixelData> getDelta();

private:
    QList<PixelData> m_changeList;
};

#endif // LAYERSTACKUIHISTORY_H
