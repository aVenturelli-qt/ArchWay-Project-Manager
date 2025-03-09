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
**
**
**
** $QT_END_LICENSE$
**
****************************************************************************/

#ifndef PATHINFO_H
#define PATHINFO_H
#include <QWidget>
#include <QFileInfo>
#include <QPixmap>
#include <QPainter>
#include <QDesktopServices>
#include <QUrl>
#include <QDir>
#include <QMessageBox>

/*!----------------------------------------------------+
 *                                                     |
 *      @author: Andrea Venturelli                     |
 *      @mail  : aventurelli.github@gmail.com          |
 *                                                     |
 *-----------------------------------------------------+
*/


/* FUTURE POSSIBLE IMPLEMENNTATIONS
 *
 * 1) swap method within move-constructor to flip two rows on the model with ease
 * 2) implementation of operator== and operator< for comparisons and sort
 *
 *
 *
*/
//:/icons/pathEditor/asset/icons/file_icon.svg

class PathInfo: QObject
{
    Q_OBJECT

public:
    Q_SIGNAL void pathChanged( QString old_path, QString new_path );
    Q_SIGNAL void validityChanged(bool isValid);
    Q_SIGNAL void openableChanged(bool isOpenable);

    const QPixmap VALID_PIX  { validityPixmap(true)  };
    const QPixmap INVALID_PIX{ validityPixmap(false) };

    const QPixmap NOT_A_FILE_PIX { QPixmap(":/icons/pathEditor/asset/icons/not_existing_icon.svg").scaledToHeight(52, Qt::SmoothTransformation) };
    const QPixmap FILE_PIX       { QPixmap(":/icons/pathEditor/asset/icons/file_icon.svg").scaledToHeight(52, Qt::SmoothTransformation) };
    const QPixmap FOLDER_PIX     { QPixmap(":/icons/pathEditor/asset/icons/folder_icon.svg").scaledToHeight(52, Qt::SmoothTransformation) };


    // constructors
    PathInfo( ) : m_path{ "not a path" } {}
    PathInfo( QString path );
    PathInfo( char* path );

    PathInfo(const PathInfo& other );

    // destructor
    ~PathInfo() {} ;

    // operators overload
    QVariant operator[]( int column ) const;
    PathInfo operator= (const PathInfo& other );
    bool     operator==( const PathInfo& other ) const;

    // getters
    bool openable()     const { return m_isOpenable; }
    bool fileType()     const { return m_isFile; }
    bool pathValidity() const { return m_isValid; }
    QString path()      const { return m_path; }

    //setters
    void changePath ( QString new_path );
    void setOpenable( bool state );
    void checkPathValidity();


    // open file
    void openPath( bool ignoreIsOpenable = false ) const;

    // file info
    QList<QString> absolute_fullPath_fName();
    QString absPath()  const;
    QString fileName() const;
    QString filePath() const;

    // static convinience methods
    static bool pathIsValid( const QString path );
    static int  isFile( const QString path );

    void printInfo();

private:
    QString m_path;
    bool m_isValid{ false };
    bool m_isFile{ false };
    bool m_isOpenable{ true };

    QFileInfo m_file_info;

    void updatePathInfo();

    QPixmap validityPixmap(bool valid);
};





#endif // PATHINFO_H
