/**
  * Code in this file is licensed under BSD license. See LICENSE file for more information.
  */
#include "NetworkSettings.h"
#include <QSettings>
#define DEFAULT_CHAT_PORT 7777
#define DEFAULT_CHAT_HOST "127.0.0.1"

NetworkSettings::NetworkSettings(const QString &baseFilename)
    : m_settings(new QSettings(baseFilename + ".ini", QSettings::IniFormat))
    , m_tcpPort(m_settings->value("tcpPort", DEFAULT_CHAT_PORT).toUInt())
    , m_targetHost(m_settings->value("host", DEFAULT_CHAT_HOST).toString())

{
}
