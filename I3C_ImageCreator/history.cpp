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
    for(int i = 0; i < m_iHistoryLenght; i++){
        if(m_HistoryArray[i] != NULL){
            delete m_HistoryArray[i];
        }
    }

    delete[] m_HistoryArray;
}

void History::initHistory(int historyLenght)
{
    m_HistoryArray = new LocalHistory*[historyLenght];
    m_iHistoryLenght = historyLenght;
    for(int  i = 0; i < m_iHistoryLenght; i++){
        m_HistoryArray[i] = NULL;
    }

    m_iCurrent = 0;
    m_iDwnBoundary = m_iHistoryLenght-1;
    m_iUpBoundary = 0;
    m_bHistoryEmpty = true;
}

void History::appendHistoryElement(LocalHistory *element)
{
    appendHistoryElementSilently(element);
    emit newElementInHistory();
}

void History::appendHistoryElementSilently(LocalHistory *element)
{
    if(m_bHistoryEmpty){
        m_HistoryArray[m_iCurrent] = element;
        m_bHistoryEmpty = false;
    }
    else{
        /* Update position */
        m_iCurrent++;
        m_iUpBoundary = m_iCurrent;

        /* Keep in array range */
        if(m_iCurrent >= m_iHistoryLenght){
            m_iCurrent -= m_iHistoryLenght;
        }
        if(m_iUpBoundary >= m_iHistoryLenght){
            m_iUpBoundary -= m_iHistoryLenght;
        }
        cout<< m_iCurrent <<endl;
        cout<< "Dwn : " << m_iDwnBoundary <<endl;
        m_HistoryArray[m_iCurrent] = element;

        /* Prepare for next insert in history */
        if(m_iDwnBoundary == m_iUpBoundary){
            m_iDwnBoundary ++;
            if(m_iDwnBoundary >= m_iHistoryLenght){
                m_iDwnBoundary -= m_iHistoryLenght;
            }
            /* Delete previously stored */
            cout<< m_iDwnBoundary <<endl;
            delete m_HistoryArray[m_iDwnBoundary];
            m_HistoryArray[m_iDwnBoundary] = NULL;
            cout<< "Delete done" <<endl;
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
    int dwnBoundaryPlus1 = m_iDwnBoundary + 1;
    if(dwnBoundaryPlus1 > m_iHistoryLenght){
        dwnBoundaryPlus1 -= m_iHistoryLenght;
    }
    if(m_iCurrent != m_iDwnBoundary && m_iCurrent != dwnBoundaryPlus1){
        m_iCurrent --;
        if(m_iCurrent < 0){
            m_iCurrent += m_iHistoryLenght;
        }
        emit undoCall(m_HistoryArray[m_iCurrent]);
    }
    else{
        emit enableUndoButton(false);
    }

    if(m_iCurrent != m_iUpBoundary){
        emit enableRedoButton(true);
    }
}

void History::redo()
{
    if(m_iCurrent != m_iUpBoundary){
        m_iCurrent ++;
        if(m_iCurrent >= m_iHistoryLenght){
            m_iCurrent -= m_iHistoryLenght;
        }
        emit redoCall(m_HistoryArray[m_iCurrent]);
    }
    else{
        emit enableRedoButton(false);
    }

    if(m_iCurrent != m_iDwnBoundary){
        emit enableUndoButton(true);
    }
}
