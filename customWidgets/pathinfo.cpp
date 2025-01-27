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

#include "pathinfo.h"

PathInfo::PathInfo( QString path ) : m_path{ path }
{
    this->updatePathInfo();
}

// delegate constructor
PathInfo::PathInfo( char* path ) : PathInfo( QString(path) )
{}


/****************************************
 *
 *           copy coonstructor
 *
 ***************************************/
PathInfo::PathInfo(const PathInfo& other ) : QObject{ other.parent() } // not sure about parent initialization
{
    this->m_path = other.m_path;
    this->m_isValid = other.m_isValid;
    this->m_isFile = other.m_isFile;
    this->m_isOpenable = other.m_isOpenable;

    this->m_file_info = other.m_file_info;
}


/****************************************
 *
 *  operators overload
 *
 ***************************************/
PathInfo PathInfo::operator=(const PathInfo& other )
{
    // prevent self-assignment:
    // obj1 = obj1
    if ( this == &other ) return *this;

    this->m_path = other.m_path;
    this->m_isValid = other.m_isValid;
    this->m_isFile = other.m_isFile;
    this->m_isOpenable = other.m_isOpenable;

    this->m_file_info = other.m_file_info;

    return *this;
}


QVariant PathInfo::operator[] ( int column )
{
    switch ( column )
    {
        case 0:
            return QVariant( m_isFile );
        case 1:
            return QVariant( m_path );
        case 2:
            return QVariant( m_isValid );
        case 3:
            return QVariant( m_isOpenable );
    }

    return QVariant();
}

/****************************************
 *
 *      opens the file or the dir
 *
 ***************************************/
void PathInfo::openPath( bool ignoreIsOpenable )
{
    if ( !m_isValid || ( !ignoreIsOpenable && !m_isOpenable) )
    {
        return;
    }

    QString path_unix = QDir::toNativeSeparators( m_path );
    QUrl file_url = QUrl::fromLocalFile( path_unix );

    // open the file or dir
    (QDesktopServices::openUrl( file_url )) ?
        ( qDebug() << "opened succesfully:" << m_path ) :
        ( qDebug() << "impossibile to open file: " << m_path );
}


/* ---------------------------------------------- */
void PathInfo::updatePathInfo()
{
    m_file_info = QFileInfo( m_path );

    // check if the path exist
    m_isValid = m_file_info.exists();
    qDebug() << "\n\t [UPI] path '" << m_path << "' exist=" << m_isValid;

    // path's type checking
    if ( m_isValid )
    {
        // if you not check the "fileExists"
        // .isFile() could return false and you
        // have to check .isDir also
        m_isFile = m_file_info.isFile();
        qDebug() << "\n\t [UPI] path '" << m_path << "' isFile=" << m_isFile;
    }
}


void PathInfo::changePath( QString new_path )
{
    // avoid users for select the already present path
    if ( m_path == new_path ) return;

    // store old path
    auto old_p = this->m_path;

    // assign new path and synch
    // the others member var
    m_path = new_path;

    // updates FileInfo(new_path) and checks
    // m_isValid = fileInfo.exists()
    // update m_isFile also
    this->updatePathInfo();

    // notify path changed
    emit pathChanged( old_p, new_path );
    emit validityChanged( m_isValid );
}


void PathInfo::checkPathValidity()
{
    // this method is usefull if a
    // user add a validPath to the app
    // but later in the future he unconsciusly
    // move the file to another location or changing
    // it's name, resulting in an invalid path

    bool old_validity = m_isValid;

    // check if exists now
    m_isValid = m_file_info.exists();

    if ( m_isValid )
    {
        // update the flag
        m_isFile = m_file_info.isFile();
    }

    // notify the change whenever
    // the state is different from before
    if ( old_validity != m_isValid )
        emit validityChanged( m_isValid );
}


void PathInfo::setOpenable( bool state )
{
    if ( m_isOpenable != state )
    {
        m_isOpenable = state;
        emit openableChanged( m_isOpenable );
    }
}

/*********************************
 *
 *      FILE PATH INFO
 *
 *********************************/
QList<QString> PathInfo::absolute_fullPath_fName()
{
    if ( m_path.isEmpty() ) return QList<QString>();

    auto parent_abs_dir_path = m_file_info.absolutePath();
    auto file_name = m_file_info.fileName();

    return QList<QString>(
        {
            parent_abs_dir_path,
            m_path,
            file_name
        }
    );
}


QString PathInfo::absPath() const
{
    if ( m_path.isEmpty() ) return QString();

    return m_file_info.absolutePath();
}


QString PathInfo::fileName() const
{
    if ( m_path.isEmpty() ) return QString();

    return m_file_info.fileName();
}


QString PathInfo::filePath() const
{
    if ( m_path.isEmpty() ) return QString();

    return m_file_info.absoluteFilePath();
}


/*********************************
 *
 *      STATIC METHODS
 *
 *********************************/
bool PathInfo::pathIsValid( const QString path )
{
    return QFileInfo::exists( path );
}


int PathInfo::isFile( const QString path )
{
    // Description:
    // return -1 if the given path does not exists
    // otherwise 0 for Dir and 1 for File is given

    auto f_info = QFileInfo( path );
    if ( !f_info.exists() ) return -1;

    return f_info.isFile();
}


QPixmap PathInfo::validityPixmap(bool valid)
{
    QPixmap pix(32, 32);
    pix.fill(Qt::transparent);

    QPainter p( &pix );
    p.setRenderHint(QPainter::Antialiasing, true);

    p.setPen( QPen( QBrush(Qt::white), 2 ) );

    auto brush = ( valid ) ? Qt::green : Qt::darkRed;
    p.setBrush(brush);

    p.drawEllipse( pix.rect().center(), 12, 12);
    p.end();

    return pix;
}




/*********************************
 *
 *          statistics
 *
 *********************************/
void PathInfo::printInfo()
{
    qDebug() << "\n------------------------------------------------------------------------------------------------\n"
             << "[PathInfo] path=" << m_path;
    qDebug() << "\n\t(isValid):" << m_isValid
        << "\t(isFile):" << m_isFile
        << "\t(isOpenable):" << m_isOpenable;
    qDebug() << "\n-------------------------------------------------------------------------------------------------\n"
        << "[others information]\n"
        << "\tAbsolute Parent Path:  " << absPath()
        << "\n\tFile name:  " << fileName();
}


