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

#include "qkutils.h"

#include <QDebug>
#include <QApplication>
#include <QStyleFactory>
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

using namespace QkUtils;

QMap<QString, Target> QkUtils::supportedTargets(const QString &embPath)
{
    QMap<QString, Target>  targets;

    QString filePath = embPath + "/target/targets.json";
    QJsonDocument doc = jsonFromFile(filePath);

    QVariantMap jsonTargets = doc.object().toVariantMap();

    foreach(QString targetName, jsonTargets.keys())
    {
        Target target;

        if(targetName == "version")
            continue;

        target.name = targetName;
        QVariantMap jsonTargetInfo = jsonTargets[targetName].toMap();

        target.toolchainUrl = jsonTargetInfo["toolchainUrl"].toString();

        QVariantList jsonTargetVariants = jsonTargetInfo["variants"].toList();
        TargetVariantList targetVariants;

        foreach(QVariant jsonVariant, jsonTargetVariants)
        {
            QMap<QString, QVariant> variant = jsonVariant.toMap();
            Target::Board targetVariant;

            targetVariant.name = variant["name"].toString();

            targetVariants.append(targetVariant);
        }
        target.boards.append(targetVariants);
        targets.insert(targetName, target);
    }

    return targets;
}

QJsonDocument QkUtils::jsonFromFile(const QString &filePath)
{
    QString jsonFilePath = filePath;
    QJsonDocument doc;

    QFile file(jsonFilePath);
    if(!file.open(QFile::ReadOnly | QFile::Text))
    {
        qDebug() << "failed to open file" << jsonFilePath;
    }
    else
    {
        QString jsonStr = QString(file.readAll());
        jsonStr.remove('\t');
        jsonStr.remove(' ');

        QJsonParseError parseError;
        doc = QJsonDocument::fromJson(jsonStr.toUtf8(), &parseError);

        if(parseError.error != QJsonParseError::NoError)
            qDebug() << "json parse error:" << parseError.errorString();
    }

    return doc;
}

void QkUtils::fillValue(int value, int count, int *idx, QByteArray &data)
{
    int i, j = *idx;
    for(i = 0; i < count; i++, j++)
    {
        data.insert(j, (value >> 8*i) & 0xFF);
    }
    *idx = j;
}

void QkUtils::fillString(const QString &str, int count, int *idx, QByteArray &data)
{
    QString justifiedStr = str.leftJustified(count, '\0', true);
    fillString(justifiedStr, idx, data);
}

void QkUtils::fillString(const QString &str, int *idx, QByteArray &data)
{
    int j = *idx;
    data.insert(j, str);
    j += str.length()+1;
}

int QkUtils::getValue(int count, int *idx, const QByteArray &data, bool sigExt)
{
    int j, value = 0;
    int i = *idx;

    for(j = 0; j < count; j++, i++)
    {
        if(i < data.count())
            value += (data.at(i) & 0xFF) << (8*j); // little endian
    }

    switch(count) // truncate and extend sign bit
    {
    case 1:
        value &= 0xFF;
        if(sigExt & ((value & 0x80) > 0))
            value |= 0xFFFFFF00;
        break;
    case 2:
        value &= 0xFFFF;
        if(sigExt & ((value & 0x8000) > 0))
            value |= 0xFFFF0000;
        break;
    case 4:
        value &= 0xFFFFFFFF;
        break;
    }

    *idx = i;
    return value;
}

QString QkUtils::getString(int *idx, const QByteArray &data)
{
    int j;
    char c, buf[1024];
    int i = *idx;

    memset(buf, '\0', sizeof(buf));

    for(j=0; j < 1024; j++)
    {
        if(j+1 < data.length())
        {
            c = (char)((quint8)data[i++]);
            if(c == '\0')
                break;
            if((c < 32 || c > 126))
                c = ' ';
            buf[j] = c;
        }
        else
            break;
    }

    *idx = i;
    return QString(buf);
}

QString QkUtils::getString(int count, int *idx, const QByteArray &data)
{
    int j;
    char c, buf[count+1];
    int i = *idx;

    memset(buf, '\0', sizeof(buf));

    for(j=0; j < count; j++)
    {
        if(j+1 < data.length())
        {
            c = (char)((quint8)data[i++]);
            if((c < 32 || c > 126) && c != '\0')
                c = ' ';
            buf[j] = c;
        }
        else
            break;
    }
    buf[count] = '\0';
    *idx = i;
    return QString(buf);
}

float QkUtils::floatFromBytes(int value)
{
    IntFloatConverter converter;
    converter.i_value = value;
    return converter.f_value;
}

int QkUtils::bytesFromFloat(float value)
{
    IntFloatConverter converter;
    converter.f_value = value;

    return converter.i_value;
}

