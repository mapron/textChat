/**
  * Code in this file is licensed under BSD license. See LICENSE file for more information.
  */
#include <ChatMessage.h>

// Тестирование сообщений канальго уровня.
int main()
{
    ChatMessage in, out;
    QByteArray dataBuffer;
    // Логин на сервере
    in.m_login = "testLogin";
    in.writeToByteArray(dataBuffer);
    ChatMessage::ReadResult result = out.readFromByteArray(dataBuffer);
    Q_ASSERT(result == ChatMessage::rrOk);
    Q_ASSERT(in == out);

    // Простое сообщение
    in.m_type = ChatMessage::mtPlainMessage;
    in.m_text = "Hello, World!";
    //in.m_userList = QStringList() << "user1" << "user2";
    in.writeToByteArray(dataBuffer);
    out.readFromByteArray(dataBuffer);
    Q_ASSERT(in == out);

    // Список пользователей
    in.m_type = ChatMessage::mtUserList;
    in.m_userList = QStringList() << "user1" << "user2";
    in.writeToByteArray(dataBuffer);
    out.readFromByteArray(dataBuffer);
    Q_ASSERT(in.m_userList == out.m_userList);

    // Неполное сообщение - 1
    dataBuffer = QString("foo") .toUtf8();
    result = out.readFromByteArray(dataBuffer);
    Q_ASSERT(result == ChatMessage::rrIncomplete);

    // Неполное сообщение - 2
    dataBuffer.resize(5);
    dataBuffer[0]=0;
    dataBuffer[1]=0;
    dataBuffer[2]=0;
    dataBuffer[3]=5;
    result = out.readFromByteArray(dataBuffer);
    Q_ASSERT(result == ChatMessage::rrIncomplete);

    // Неверно сформированное сообщение.
    dataBuffer.resize(8);
    dataBuffer[3]=1;
    dataBuffer[18]=1;
    result = out.readFromByteArray(dataBuffer);
    Q_ASSERT(result == ChatMessage::rrBroken);

}
