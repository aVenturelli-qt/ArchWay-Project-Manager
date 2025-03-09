/****************************************************************************
**
** Copyright (C) 2012 Nokia Corporation and/or its subsidiary(-ies).
** Contact: http://www.qt-project.org/
**
** This file is part of the QtSql module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL$
** GNU Lesser General Public License Usage
** This file may be used under the terms of the GNU Lesser General Public
** License version 2.1 as published by the Free Software Foundation and
** appearing in the file LICENSE.LGPL included in the packaging of this
** file. Please review the following information to ensure the GNU Lesser
** General Public License version 2.1 requirements will be met:
** http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** In addition, as a special exception, Nokia gives you certain additional
** rights. These rights are described in the Nokia Qt LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU General
** Public License version 3.0 as published by the Free Software Foundation
** and appearing in the file LICENSE.GPL included in the packaging of this
** file. Please review the following information to ensure the GNU General
** Public License version 3.0 requirements will be met:
** http://www.gnu.org/copyleft/gpl.html.
**
** Other Usage
** Alternatively, this file may be used in accordance with the terms and
** conditions contained in a signed written agreement between you and Nokia.
**
**
**
** $QT_END_LICENSE$
**
****************************************************************************/

#ifndef JSONDATABRIDGE_H
#define JSONDATABRIDGE_H

#include <QObject>
#include <QString>
#include <QList>
#include <QMap>
#include <QDebug>
#include <QFileInfo>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>
#include <QFile>
#include <QMessageBox>

#include "core/pathinfo.h"

/*!----------------------------------------------------+
 *                                                     |
 *      @author: Andrea Venturelli                     |
 *      @mail  : aventurelli.github@gmail.com          |
 *                                                     |
 *-----------------------------------------------------+
*/


class JsonDataBridge : public QObject
{
    Q_OBJECT
public:
    using PathInfoDictionary = QMap<QString, QList<PathInfo>>;

    // constructor
    explicit JsonDataBridge( const QString currDir, QObject* parent = nullptr );

    // load/save
    PathInfoDictionary loadData() const;
    bool saveData( const PathInfoDictionary& map_obj ) const;

    // getter
    inline const QString& path() { return m_path; }
    inline static const QString fileName() { return QString(FILE_NAME); }

private:
    const QString m_path;
    static constexpr char FILE_NAME[] { "/groupDataStorage.json" };

};

#endif // JSONDATABRIDGE_H
