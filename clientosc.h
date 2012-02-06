#ifndef CLIENTOSC_H
#define CLIENTOSC_H

#include "port.h"
#include "outputable.h"
#include "databitg.h"
#include "messagesynapse.h"

class ClientOSC : public Port, public Outputable
{
public:
    ClientOSC();
    ClientOSC(const quint16 &portNb, const QString &host);
    void setHost(const QString &h);
    void setDataBITG(DataBITG* d);
    void setMsgSynapse(QList<MessageSynapse*>* messages);

    bool sendDataBITG();
    bool sendMsgSynapse();
    bool send(QString *host);

    virtual const char* className();
    virtual void run();

private:
    QList<MessageSynapse*>* msg;
    DataBITG *datas;
    QString host;
};

#endif // CLIENTOSC_H
