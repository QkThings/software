#include "qkcore.h"
#include "qkutils.h"

#include <QDebug>

using namespace Qk;
using namespace QkUtils;

int Frame::flags(QByteArray frame, bool raw)
{
    int flags = 0;
    int i=0;
    if(raw)
        i++;

    flags = (frame[i] & 0xFF) +
            ((frame[i+1] & 0xFF) << 8);

    return flags;
}

QByteArray Frame::join(QList<QByteArray> fragments, bool raw)
{
    Q_ASSERT(fragments.count() > 0);
    QByteArray frame;

    frame += fragments.takeFirst();
    if(raw)
    {
        Q_ASSERT(frame.count() >= 2);
        frame = frame.mid(1, frame.count()-2);
    }

    while(!fragments.isEmpty())
    {
        QByteArray fragment = fragments.takeFirst();
        int headerByteCount = 4;
        if(raw)
        {
            Q_ASSERT(fragment.count() >= 2);
            fragment = fragment.mid(1, fragment.count()-2);
        }

        frame += fragment.right(fragment.count() - headerByteCount);
    }

    if(raw)
    {
        frame.prepend(Qk::PROTOCOL_BEP);
        frame.append(Qk::PROTOCOL_BEP);
    }

    return frame;
}

Packet::Packet(QObject *parent) :
    QObject(parent)
{

}

void Packet::fromFrame(QByteArray frame, bool raw, Packet *packet)
{
    int i_data = 0;

    if(raw)
    {
        frame = frame.mid(1, frame.count()-2);
    }

    packet->_flags = getValue(2, &i_data, frame);
    packet->_id = getValue(1, &i_data, frame);
    packet->_code = getValue(1, &i_data, frame);

    packet->_payload = frame.right(frame.count() - i_data);
}

QString Packet::friendlyName(int code)
{
    switch((quint8)code)
    {
    case PACKET_CODE_READY:
        return "READY";
    case PACKET_CODE_OK:
        return "OK";
    case PACKET_CODE_ERR:
        return "ERR";
    case PACKET_CODE_ACK:
        return "ACK";
    case PACKET_CODE_SEARCH:
        return "SEARCH";
    case PACKET_CODE_GETNODE:
        return "GET_NODE";
    case PACKET_CODE_SETNAME:
        return "SET_NAME";
    case PACKET_CODE_SETCONFIG:
        return "SET_CONFIG";
    case PACKET_CODE_SETSAMP:
        return "SET_SAMP";
    case PACKET_CODE_INFOQK:
        return "INFO_QK";
    case PACKET_CODE_INFOSAMP:
        return "INFO_SAMP";
    case PACKET_CODE_INFOBOARD:
        return "INFO_BOARD";
    case PACKET_CODE_INFOCOMM:
        return "INFO_MODULE";
    case PACKET_CODE_INFODEVICE:
        return "INFO_DEVICE";
    case PACKET_CODE_INFODATA:
        return "INFO_DATA";
    case PACKET_CODE_INFOEVENT:
        return "INFO_EVENT";
    case PACKET_CODE_INFOACTION:
        return "INFO_ACTION";
    case PACKET_CODE_INFOCONFIG:
        return "INFO_CONFIG";
    case PACKET_CODE_DATA:
        return "DATA";
    case PACKET_CODE_EVENT:
        return "EVENT";
    case PACKET_CODE_STATUS:
        return "STATUS";
    case PACKET_CODE_START:
        return "START";
    case PACKET_CODE_STOP:
        return "STOP";
    case PACKET_CODE_STRING:
        return "STRING";
    case PACKET_CODE_TIMEOUT:
        return "TIMEOUT";
    default:
        return QString().sprintf("UNKNOWN(%02X)", code);
    }
}

Protocol::Protocol(QObject *parent) :
    QObject(parent)
{
    init();
}

void Protocol::init()
{
    _receiving = false;
    _escape = false;
    _valid = false;
    _frame.clear();
}

void Protocol::parseData(QByteArray data, bool raw)
{
//    qDebug() << __PRETTY_FUNCTION__;
    int i;
    char *p_data = data.data();

    for(i = 0; i < data.count(); i++, p_data+=1)
    {
//        qDebug("rx: %02X", *p_data & 0xFF);

        if(*p_data == PROTOCOL_BEP)
        {
            if(raw)
                _frame.append(*p_data);
            if(!_escape)
            {
                if(!_receiving)
                {
                    _receiving = true;
                    _valid = true;
                }
                else
                {
                    if(_valid && _frame.count() > 0)
                    {
                        emit parsedFrame(_frame, raw);
                        _frame.clear();
                        _receiving = false;
                        _valid = false;
                    }
                }
                continue;
            }
        }
        if(*p_data == PROTOCOL_DLE)
        {
            if(_valid)
            {
                if(raw)
                    _frame.append(*p_data);
                if(!_escape)
                {
                    _escape = true;
                    continue;
                }
            }
        }

        if(_valid)
            _frame.append(*p_data);

        _escape = false;
    }

}
