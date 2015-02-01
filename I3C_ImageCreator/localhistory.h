/*********************************************************
 * LocalHistory.h
 * Author:      Pascal Gendron
 * Version:     0.1.0
 * *******************************************************/

#ifndef LOCALHISTORY_H
#define LOCALHISTORY_H

/* ************************************************************
 * The purpose of this class is to be derived to form elements
 * that can be stored in history. It is not officially an
 * abstract class but... it does nothing interesting by itself.
 * ************************************************************/

class LocalHistory
{
public:
    LocalHistory();
    virtual ~LocalHistory();

    int getId();

protected:
    int id;
};

#endif // LOCALHISTORY_H
