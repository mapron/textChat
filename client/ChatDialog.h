/**
  * Code in this file is licensed under BSD license. See LICENSE file for more information.
  */
#pragma once
#ifndef CHAT_DIALOG_H
#define CHAT_DIALOG_H

#include <QDialog>
#include <ChatMessage.h>
#include <ChatClient.h>
#include <QListWidget>
#include <QPlainTextEdit>
#include <QLineEdit>

/**
 * \brief Диалоговое окно чата.
 *
 * В нем отображается список сообщений,
 * список пользователей в чате, а также поле ввода для отправки сообщений.
 */
class ChatDialog : public QDialog
{
    Q_OBJECT
public:
    ChatDialog(ChatClient* client);
    ~ChatDialog();

signals:
    void sendMessage(ChatMessage message);

protected slots:
    void incomingMessage(ChatMessage message);    
    void requestUserLogin();                     //!< Запрос логина для сервера.
    void sendLoginToServer();
    void sendMessageText();
    void itemDoubleClicked(QListWidgetItem *item);

private:
    ChatClient* const m_client;
    QPlainTextEdit* m_chatMessages;
    QListWidget* m_userList;
    QLineEdit* m_message;
    QString m_login;
};

#endif
