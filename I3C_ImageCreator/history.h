/*********************************************************
 * History.h
 * Author:      Pascal Gendron
 * Version:     0.1.0
 * *******************************************************/

#ifndef HISTORY_H
#define HISTORY_H

#include <QObject>
#include "localhistory.h"

#define DEFAULT_HISTORY_SIZE    50

/* **************************************************************
 * The purpose of this class is to provide a centralized object
 * where the history would be stored. As the history might contain
 * different types of elements it only keeps a reference to all of
 * these elements and make sure to delete them when they are not
 * needed anymore.
 *
 * All these history elements must be derived from local history.
 * The class that we want to keep track of the history has the
 * role to create a new instance of the class derived from
 * LocalHistory and to pass it to History. There is no need to
 * delete it. This class(History) does this job.
 * **************************************************************/

class History: public QObject
{
    Q_OBJECT
public:
    History();
    History(int historyLenght);
    ~History();

private:
    void initHistory(int historyLenght);
    int incrementCursor(int cursor);
    int decrementCursor(int cursor);

    void deleteHistory();

public slots:
    void undo();
    void redo();

    void appendHistoryElement(LocalHistory *element);
    void appendHistoryElementSilently(LocalHistory *element);

    void updateButtonStatus();

    void clearHistory();

signals:
    void newElementInHistory();
    void undoCall(LocalHistory*);
    void redoCall(LocalHistory*);

    void enableUndoButton(bool enable);
    void enableRedoButton(bool enable);

private:
    LocalHistory **m_HistoryArray;

    int m_iHistoryLenght;

    int m_iCurrent;
    int m_iUpBoundary;
    int m_iDwnBoundary;
};

#endif // HISTORY_H
