#ifndef ANALYSE_H
#define ANALYSE_H

#include "math.h"
#include <QtGlobal>
#include <QList>
#include <time.h>

#include "movement.h"
#include "databitg.h"

class Analyse
{
private:
    float pourcentage;
    float seuil;

public:
    Analyse();
    Analyse(float,float);
    void calculBITG(QList<Movement *> *);
private :
    QList<Position*> modifFreq(QList<Position*>* listPositionsJointMvt, int sizeMvt);
    Position* moyenne(Position* p1, Position *p2);
};

#endif // ANALYSE_H
