#ifndef HISTORY_H
#define HISTORY_H

#include <QObject>
#include "localhistory.h"

#include <iostream>
using namespace std;

#define DEFAULT_HISTORY_SIZE    50

class History: public QObject
{
    Q_OBJECT
public:
    History();
    History(int historyLenght);
    ~History();

private:
    void initHistory(int historyLenght);

public slots:
    /* Triggered by Calls from MainWindow */
    void undo();
    void redo();

    void appendHistoryElement(LocalHistory *element);
    void appendHistoryElementSilently(LocalHistory *element);

signals:
    void newElementInHistory();
    void undoCall(LocalHistory*);
    void redoCall(LocalHistory*);

    void enableUndoButton(bool enable);
    void enableRedoButton(bool enable);

private:
    LocalHistory **m_HistoryArray;

    int m_iHistoryLenght;

    bool m_bHistoryEmpty;

    int m_iCurrent;
    int m_iUpBoundary;
    int m_iDwnBoundary;
};

#endif // HISTORY_H
