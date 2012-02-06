#include "sampleaudio.h"
quint32 SampleAudio::idSampleAudioStatic = 0;

SampleAudio::SampleAudio() : Sample()
{
}

SampleAudio::SampleAudio(const QString &name, const QString &url, const quint32 &idMovement) : Sample(++idSampleAudioStatic, name, url, idMovement)
{
}

SampleAudio::SampleAudio(const SampleAudio &copie) : Sample(copie.idElement, copie.nameElement, copie.fileURL, copie.idMovement)
{
}

//save
QDataStream & operator << (QDataStream & out, const SampleAudio &valeur)
{
    //std::cout << "Entrée operator << SampleAudio" << std::endl;
    out << valeur.idElement;
    out << valeur.idMovement;
    out << valeur.nameElement;
    out << valeur.fileURL;
    return out;
}

//load
QDataStream & operator >> (QDataStream & in, SampleAudio &valeur)
{
    //std::cout << "Entrée operator >> SampleAudio" << std::endl;
    in >> valeur.idElement;
    in >> valeur.idMovement;
    in >> valeur.nameElement;
    in >> valeur.fileURL;
    return in;
}
