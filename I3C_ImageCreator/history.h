#ifndef HISTORY_H
#define HISTORY_H

#include <QObject>
#include "historyelement.h"

#define DEFAULT_HISTORY_SIZE    50

class History: public QObject
{
    Q_OBJECT
public:
    History();
    History(int historyLenght);
    ~History();

    void appendHistoryElement(HistoryElement element);
    void appendHistoryElementSilently(HistoryElement element);

private:
    void initHistory(int historyLenght);

public slots:
    /* Triggered by Calls from MainWindow */
    void undo();
    void redo();

signals:
    void newElementInHistory();
    void undoCall(HistoryElement);
    void redoCall(HistoryElement);

    void enableUndoButton(bool enable);
    void enableRedoButton(bool enable);

private:
    HistoryElement *m_HistoryArray;

    int m_iHistoryLenght;

    int m_iCurrent;
    int m_iUpBoundary;
    int m_iDwnBoundary;
};

#endif // HISTORY_H
