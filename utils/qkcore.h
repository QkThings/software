#ifndef QKCORE_H
#define QKCORE_H

#include <QObject>

namespace Qk
{

const char PROTOCOL_BEP = 0x55;
const char PROTOCOL_DLE = 0xDD;

const quint16 PACKET_FLAG_FRAG = 0x0004;
const quint16 PACKET_FLAG_LASTFRAG = 0x0002;

const quint8 PACKET_CODE_OK = 0x01;
const quint8 PACKET_CODE_ACK = 0x03;
const quint8 PACKET_CODE_ERR = 0xFF;
const quint8 PACKET_CODE_TIMEOUT = 0xFE;
const quint8 PACKET_CODE_SAVE = 0x04;
const quint8 PACKET_CODE_RESTORE = 0x05;
const quint8 PACKET_CODE_SEARCH = 0x06;
const quint8 PACKET_CODE_START = 0x0A;
const quint8 PACKET_CODE_STOP = 0x0F;
const quint8 PACKET_CODE_READY = 0x0D;
const quint8 PACKET_CODE_GETNODE = 0x10;
const quint8 PACKET_CODE_SETNAME = 0x34;
const quint8 PACKET_CODE_SETCONFIG = 0x3C;
const quint8 PACKET_CODE_SETSAMP = 0x36;
const quint8 PACKET_CODE_ACTION = 0x35;
const quint8 PACKET_CODE_INFOQK = 0xB1;
const quint8 PACKET_CODE_INFOSAMP = 0xB2;
const quint8 PACKET_CODE_INFOBOARD = 0xB5;
const quint8 PACKET_CODE_INFOCOMM = 0xB6;
const quint8 PACKET_CODE_INFODEVICE = 0xB7;
const quint8 PACKET_CODE_INFOACTION = 0xBA;
const quint8 PACKET_CODE_INFODATA = 0xBD;
const quint8 PACKET_CODE_INFOEVENT = 0xBE;
const quint8 PACKET_CODE_INFOCONFIG = 0xBC;
const quint8 PACKET_CODE_STATUS = 0xD5;
const quint8 PACKET_CODE_DATA = 0xD0;
const quint8 PACKET_CODE_EVENT = 0xDE;
const quint8 PACKET_CODE_STRING = 0xDF;

class Frame : public QObject
{
    Q_OBJECT
public:
    static int flags(QByteArray frame, bool raw);
    static QByteArray join(QList<QByteArray> fragments, bool raw);

};

class Packet : public QObject
{
    Q_OBJECT
public:
    explicit Packet(QObject *parent = 0);

    static void fromFrame(QByteArray frame, bool raw, Packet *packet);
    static QString friendlyName(int code);

    int flags() { return _flags; }
    int id() { return _id; }
    int code() { return _code; }
    QByteArray payload() { return _payload; }

private:
    int _flags;
    int _id;
    int _code;
    QByteArray _payload;
};

class Protocol : public QObject
{
    Q_OBJECT
public:
    explicit Protocol(QObject *parent = 0);

public slots:
    void init();
    void parseData(QByteArray data, bool raw = false);

signals:
    void parsedFrame(QByteArray,bool);

private:
    QByteArray _frame;
    QByteArray _rawFrame;
    bool _receiving;
    bool _escape;
    bool _valid;
    QList<QByteArray> _fragments;

};

}

#endif // QKCORE_H
