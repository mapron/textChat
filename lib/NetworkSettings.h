/**
  * Code in this file is licensed under BSD license. See LICENSE file for more information.
  */
#pragma once
#ifndef NETWORK_SETTINGS_H
#define NETWORK_SETTINGS_H
#include <QString>
#include <QHostAddress>
#include <QSharedPointer>

class QSettings;
/**
 * \brief сетевые настройки для чата.
 *
 * При создании объекта, происходит чтение из объекта QSettings (например, ini-файла).
 *
 * Из настроек считываются парметр TCP-порта и хоста, к которому проводить подключение.
 *
 * По умолчанию используются  7777 TCP-порт и localhost.
 */
class NetworkSettings
{
    QSharedPointer<QSettings> m_settings;
public:
    NetworkSettings(const QString& baseFilename);

    const quint16 m_tcpPort;
    const QHostAddress m_targetHost;
};

#endif
