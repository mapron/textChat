/**
  * Code in this file is licensed under BSD license. See LICENSE file for more information.
  */
#include "ChatClient.h"
#include <QTcpSocket>


ChatClient::ChatClient(quint16 port, QHostAddress host)
    : QObject(NULL)
    , m_socket(new QTcpSocket(this))
    , m_port(port)
    , m_host(host)
{
    connect(m_socket, SIGNAL(readyRead()), this, SLOT(newDataAvailable()));
    connect(m_socket, SIGNAL(connected()), this, SIGNAL(socketConnected()));
    startTimer(200);
}


ChatClient::~ChatClient()
{
    disconnect(m_socket, NULL, this, NULL);
}

void ChatClient::connectToServer()
{
    m_socket->connectToHost(m_host, m_port);
}

void ChatClient::newDataAvailable()
{
    m_incomingBuffer.append(m_socket->readAll());
    do {
        ChatMessage msg;
        ChatMessage::ReadResult result =  msg.readFromByteArray(m_incomingBuffer);
        if (result == ChatMessage::rrIncomplete) {
            return;
        }

        if (result == ChatMessage::rrBroken) {
            m_incomingBuffer.clear();
            return;
        }
        m_incomingBuffer = m_incomingBuffer.mid(msg.fullMessageSize());
        qDebug() << "ChatClient::newDataAvailable" << msg.m_type;

        emit incomingMessage(msg);
    }while(true); // обработаем все возможные входные сообщения.
}

void ChatClient::sendMessage(ChatMessage message)
{
    if (m_socket->state() != QTcpSocket::ConnectedState) {
        return;
    }
    QByteArray outgoingBuffer;
    message.writeToByteArray(outgoingBuffer);
    m_socket->write(outgoingBuffer);
}

void ChatClient::timerEvent(QTimerEvent *)
{
    if (m_socket->state() == QTcpSocket::UnconnectedState) {
        connectToServer();
    }
}

