/*********************************************************
 * History.cpp
 * Author:      Pascal Gendron
 * Version:     0.1.0
 * *******************************************************/

#include "history.h"

History::History()
{
    initHistory(DEFAULT_HISTORY_SIZE);
}

History::History(int historyLenght)
{
    initHistory(historyLenght);
}

History::~History()
{
    deleteHistory();
}

void History::initHistory(int historyLenght)
{
    m_HistoryArray = new LocalHistory*[historyLenght];
    m_iHistoryLenght = historyLenght;
    for(int  i = 0; i < m_iHistoryLenght; i++){
        m_HistoryArray[i] = NULL;
    }
    m_iCurrent = -1;
    m_iDwnBoundary = -1;
    m_iUpBoundary = -1;
}

int History::incrementCursor(int cursor)
{
    cursor ++;
    if (cursor < m_iHistoryLenght){
        return cursor;
    }
    else{
        return cursor - m_iHistoryLenght;
    }
}

int History::decrementCursor(int cursor)
{
    cursor --;
    if (cursor >= 0){
        return cursor;
    }
    else{
        return cursor + m_iHistoryLenght;
    }
}

void History::deleteHistory()
{
    for(int i = 0; i < m_iHistoryLenght; i++){
        if(m_HistoryArray[i] != NULL){
            delete m_HistoryArray[i];
        }
    }

    delete[] m_HistoryArray;
}

void History::appendHistoryElement(LocalHistory *element)
{
    appendHistoryElementSilently(element);
    emit newElementInHistory();
}

void History::appendHistoryElementSilently(LocalHistory *element)
{
    m_iCurrent = incrementCursor(m_iCurrent);

    /* Clear if needed */
    if(m_HistoryArray[m_iCurrent] != NULL){
        delete m_HistoryArray[m_iCurrent];
    }

    /* Add Element */
    m_HistoryArray[m_iCurrent] = element;

    /* Update Delimiters */
    m_iUpBoundary = m_iCurrent;
    if(m_iDwnBoundary == m_iCurrent || m_iDwnBoundary == -1){
        m_iDwnBoundary = incrementCursor(m_iDwnBoundary);
    }

    /* Update Buttons */
    emit enableUndoButton(true);
    emit enableRedoButton(false);
}

void History::undo()
{
    emit undoCall(m_HistoryArray[m_iCurrent]);
    if(m_iCurrent != m_iDwnBoundary){
        m_iCurrent = decrementCursor(m_iCurrent);
    }
    updateButtonStatus();
}

void History::redo()
{
    if(m_iCurrent != m_iUpBoundary){
        m_iCurrent = incrementCursor(m_iCurrent);
        emit redoCall(m_HistoryArray[m_iCurrent]);
    }
    updateButtonStatus();
}

void History::updateButtonStatus()
{
    if(m_iCurrent == m_iDwnBoundary){
        emit enableUndoButton(false);
    }
    else{
        emit enableUndoButton(true);
    }
    if(m_iCurrent == m_iUpBoundary){
        emit enableRedoButton(false);
    }
    else{
        emit enableRedoButton(true);
    }
}

void History::clearHistory()
{
    deleteHistory();
    initHistory(m_iHistoryLenght);
}
