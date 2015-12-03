/**
  * Code in this file is licensed under BSD license. See LICENSE file for more information.
  */
#pragma once
#ifndef CHAT_CLIENT_H
#define CHAT_CLIENT_H

#include <QHostAddress>
#include <QObject>
#include <ChatMessage.h>
class QTcpSocket;

/**
 * \brief Клиент чата
 *
 * Подключается по TCP к указанными в конструкторе port и host.
 *
 * Для получения обработанных сообщений, нужно сделать connect на сигнал incomingMessage();
 * для отправлки сообщений на сервер, connect на slot sendMessage.
 *
 * При разрыве соединения, новое будет установлено автоматически.
 */
class ChatClient : public QObject
{
    Q_OBJECT
public:
    ChatClient(quint16 port, QHostAddress host);

    ~ChatClient();

signals:
    void incomingMessage(ChatMessage message);
    void socketConnected();

protected slots:
    void connectToServer();
    void newDataAvailable();
    void sendMessage(ChatMessage message);

protected:
    void timerEvent(QTimerEvent*);
private:
    QTcpSocket * const m_socket;
    const quint16 m_port;
    const QHostAddress m_host;
    QByteArray m_incomingBuffer;
};

#endif
