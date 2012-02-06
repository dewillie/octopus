#ifndef MANAGERJOINTS_H
#define MANAGERJOINTS_H

#include "manager.h"

class ManagerJoints : public Manager
{
private:
    QList<Joint*> *listJoints;

public:
    ManagerJoints();
    void loadAll();
    void saveAll();
    void initSystem();
    QList<Joint*>* getListJoints();

};

#endif // MANAGERJOINTS_H
