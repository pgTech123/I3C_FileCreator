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
}

void History::appendHistoryElement(HistoryElement element)
{
    appendHistoryElementSilently(element);
    emit newElementInHistory();
}

void History::appendHistoryElementSilently(HistoryElement element)
{
    //TODO
}

void History::undo()
{
    //TODO: make a call
}

void History::redo()
{
    //TODO: make a call
}
