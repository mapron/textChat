/**
  * Code in this file is licensed under BSD license. See LICENSE file for more information.
  */
#include "ChatMessage.h"
#include <QDataStream>
#include <QRegExp>
#include <stdint.h>
static QRegExp loginRE("\\w{1,64}");


ChatMessage::ChatMessage()
    : m_type(mtLogin)
{

}

void ChatMessage::writeToByteArray(QByteArray &dataBuffer) const
{
    dataBuffer.clear();
    QDataStream stream(&dataBuffer, QIODevice::WriteOnly);
    uint8_t type = m_type;
    stream << type;
    switch (m_type) {
        case mtLogin:
            stream << m_login;
        break;
        case mtPlainMessage:
            stream << m_login << m_text;
        break;
        case mtUserList:
            stream << m_userList;
        break;
        case mtCOUNT:
        break;
            // default пропущен, чтобы иметь warning при изменении MessageType
    }
    QByteArray lengthBytes;
    QDataStream streamLength(&lengthBytes, QIODevice::WriteOnly);
    streamLength << qint32( dataBuffer.size() );
    dataBuffer = lengthBytes + dataBuffer;

}

ChatMessage::ReadResult ChatMessage::readFromByteArray(const QByteArray &dataBuffer)
{
    if (dataBuffer.size() < (int)sizeof(qint32) ) {
        return rrIncomplete;
    }
    qint32 size;
    QDataStream stream(dataBuffer);
    stream >> size;
    if (dataBuffer.size() < (int)(size + sizeof(qint32))) {
        return rrIncomplete;
    }
    uint8_t type;
    stream >> type;
    if (type >= mtCOUNT) {
        return rrBroken;
    }
    m_type = MessageType(type);

    switch (m_type) {
        case mtLogin:
            stream >> m_login;
        break;
        case mtPlainMessage:
            stream >> m_login >> m_text;
        break;
        case mtUserList:
            stream >> m_userList;
        break;
        case mtCOUNT:
        break;
    }
    if (m_type == mtLogin || m_type == mtPlainMessage) {
        if (!loginRE.exactMatch(m_login)) {
            return rrBroken;
        }
    }

    return rrOk;
}

int ChatMessage::fullMessageSize() const
{
    QByteArray data;
    this->writeToByteArray(data); //FIXME: неэффективный способ определения размера.
    return data.size();
}

bool ChatMessage::operator ==(const ChatMessage &another) const
{
    return  m_type      == another.m_type
         && m_login     == another.m_login
         && m_text      == another.m_text
         && m_userList  == another.m_userList
            ;
}
