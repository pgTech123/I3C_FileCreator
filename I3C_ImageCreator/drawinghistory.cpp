#include "drawinghistory.h"

DrawingHistory::DrawingHistory()
{
}

void DrawingHistory::setDelta(QList<PixelData> changeList)
{
    m_changeList = changeList;
}

QList<PixelData> DrawingHistory::getDelta()
{
    return m_changeList;
}
