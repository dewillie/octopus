#include "samplevideo.h"

quint32 SampleVideo::idSampleVideoStatic = 0;

SampleVideo::SampleVideo() : Sample()
{

}

SampleVideo::SampleVideo(const QString &name, const QString &url, const quint32 &idMovement) : Sample(++idSampleVideoStatic, name, url, idMovement)
{
}


SampleVideo::SampleVideo(const SampleVideo &copie) : Sample(copie.idElement, copie.nameElement, copie.fileURL, copie.idMovement)
{
}

//save
QDataStream & operator << (QDataStream & out, const SampleVideo &valeur)
{
    //std::cout << "Entrée operator << SampleVideo" << std::endl;
    out << valeur.idElement;
    out << valeur.idMovement;
    out << valeur.nameElement;
    out << valeur.fileURL;
    return out;
}

//load
QDataStream & operator >> (QDataStream & in, SampleVideo &valeur)
{
    //std::cout << "Entrée operator >> SampleVideo" << std::endl;
    in >> valeur.idElement;
    in >> valeur.idMovement;
    in >> valeur.nameElement;
    in >> valeur.fileURL;
    return in;
}
