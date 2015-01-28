#include "layerstackuihistory.h"

LayerStackUIHistory::LayerStackUIHistory()
{
}

void LayerStackUIHistory::setDelta(QList<PixelData> changeList)
{
    m_changeList = changeList;
}

QList<PixelData> LayerStackUIHistory::getDelta()
{
    return m_changeList;
}
