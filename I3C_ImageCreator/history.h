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
    void enableUndoButton(bool enable);
    void enableRedoButton(bool enable);

private:
    HistoryElement *m_HistoryArray;
};

#endif // HISTORY_H
