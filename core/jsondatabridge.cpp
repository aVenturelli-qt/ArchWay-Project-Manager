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

#include "core/jsondatabridge.h"


/*!----------------------------------------------------+
 *                                                     |
 *      @author: Andrea Venturelli                     |
 *      @mail  : aventurelli.github@gmail.com          |
 *                                                     |
 *-----------------------------------------------------+
*/


JsonDataBridge::JsonDataBridge(const QString currDir, QObject *parent)
    : QObject{parent}, m_path{ currDir + FILE_NAME }
{}


JsonDataBridge::PathInfoDictionary JsonDataBridge::loadData() const
{
    PathInfoDictionary temp_map;
    QFile file( m_path );

    QFileInfo fInfo( m_path );
    if ( !fInfo.exists() )
    {
        // create the file if not exist
        file.open( QFile::WriteOnly | QFile::Text );
        file.close();

        temp_map["My first Group"] = QList<PathInfo>();
        return temp_map;
    }

    // open file and read bytes
    bool success = file.open( QFile::ReadOnly );
    if ( !success )
    {
        qDebug() << "Json file can't be opend.. try again.";

        QMessageBox::critical(
            nullptr, "Unable To Load Groups",
            "The file containing the groups data is out of reach.\nThe application will close.."
        );
        throw std::runtime_error( "Can't open the file ");
    }

    QByteArray payload = file.readAll();
    file.close();

    // create json obj
    auto doc = QJsonDocument( QJsonDocument::fromJson( payload ) );
    QJsonObject jsonObj = doc.object();

    // case json file exist, is readable but has no data
    if ( jsonObj.isEmpty() )
    {
        temp_map["My first Group"] = QList<PathInfo>();
        return temp_map;
    }

    // fill the QMap
    foreach ( const QString& key, jsonObj.keys() )
    {
        QJsonObject child_obj = jsonObj.value(key).toObject();
        // qDebug() << "\n\nGroupName= " << key;
        // qDebug() << "-----------------------";

        QList<PathInfo> temp;
        temp.reserve( 50 );

        foreach ( const QString& path, child_obj.keys() )
        {
            bool openable{ child_obj.value(path).toBool() };
            // qDebug() << "pathKey = " << path << ", openable: " << openable;

            PathInfo pInfo( path );
            pInfo.setOpenable( openable );

            temp.append( pInfo );
        }

        temp_map[key] = temp;
    }

    qDebug() << "\nJson Loaded correctly";
    return temp_map;
}


bool JsonDataBridge::saveData( const PathInfoDictionary& map_obj ) const
{
    /*
     *  convert a QMap< QString, QList<PathInfo>
     *  to a json like:
     *  {
     *      "QString_QMapKey" :
     *          {
     *              "PathInfo1.path()" : PathInfo.isOpenable,
     *              "PathInfo2.path()" : PathInfo.isOpenable,
     *              "PathInfo3.path()" : PathInfo.isOpenable
     *          },
     *      "QString_QMapKey" :
     *          {
     *              "PathInfo1.path()" : PathInfo.isOpenable,
     *              "PathInfo2.path()" : PathInfo.isOpenable,
     *              "PathInfo3.path()" : PathInfo.isOpenable
     *          }
     *  }
    */

    QFile exp_file{ m_path };
    QJsonDocument doc;
    QJsonObject   root;

    // loop through the group
    foreach ( const QString& key, map_obj.keys() )
    {
        // qDebug() << "\n\nkey: " << key;

        QJsonObject   subRoot;
        // subGroup, access all PathInfo inside list
        foreach ( const PathInfo pInfo, map_obj[key] )
        {
            auto path = pInfo.path();
            bool openable = pInfo.openable();
            // qDebug() << "path: " << path << ", openable:" << openable;
            subRoot.insert( path, openable );
        }

        // add subGroup to the main branch. "key" : subgroup
        root.insert( key, subRoot );
    }

    // create the document and write the bytes to the file
    doc.setObject( root );
    bool success = exp_file.open( QFile::WriteOnly );
    if ( !success )
    {
        qDebug() << "\nImpossibile to open export-file...";
        return false;
    }

    exp_file.write( doc.toJson() );
    exp_file.close();
    qDebug() << "\nData Saved Correctly.";
    return true;

}
