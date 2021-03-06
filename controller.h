#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "managerjoints.h"
#include "managerelements.h"
#include "serverosc.h"
#include "clientosc.h"
#include "analyse.h"

class Controller
{
private:
    ManagerJoints *managerJoints;
    ManagerElements *managerElements;

public:
    Controller();
    ManagerJoints* getManagerJoints();
    ManagerElements* getManagerElements();
    void linkJointToJointMvt();

    void bubble(QList<Movement *> *moves);
    void troncage(Movement* move);

};

#endif // CONTROLLER_H
