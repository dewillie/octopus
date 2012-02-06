#include "analyse.h"

Analyse::Analyse(){
    this->pourcentage=0;
}


Analyse::Analyse(float pourc,float seuil)
{
    this->pourcentage=pourc;
    this->seuil= seuil;
    //this->calculBITG(moves);
}

void Analyse::calculBITG(QList<Movement*>* mouv){
    quint32 tailleBuffer = SIZE_MAX_BUFFERS;

    for(int k=0; k<mouv->size();k++) { // chaque Mvt
        for (int j=0;j<mouv->at(k)->getListJointsMvt()->size();j++) { // chaque JointMvt
            if (mouv->at(k)->getListJointsMvt()->at(j)->getJointRef()->getNom() == "lefthand"){
                qDebug() << "Last buffer JointRef lefthand X : " << mouv->at(k)->getListJointsMvt()->at(j)->getJointRef()->getBufferPositions()->last()->getDx() << endl;
            }
        }
    }

    QList<Position*> EnregistrementVite;

    int m=0;
    int jt=0;
    int T=0;
    int t=0;
    int r=0;

    float moyenneX=0;
    float moyenneY=0;
    float moyenneZ=0;

    float moyenneJ=0;
    float moyenneG=0;

    float moyenneGtemp=seuil+1;
    quint16 decal=0;

    quint32 sommeX=0;
    quint32 sommeY=0;
    quint32 sommeZ=0;

    quint16 TempsEnregistrement;
    quint16 TempsRallonge;
    quint16 TempsReduit;

    quint16 variation;

    quint16 PasVitesse=0;
    quint16 PasDecalage=0;

}

QList<Position*> Analyse::modifFreq(QList<Position*>* listPositionsJointMvt, int T){
    QList<Position*> newPositionsJointMvt;
    int j=0;
    int k=0;
    int reste=0;

    // travail sur la taille souhaitee
    if(listPositionsJointMvt->size()!=T){
        j= (listPositionsJointMvt->size())/(abs(listPositionsJointMvt->size()-T));
        reste = (listPositionsJointMvt->size()/j)-abs((listPositionsJointMvt->size()-T));
        k=j;
        j=j-1;
    }

    // Agrandir le jointMvt
    if(T >= listPositionsJointMvt->size() ){
        //parcourir les positions du jointMvt
        for(int i=0; i<listPositionsJointMvt->size(); i++){
            if(i==j && j!=0){
                newPositionsJointMvt.append(moyenne(listPositionsJointMvt->at(i-1), listPositionsJointMvt->at(i)));
                newPositionsJointMvt.append(listPositionsJointMvt->at(i));
                j=j+k;
            }else{
                newPositionsJointMvt.append(listPositionsJointMvt->at(i));
            }

        }
        //Si y'a des cases en trop, on a supprime aleatoirement
        if(reste!=0){
            for(int i=0; i<reste;i++){
                newPositionsJointMvt.removeAt(rand()%(T-reste));
            }
        }

    }
    // Retrecir le jointMvt
    else{
        //parcourir les positions du jointMvt
        for(int i=0; i<listPositionsJointMvt->size(); i++){
            if(i==j){
                j=j+k;
            }else{
                newPositionsJointMvt.append(listPositionsJointMvt->at(i));
            }
        }
        if(reste!=0){
            int random;
            for(int i=0; i<reste;i++){
                random = rand()%(T-1)+1;
                newPositionsJointMvt.insert(random,moyenne(listPositionsJointMvt->at(random-1), listPositionsJointMvt->at(random)));
            }
        }
    }

    //qDebug() << "Taille nouveau : "<< Temp.size() <<endl<<endl;
    return newPositionsJointMvt;
}

Position* Analyse::moyenne(Position *p1, Position *p2)
{
    return new Position(
                0,
                (p1->getX() + p2->getX()) / 2,
                (p1->getY() + p2->getY()) / 2,
                (p1->getZ() + p2->getZ()) / 2,
                (p1->getDx() + p2->getDx()) / 2,
                (p1->getDy() + p2->getDy()) / 2,
                (p1->getDz() + p2->getDz()) / 2,
                (p1->getA() + p2->getA()) /2,
                (p1->getB() + p2->getB()) /2,
                (p1->getC() + p2->getC()) /2
                );
}
