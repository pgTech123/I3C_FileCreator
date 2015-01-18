#ifndef HISTORY_H
#define HISTORY_H

#include <QObject>
#include "historyelement.h"

class History: public QObject
{
    Q_OBJECT
public:
    History();
    History(int historyLenght);
    ~History();

    void appendHistoryElement(HistoryElement element);

public slots:
    /* Triggered by Calls from MainWindow */
    void undo();
    void redo();

signals:
    void newElementInHistory();
    void enableUndoButton(bool enable);
    void enableRedoButton(bool enable);
};

#endif // HISTORY_H
