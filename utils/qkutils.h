/*
 * QkThings LICENSE
 * The open source framework and modular platform for smart devices.
 * Copyright (C) 2014 <http://qkthings.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef QKUTILS_H
#define QKUTILS_H

#include <QObject>
#include <QString>
#include <QList>
#include <QJsonDocument>
#include <QMap>
#include <stdint.h>

#define MASK(bit, shift) (((1<<bit)-1) << shift)
#define FLAG_SET(val, mask) (val |= mask)
#define FLAG_CLR(val, mask) (val &= ~mask)
#define FLAG(val, mask)     ((val & mask) ? true : false)

namespace QkUtils
{
class Version
{
public:
    enum StringFormat
    {
        sfDec,
        sfHex
    };
    Version(int major = 0, int minor = 0, int patch = 0)
    {
        m_major = major;
        m_minor = minor;
        m_patch = patch;
    }
    QString toString(StringFormat sf)
    {
        switch(sf)
        {
        case sfDec: return QString().sprintf("%d.%d.%02d", m_major, m_minor, m_patch);
        case sfHex: return QString().sprintf("%1X%1X%02X", m_major, m_minor, m_patch);
        }
    }
private:
    int m_major, m_minor, m_patch;
};

class Target
{
public:
    class Board
    {
    public:
        QString name;
    };

    QString name;
    QString toolchainUrl;
    QList<Board> boards;
};

class JsonParser : public QObject
{
    Q_OBJECT
public:
    explicit JsonParser(QObject *parent =0);
    void parseData(QByteArray data);

signals:
    void parsed(QJsonDocument);

private:
    QByteArray _jsonStr;
    int _depthLevel;
    bool _inString;
    bool _escChar;

};

typedef QList<QkUtils::Target> TargetList;
typedef QList<QkUtils::Target::Board> TargetVariantList;

QMap<QString, Target> supportedTargets(const QString &embPath);

extern QString _infoPath;
QJsonDocument jsonFromFile(const QString &filePath);
bool validateJson(const QJsonDocument &doc);

union IntFloatConverter {
    float f_value;
    int i_value;
    uint8_t bytes[4];
};
void fillValue(int value, int count, int *idx, QByteArray &data);
void fillString(const QString &str, int count, int *idx, QByteArray &data);
void fillString(const QString &str, int *idx, QByteArray &data);
int getValue(int count, int *idx, const QByteArray &data, bool sigExt = false);
QString getString(int *idx, const QByteArray &data);
QString getString(int count, int *idx, const QByteArray &data);
float floatFromBytes(int value);
int bytesFromFloat(float value);
}

#endif // QKUTILS_H
