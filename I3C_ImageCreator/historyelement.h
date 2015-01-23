#ifndef HISTORYELEMENT_H
#define HISTORYELEMENT_H

/******************************************************************
 * Abstract Class
 *
 * Warning: Copy constructor must be implemented (if the default one
 *          is not suited).
 * *****************************************************************/

class HistoryElement
{
public:
    HistoryElement();

    /*Stored as Date or as Delta*/
private:
    int m_iCallerID;
};

#endif // HISTORYELEMENT_H
