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
    delete[] m_HistoryArray;
}

void History::initHistory(int historyLenght)
{
    m_HistoryArray = new HistoryElement[historyLenght];
    m_iHistoryLenght = historyLenght;
    m_iCurrent = 0;
    m_iDwnBoundary = 0;
    m_iUpBoundary = 0;
}

void History::appendHistoryElement(HistoryElement element)
{
    appendHistoryElementSilently(element);
    emit newElementInHistory();
}

void History::appendHistoryElementSilently(HistoryElement element)
{
    //FIXME: Problem in logic

    if(m_iCurrent == m_iUpBoundary){
        if(m_iUpBoundary >= m_iHistoryLenght){
            m_iUpBoundary -= m_iHistoryLenght;
        }

        m_HistoryArray[m_iCurrent] = element;
    }
    else{
        m_iCurrent++;
        if(m_iCurrent >= m_iHistoryLenght){
            m_iCurrent -= m_iHistoryLenght;
        }

        m_iUpBoundary = m_iCurrent + 1;
        if(m_iUpBoundary >= m_iHistoryLenght){
            m_iUpBoundary -= m_iHistoryLenght;
        }

        m_HistoryArray[m_iCurrent] = element;
    }

    if(m_iDwnBoundary == m_iUpBoundary){
        m_iDwnBoundary ++;
        if(m_iDwnBoundary >= m_iHistoryLenght){
            m_iDwnBoundary -= m_iHistoryLenght;
        }
    }

    /* See if we need to update buttons */
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

void History::undo()
{
    if(m_iCurrent != m_iDwnBoundary){
        m_iCurrent --;
        emit undoCall(m_HistoryArray[m_iCurrent]);
    }

    if(m_iCurrent == m_iDwnBoundary){
        emit enableUndoButton(false);
    }
}

void History::redo()
{
    if(m_iCurrent != m_iUpBoundary){
        m_iCurrent ++;
        emit redoCall(m_HistoryArray[m_iCurrent]);
    }

    if(m_iCurrent == m_iUpBoundary){
        emit enableRedoButton(false);
    }
}
