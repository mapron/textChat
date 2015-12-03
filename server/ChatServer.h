/**
  * Code in this file is licensed under BSD license. See LICENSE file for more information.
  */
#pragma once
#ifndef CHAT_SERVER_H
#define CHAT_SERVER_H

#include <QHostAddress>
#include <QObject>
#include <QMap>

class QTcpServer;
class QTcpSocket;
class ChatMessage;
/**
 * \brief Сервер чата.
 *
 * Открывает TCP порт, слушает входящие соединения. При получении сообщений от клиентов, пересылает их остальным.
 * При установлении нового подключения, также отсылает всем клиентам обновленный список клиентов.
 * При разрыве соединения, удаляет login клиента из своего списка.
 *
 */
class ChatServer : public QObject
{
    QTcpServer * const m_listener;
    struct ConnectionInfo {             //!< Структура с информацией о каждом клиенте - его логин и прочитанный буфер данных.
        QString m_login;
        QByteArray m_clientBuffer;
    };
    QMap<QTcpSocket*, ConnectionInfo> m_clients;    //!< Список клиентов, индексированный по TCP-сокету.

    Q_OBJECT
public:
    ChatServer(quint16 port, QHostAddress host);
    ~ChatServer();

protected slots:
    void incomingConnection();  //!< Установленно новое TCP-подключение.
    void clientReadyRead();     //!< От клиента пришла новая порция данных.
    void clientDisconnected();  //!< Клиент отключился.
    void sendLoginsToAll();                             //!< Отправить всем клиентам обновленный список логинов.
    void sendMessageToAll(const ChatMessage& message);  //!< Отправка абстрактного сообщения канального уровня для всех.

protected:
    QStringList getAllUserLogins() const;
};

#endif
