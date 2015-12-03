/**
  * Code in this file is licensed under BSD license. See LICENSE file for more information.
  */
#include "ChatServer.h"
#include <ChatMessage.h>
#include <QTcpServer>
#include <QTcpSocket>
ChatServer::ChatServer(quint16 port, QHostAddress host)
    : QObject(NULL)
    , m_listener(new QTcpServer())
{
    if (!m_listener->listen(host, port)){
        qWarning() << "Failed to listen on " << host.toString() << ":" << port << ", check settings.";
    }
    connect(m_listener, SIGNAL(newConnection()), this, SLOT(incomingConnection()));
}

ChatServer::~ChatServer()
{
    delete m_listener;
}

void ChatServer::incomingConnection()
{
    qDebug() << "ChatServer::incomingConnection()";
    QTcpSocket* client = m_listener->nextPendingConnection();
    connect(client, SIGNAL(readyRead()), this, SLOT(clientReadyRead()));
    connect(client, SIGNAL(disconnected()), this, SLOT(clientDisconnected()));
}

void ChatServer::clientReadyRead()
{
     QTcpSocket* client = dynamic_cast< QTcpSocket*>(sender());
     if (!client) return;
     ConnectionInfo& connection = m_clients[client];
     qDebug() << "ChatServer::clientReadyRead()";
     connection.m_clientBuffer += client->readAll();
     do {
         ChatMessage incomingMsg;
         ChatMessage::ReadResult result =  incomingMsg.readFromByteArray(connection.m_clientBuffer);
         qDebug() << "ChatServer::clientReadyRead() result=" << result;
         if (result == ChatMessage::rrIncomplete) {
             return;
         }
         if (result == ChatMessage::rrBroken) {
             connection.m_clientBuffer.clear();
             return;
         }
         connection.m_clientBuffer = connection.m_clientBuffer.mid(incomingMsg.fullMessageSize());

          qDebug() << "ChatServer::clientReadyRead() incomingMsg=" << incomingMsg.m_type ;
         // новое подключение, прислан логин.
         if (incomingMsg.m_type == ChatMessage::mtLogin) {
             if (getAllUserLogins().contains(incomingMsg.m_login)) {
                 client->disconnectFromHost();
                 return;
             }
             connection.m_login = incomingMsg.m_login;
             ChatMessage newUserMsg;
             newUserMsg.m_login = incomingMsg.m_login;
             newUserMsg.m_type = ChatMessage::mtPlainMessage;
             newUserMsg.m_text = "New user coming: " + incomingMsg.m_login;
             sendMessageToAll(newUserMsg);
             sendLoginsToAll();
         }
         if (incomingMsg.m_type == ChatMessage::mtPlainMessage) {
            sendMessageToAll(incomingMsg);
         }
     } while(true); // Продолжаем обрабатывать все возможные сообщения в буфере.
}

void ChatServer::clientDisconnected()
{
    qDebug() << "ChatServer::clientDisconnected()";
    QTcpSocket* client = dynamic_cast< QTcpSocket*>(sender());
    if (!client) return;
    QString userLogin = m_clients[client].m_login;
    m_clients.remove(client);

    ChatMessage exitUserMsg;
    exitUserMsg.m_login = userLogin;
    exitUserMsg.m_type = ChatMessage::mtPlainMessage;
    exitUserMsg.m_text = "User is leaving: " + userLogin;
    sendMessageToAll(exitUserMsg);
    sendLoginsToAll();
}

void ChatServer::sendLoginsToAll()
{
    ChatMessage loginsMessage;
    loginsMessage.m_type = ChatMessage::mtUserList;
    loginsMessage.m_userList = getAllUserLogins();
    qDebug() << "sendLoginsToAll " << loginsMessage.m_userList;
    sendMessageToAll(loginsMessage);
}

void ChatServer::sendMessageToAll(const ChatMessage &message)
{
    QByteArray outgoingBuffer;
    message.writeToByteArray(outgoingBuffer);
    foreach (QTcpSocket* socket, m_clients.keys()) {
        socket->write(outgoingBuffer);
    }
}

QStringList ChatServer::getAllUserLogins() const
{
    QStringList logins;
    foreach (const ConnectionInfo& info, m_clients) {
        if (! info.m_login.isEmpty()) {
            logins << info.m_login;
        }
    }
    return logins;
}
