#ifndef LOCALHISTORY_H
#define LOCALHISTORY_H

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
