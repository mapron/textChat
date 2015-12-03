/**
  * Code in this file is licensed under BSD license. See LICENSE file for more information.
  */
#include <ChatDialog.h>
#include <QApplication>
#include <NetworkSettings.h>
int main(int argc, char* argv[])
{
    QApplication app(argc, argv);
    NetworkSettings settings("chatClient");

    ChatClient chatClient(settings.m_tcpPort, settings.m_targetHost);
    ChatDialog chatDialog(&chatClient);
    chatDialog.show();
    return app.exec();
}
