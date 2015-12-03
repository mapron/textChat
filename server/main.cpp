#include <ChatServer.h>
#include <QApplication>
#include <NetworkSettings.h>
int main(int argc, char* argv[])
{
    QApplication app(argc, argv);
    NetworkSettings settings("chatServer");

    ChatServer chatServer(settings.m_tcpPort, settings.m_targetHost);

    return app.exec();
}
