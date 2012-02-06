#include "port.h"

quint32 Port::idPortStatic = 0;

Port::Port() {
}

Port::Port(const quint16 &portNb) : Element(++idPortStatic, QString::number(portNb), false)
{
    this->portNumber = portNb;
}

void Port::setPortNumber(const quint16 &pn) {
    this->portNumber = pn;
}

quint16 Port::getPortNumber() {
    return portNumber;
}

const char* Port::className() {
    return "Port";
}
