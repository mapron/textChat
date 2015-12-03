/**
  * Code in this file is licensed under BSD license. See LICENSE file for more information.
  */
#include "ChatDialog.h"
#include <QInputDialog>
#include <QBoxLayout>
#include <QPushButton>
#include <QApplication>

ChatDialog::ChatDialog(ChatClient *client)
    : QDialog(NULL)
    , m_client(client)
{
    m_chatMessages = new QPlainTextEdit(this);
    m_chatMessages->setReadOnly(true);
    m_userList = new QListWidget(this);
    m_message = new QLineEdit(this);
    m_message->setMaxLength(256);
    QPushButton * doSend = new QPushButton(tr("Send"), this);

    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    QHBoxLayout* chatLogAndUsers = new QHBoxLayout();
    QHBoxLayout* messageLayout = new QHBoxLayout();
    mainLayout->addLayout(chatLogAndUsers);
    mainLayout->addLayout(messageLayout);
    chatLogAndUsers->addWidget(m_chatMessages);
    chatLogAndUsers->addWidget(m_userList);
    messageLayout->addWidget(m_message);
    messageLayout->addWidget(doSend);

    connect(m_client, SIGNAL(incomingMessage(ChatMessage)), this, SLOT(incomingMessage(ChatMessage)));
    connect(this,     SIGNAL(sendMessage(ChatMessage)), m_client, SLOT(sendMessage(ChatMessage)));
    connect(m_client, SIGNAL(socketConnected()),            this, SLOT(sendLoginToServer()));

    connect(doSend,     SIGNAL(clicked()),       this, SLOT(sendMessageText()));
    connect(m_message,  SIGNAL(returnPressed()), this, SLOT(sendMessageText()));
    connect(m_userList, SIGNAL(itemDoubleClicked(QListWidgetItem *)),
            this,       SLOT  (itemDoubleClicked(QListWidgetItem *)));

    requestUserLogin();
}

ChatDialog::~ChatDialog()
{

}

void ChatDialog::incomingMessage(ChatMessage message)
{
    if (message.m_type == ChatMessage::mtPlainMessage) {
        const QString logLine = message.m_login + "> " + message.m_text;
        m_chatMessages->appendPlainText(logLine);
    }
    if (message.m_type == ChatMessage::mtUserList) {
        m_userList->clear();
        foreach (const QString &login, message.m_userList) {
            m_userList->addItem(login);
        }
    }
}

void ChatDialog::requestUserLogin()
{
    static QRegExp loginRE("\\w{1,64}");

    do {
        m_login = QInputDialog::getText(this, tr("Server login"), tr("Enter yout nickname (a-zA-Z0-9)*:"), QLineEdit::Normal, "guest");
        if (loginRE.exactMatch(m_login)) break;
        if (m_login.isEmpty())  {
            qApp->quit();
            return;
        }
    }while (true);    

    sendLoginToServer();
}

void ChatDialog::sendLoginToServer()
{
    if (m_login.isEmpty()) return;
    ChatMessage loginMsg;
    loginMsg.m_type = ChatMessage::mtLogin;
    loginMsg.m_login = m_login;
    emit sendMessage(loginMsg);
}

void ChatDialog::sendMessageText()
{
    const QString userMessage = m_message->text().trimmed();
    if (userMessage.isEmpty()) return;
    m_message->clear();
    ChatMessage textMsg;
    textMsg.m_type = ChatMessage::mtPlainMessage;
    textMsg.m_login = m_login;
    textMsg.m_text = userMessage;
    emit sendMessage(textMsg);
}

void ChatDialog::itemDoubleClicked(QListWidgetItem *item)
{
    QString text = m_message->text();
    text.append("#" + item->text() + ": ");
    m_message->setText(text);
}
