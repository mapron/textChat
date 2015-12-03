/**
  * Code in this file is licensed under BSD license. See LICENSE file for more information.
  */
#pragma once
#ifndef CHAT_MESSAGE_H
#define CHAT_MESSAGE_H


#include <QIODevice>
#include <QStringList>
#include <QMetaObject>

/**
 * \brief Сообщение канального уровня между клиентом и сервером чата.
 *
 * Существуют следующие типы сообщений:
 * -Авторизация на сервере (логин) (клиент->сервер)
 * -Текстовое сообщение в чате (клиент->сервер) и (сервер->клиент)
 * -Список пользователей чата (сервер->клиент)
 */
class ChatMessage
{
public:
    enum MessageType { mtLogin, mtPlainMessage, mtUserList, mtCOUNT };
    MessageType m_type;
    QString m_login;
    QString m_text;
    QStringList m_userList;
    ChatMessage();
    enum ReadResult { rrOk, rrIncomplete, rrBroken };

    void writeToByteArray(QByteArray& dataBuffer) const;        //!< Запись в буфер
    ReadResult readFromByteArray(const QByteArray& dataBuffer); //!< Чтение из буфера. В случае, если данных недостаточно, результатом будет rrIncomplete, при повреждении пакета - rrBroken.
    int fullMessageSize() const;                                //!< Полный размер в байтах этого сообщения. Нужен для вычисления смещения для следующего сообщения.
    bool operator == (const ChatMessage& another) const;
};


#endif
