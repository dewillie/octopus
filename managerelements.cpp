#include "managerelements.h"

ManagerElements::ManagerElements() : Manager()
{
    //Initialisation de la serialisation des joints
	this->initSystem();

    this->listMovements = new QList<Movement*>();
    this->listPorts = new QList<ClientOSC*>();
    this->listSamplesAudios = new QList<SampleAudio*>();
    this->listSamplesVideos = new QList<SampleVideo*>();


}

/**
  *   FONCTIONS LOAD ET SAVE GENERALES
  **/
void ManagerElements::loadAll()
{
    qDebug() << "   debut loadAll()..." << endl;
    this->loadAllMovements();
    qDebug() << "   ...fin loadAll()" << endl;

}

void ManagerElements::saveAll()
{
    qDebug() << "   debut saveAll()..." << endl;
    this->saveAllMovements();
    qDebug() << "   ...fin saveAll()" << endl;

}

/**
  *  FONCTIONS LOAD ET SAVE DE TOUT LES MOVEMENTS
  **/
void ManagerElements::saveAllMovements()
{
    qDebug() << "      debut saveAllMovements()..." << endl;

    //SAUVEGARDE DES MOUVEMENTS
    QFile::remove("movement.ini");
    QFile::remove("jointmvt.ini");
    QFile::remove("position.ini");
    QFile::remove("sampleaudio.ini");
    QFile::remove("samplevideo.ini");
    QSettings fichierMovement("movement.ini", QSettings::IniFormat);

    for(int i = 0 ; i < this->listMovements->size() ; i++)
        this->saveMovement(this->listMovements->at(i), fichierMovement);
    fichierMovement.sync();
    qDebug() << "      ...fin saveAllMovements()" << endl;
}

/**
  *  FONCTIONS LOAD ET SAVE ET DELETE D'UN MOVEMENT
  **/
void ManagerElements::loadAllMovements()
{
    //qDebug() << "      debut loadMovements()..." << endl;
    //CHARGEMENT LISTE MOVEMENTS
    this->listMovements = new QList<Movement*>();
    QSettings fichierMovement("movement.ini", QSettings::IniFormat);
    for(int i = 0 ; i < fichierMovement.allKeys().size() ; i++)
        this->listMovements->append(new Movement(fichierMovement.value(fichierMovement.allKeys().at(i), qVariantFromValue(Movement())).value<Movement>()));

    //CHARGEMENT DE LA LISTE TEMPORAIRE DES JOINTS MOUVEMENTS ET ASSIGNEMENT DE CHACUN DANS LEUR MOUVEMENT RESPECTIF
    // CHARGEMENT DE LA LISTE TEMPORAIRE DES POSITIONS ET ASSIGNEMENT DE CHACUN DANS LEUR JOINT MOUVEMENT RESPECTIF
    this->loadJointMvt();
    this->loadPositions();
    this->loadSamplesAudios();
    this->loadSamplesVideos();

    for(int i = 0 ; i < this->listMovements->size() ; i++)
    {
        for(int j = 0 ; j < this->listJointsMvts->size() ; j++)
        {
            JointMvt *jointMvtTemp = this->listJointsMvts->at(j);
            if(this->listMovements->at(i)->getId() == jointMvtTemp->getIdMovement())
            {
                for(int k = 0 ; k < this->listPositionsTemp->size() ; k++)
                {
                    if(jointMvtTemp->getIdJointMvt() == this->listPositionsTemp->at(k)->getIdJointMvt())
                        jointMvtTemp->addPosition(this->listPositionsTemp->at(k));
                }
                this->listMovements->at(i)->addJointMvt(this->listJointsMvts->at(j));
            }
        }
        for(int j = 0 ; j < this->listSamplesAudios->size() ; j++)
            if(this->listMovements->at(i)->getId() == this->listSamplesAudios->at(j)->getIdMovement())
                this->listMovements->at(i)->setSampleAudio(this->listSamplesAudios->at(j));
        for(int j = 0 ; j < this->listSamplesVideos->size() ; j++)
            if(this->listMovements->at(i)->getId() == this->listSamplesVideos->at(j)->getIdMovement())
                this->listMovements->at(i)->setSampleVideo(this->listSamplesVideos->at(j));
    }
    if(this->listPositionsTemp->isEmpty())
        Position::idPositionsStatic = 0;
    else{
        int tmp = 0;
        for(int i=0; i<listPositionsTemp->size();i++)
            if (tmp < listPositionsTemp->at(i)->getId()) tmp = listPositionsTemp->at(i)->getId();
        Position::idPositionsStatic = tmp;
    }
    if(this->listJointsMvts->isEmpty())
        JointMvt::idJointMvtStatic = 0;
    else{
        int tmp = 0;
        for(int i=0; i<listJointsMvts->size();i++)
            if(tmp < listJointsMvts->at(i)->getIdJointMvt()) tmp = listJointsMvts->at(i)->getIdJointMvt();
        JointMvt::idJointMvtStatic = tmp;

    }if(this->listMovements->isEmpty())
        Movement::idMovementStatic = 0;
    else {
        int tmp = 0;
        for(int i=0; i<listMovements->size(); i++)
            if(tmp < listMovements->at(i)->getId()) tmp = listMovements->at(i)->getId();
        Movement::idMovementStatic = tmp;
    }if(this->listSamplesAudios->isEmpty())
        SampleAudio::idSampleAudioStatic = 0;
    else {
        int tmp = 0;
        for(int i=0; i<listSamplesAudios->size(); i++)
            if(tmp < listSamplesAudios->at(i)->getId()) tmp = listSamplesAudios->at(i)->getId();
        SampleAudio::idSampleAudioStatic = tmp;
    }if(this->listSamplesVideos->isEmpty())
        SampleVideo::idSampleVideoStatic = 0;
    else {
        int tmp = 0;
        for(int i=0; i<listSamplesVideos->size(); i++)
            if(tmp < listSamplesVideos->at(i)->getId()) tmp = listSamplesVideos->at(i)->getId();
        SampleVideo::idSampleVideoStatic = tmp;
    }
}

void ManagerElements::saveMovement(Movement *movement, QSettings &fichierMovement)
{
    QSettings fichierJointMvt("jointmvt.ini", QSettings::IniFormat);
    QSettings fichierPosition("position.ini", QSettings::IniFormat);
    QSettings fichierSampleAudio("sampleaudio.ini", QSettings::IniFormat);
    QSettings fichierSampleVideo("samplevideo.ini", QSettings::IniFormat);

    // enregistrer en fonction du jointmvt ayant le moins de positions
    int size = movement->getListJointsMvt()->at(0)->getListPositions()->size();
    for(int i=0; i<movement->getListJointsMvt()->size();i++) {
        if(movement->getListJointsMvt()->at(i)->getListPositions()->size() < size)
            size = movement->getListJointsMvt()->at(i)->getListPositions()->size();
    }

    for(int j = 0 ; j < movement->getListJointsMvt()->size() ; j++)
    {
        JointMvt *jointMvtTemp = movement->getListJointsMvt()->at(j);
        //parcourir jusqu'a la position SIZE correspondant a l'indice max du tableau de positions le plus petit
        for(int k = 0 ; k < size ; k++){
            this->savePosition(jointMvtTemp->getListPositions()->at(k), fichierPosition);
        }
        this->saveJointMvt(jointMvtTemp, fichierJointMvt);
    }
    this->saveSampleAudio(movement->getSampleAudio(), fichierSampleAudio);
    this->saveSampleVideo(movement->getSampleVideo(), fichierSampleVideo);
    QString key = QString::number(movement->getId());
    fichierMovement.setValue(key, qVariantFromValue(*movement));
    fichierJointMvt.sync();
    fichierPosition.sync();
    fichierSampleAudio.sync();
    fichierSampleVideo.sync();
}

void ManagerElements::removeMovement(Movement *movement)
{
    qDebug() << "         debut removeMovement..." << endl;
    quint32 idTemp = movement->getId();
    //On remove la liste de joint mouvement du fichier de serialisation
    this->removeJointsMvts(movement->getListJointsMvt());
    for(int i = 0 ;  i < movement->getListJointsMvt()->size() ; i++)
        movement->getListJointsMvt()->removeAt(i);
    QSettings fichierMovement("movement.ini", QSettings::IniFormat);

    //On supprime le mouvement du fichier de serialisation
    fichierMovement.remove(QString(movement->getId()));
    //On supprime le dernier mouvement de la liste de mouvement du fichier de serialisation
    fichierMovement.remove(QString(this->getListMovements()->last()->getId()));
    //ON SUPPRIME LE MOUVEMENT ET SES COMPOSANTES de la liste
    for(int i = 0 ; i < this->listMovements->size() ; i++)
        if(this->listMovements->at(i)->getId() == movement->getId())
        {
            delete(this->listMovements->at(i));
            this->listMovements->removeAt(i);
        }
    //On met a jour l'id du denier mouvement de la liste
    this->getListMovements()->last()->updateId(idTemp);
    //On le sauvegarde update
    this->saveMovement(this->listMovements->last(), fichierMovement);
    fichierMovement.sync();

    Movement::idMovementStatic--;
    qDebug() << "         ...fin removeMovement" << endl;
}

/**
  *   FONCTIONS LOAD ET DELETE ET SAVE JOINTMVT
  **/

void ManagerElements::loadJointMvt(){
    this->listJointsMvts = new QList<JointMvt*>();
    //CHARGEMENT LISTE JOINTMOVEMENTS
    QSettings fichierJointMvt("jointmvt.ini", QSettings::IniFormat);
    for(int i = 0 ; i < fichierJointMvt.allKeys().size() ; i++)
    {
        JointMvt* jointMvt = new JointMvt(fichierJointMvt.value(fichierJointMvt.allKeys().at(i), qVariantFromValue(JointMvt())).value<JointMvt>());
        this->listJointsMvts->append(jointMvt);
    }
}

void ManagerElements::saveJointMvt(JointMvt *jointMvt, QSettings &fichierJointMvt){
    QString key = QString::number(jointMvt->getIdJointMvt());
    fichierJointMvt.setValue(key, qVariantFromValue(*jointMvt));
}

void ManagerElements::removeJointsMvts(QList<JointMvt*> *listJointsMvtsToDelete)
{
    QSettings fichierJointMvt("jointmvt.ini", QSettings::IniFormat);
    for(int i = 0 ; i < listJointsMvtsToDelete->size() ; i++)
    {
        JointMvt *jointMvtTemp = listJointsMvtsToDelete->at(i);
        //On supprime toutes les positions du joint mouvement dans le fichier
        this->removePositions(jointMvtTemp->getListPositions());
        //on supprime chaque position dans la liste de position du joint mouvement
        for(int i = 0 ; i < jointMvtTemp->getListPositions()->size() ; i++)
            jointMvtTemp->getListPositions()->removeAt(i);
        //on supprime le joint mouvement du fichier
        fichierJointMvt.remove(QString(jointMvtTemp->getIdJointMvt()));
        //on supprime le dernier joint du fichier
        fichierJointMvt.remove(QString(this->listJointsMvts->last()->getIdJointMvt()));
        //on update son id avec celle du joint mouvement a supprimer
        this->listJointsMvts->last()->updateIdJointMvt(jointMvtTemp->getIdJointMvt());
        //on le save a nouveau update en memoire
        this->saveJointMvt(this->listJointsMvts->last(), fichierJointMvt);
        JointMvt::idJointMvtStatic--;
    }
    fichierJointMvt.sync();
}

/**
  *   FONCTIONS LOAD ET DELETE ET SAVE POSITIONS
  **/
void ManagerElements::savePosition(Position *position, QSettings &fichierPosition)
{
    QString key = QString::number(position->getId());
    fichierPosition.setValue(key, qVariantFromValue(*position));
}

// Charger toutes les Positions
void ManagerElements::loadPositions()
{
    this->listPositionsTemp = new QList<Position*>();
    QSettings fichierPosition("position.ini", QSettings::IniFormat);
    for(int i = 0 ; i < fichierPosition.allKeys().size() ; i++)
    {
        Position *temp = new Position(fichierPosition.value(fichierPosition.allKeys().at(i), qVariantFromValue(Position())).value<Position>());
        this->listPositionsTemp->append(temp);
    }
}

void ManagerElements::removePositions(QList<Position*> *listPositionsToDelete)
{
    QSettings fichierPosition("position.ini", QSettings::IniFormat);
    for(int i = 0 ; i < listPositionsToDelete->size() ; i++)
    {
        fichierPosition.remove(QString(listPositionsToDelete->at(i)->getId()));
        fichierPosition.remove(QString(this->listPositionsTemp->last()->getId()));
        this->listPositionsTemp->last()->updateId(listPositionsToDelete->at(i)->getId());
        this->savePosition(this->listPositionsTemp->last(), fichierPosition);
        Position::idPositionsStatic--;
    }
    fichierPosition.sync();
}

/**
  *   FONCTIONS LOAD ET DELETE ET SAVE SAMPLEAUDIO
  **/
void ManagerElements::saveSampleAudio(SampleAudio *sampleAudio, QSettings &fichierSampleAudio)
{
    QString key = QString::number(sampleAudio->getId());
    fichierSampleAudio.setValue(key, qVariantFromValue(*sampleAudio));
}

void ManagerElements::loadSamplesAudios()
{
    this->listSamplesAudios = new QList<SampleAudio*>();

    QSettings fichierSampleAudio("sampleaudio.ini", QSettings::IniFormat);
    for(int i = 0 ; i < fichierSampleAudio.allKeys().length() ; i++)
    {
        SampleAudio *temp = new SampleAudio(fichierSampleAudio.value(fichierSampleAudio.allKeys().at(i), qVariantFromValue(SampleAudio())).value<SampleAudio>());
        this->listSamplesAudios->append(temp);
    }
}

void ManagerElements::removeSampleAudio(SampleAudio *sampleAudio)
{
    QSettings fichierSampleAudio("sampleaudio.ini", QSettings::IniFormat);
    fichierSampleAudio.remove(QString(sampleAudio->getId()));
    fichierSampleAudio.remove(QString(this->listSamplesAudios->last()->getId()));
    this->listSamplesAudios->last()->updateId(sampleAudio->getId());
    this->saveSampleAudio(this->listSamplesAudios->last(), fichierSampleAudio);
    SampleAudio::idSampleAudioStatic--;
    fichierSampleAudio.sync();
}

/**
  *   FONCTIONS LOAD ET DELETE ET SAVE SAMPLEVIDEO
  **/
void ManagerElements::saveSampleVideo(SampleVideo *sampleVideo, QSettings &fichierSampleVideo)
{
    QString key = QString::number(sampleVideo->getId());
    fichierSampleVideo.setValue(key, qVariantFromValue(*sampleVideo));
}

void ManagerElements::loadSamplesVideos()
{
    this->listSamplesVideos = new QList<SampleVideo*>();

    QSettings fichierSampleVideo("samplevideo.ini", QSettings::IniFormat);
    for(int i = 0 ; i < fichierSampleVideo.allKeys().length() ; i++)
    {
        SampleVideo *temp = new SampleVideo(fichierSampleVideo.value(fichierSampleVideo.allKeys().at(i), qVariantFromValue(SampleVideo())).value<SampleVideo>());
        this->listSamplesVideos->append(temp);
    }
}

void ManagerElements::removeSampleVideo(SampleVideo *sampleVideo)
{
    QSettings fichierSampleVideo("samplevideo.ini", QSettings::IniFormat);
    fichierSampleVideo.remove(QString(sampleVideo->getId()));
    fichierSampleVideo.remove(QString(this->listSamplesVideos->last()->getId()));
    this->listSamplesVideos->last()->updateId(sampleVideo->getId());
    this->saveSampleVideo(this->listSamplesVideos->last(), fichierSampleVideo);
    SampleVideo::idSampleVideoStatic--;
    fichierSampleVideo.sync();
}


//Initialisation du système pour la sérialisation
void ManagerElements::initSystem()
{
    qRegisterMetaTypeStreamOperators<Movement>("Movement");
    qMetaTypeId<Movement>();
    qRegisterMetaTypeStreamOperators<JointMvt>("JointMvt");
    qMetaTypeId<JointMvt>();
    qRegisterMetaTypeStreamOperators<Position>("Position");
    qMetaTypeId<Position>();
    qRegisterMetaTypeStreamOperators<SampleAudio>("SampleAudio");
    qMetaTypeId<SampleAudio>();
    qRegisterMetaTypeStreamOperators<SampleVideo>("SampleVideo");
    qMetaTypeId<SampleVideo>();
}

/**
  *    GETTERS
  **/

QList<Movement*>* ManagerElements::getListMovements()
{
    return this->listMovements;
}

QList<Movement*>* ManagerElements::getListMovementsActive()
{
    QList<Movement*>*listMovementActive = new QList<Movement*>();
    for(int i = 0 ; i < this->listMovements->size() ; i++)
	if(this->listMovements->at(i)->isActive() == true)
	    listMovementActive->append(this->listMovements->at(i));
    return listMovementActive;
}

QList<SampleAudio*>* ManagerElements::getListSamplesAudios()
{
    return this->listSamplesAudios;
}

QList<SampleAudio*>* ManagerElements::getListSamplesAudiosActive()
{
    QList<SampleAudio*>*listSampleAudioActive = new QList<SampleAudio*>();
    for(int i = 0 ; i < this->listSamplesAudios->size() ; i++)
	if(this->listSamplesAudios->at(i)->isActive() == true)
	    listSampleAudioActive->append(this->listSamplesAudios->at(i));
    return listSampleAudioActive;
}

QList<SampleVideo*>* ManagerElements::getListSamplesVideos()
{
    return this->listSamplesVideos;
}

QList<ClientOSC*>* ManagerElements::getListPorts()
{
    return this->listPorts;
}

QList<ClientOSC*>* ManagerElements::getListPortsActive()
{
    QList<ClientOSC*>*listPortsActive = new QList<ClientOSC*>();
    for(int i = 0 ; i < this->listPorts->size() ; i++)
	if(this->listPorts->at(i)->isActive() == true)
	    listPortsActive->append(this->listPorts->at(i));
    return listPortsActive;
}

QList<JointMvt*>* ManagerElements::getListJointsMvts()
{
    return this->listJointsMvts;
}

void ManagerElements::sortMovements()
{
    for(int i = 1 ; i < this->listMovements->size() ; i++)
    if(this->listMovements->at(i - 1)->getListJointsMvt()->at(0)->getListPositions()->size() >
       this->listMovements->at(i)->getListJointsMvt()->at(0)->getListPositions()->size())
        this->listMovements->swap(i, i - 1);
}

/**
  *    DESTRUCTEURS
  **/
ManagerElements::~ManagerElements()
{
    for(int i = 0 ; i < this->listMovements->size() ; i++)
        delete(this->listMovements->at(i));
    delete(this->listMovements);
    for(int i = 0 ; i < this->listSamplesAudios->size() ; i++)
        delete(this->listSamplesAudios->at(i));
    delete(this->listSamplesAudios);
    for(int i = 0 ; i < this->listSamplesVideos->size() ; i++)
        delete(this->listSamplesVideos->at(i));
    delete(this->listSamplesVideos);
    for(int i = 0 ; i < this->listPorts->size() ; i++)
        delete(this->listPorts->at(i));
    delete(this->listPorts);
}
