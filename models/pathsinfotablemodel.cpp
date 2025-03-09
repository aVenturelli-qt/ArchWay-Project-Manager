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


#include "models/pathsinfotablemodel.h"


/*!----------------------------------------------------+
 *                                                     |
 *      @author: Andrea Venturelli                     |
 *      @mail  : insertTheMailHere@gmail.com           |
 *                                                     |
 *-----------------------------------------------------+
*/


PathsInfoTableModel::PathsInfoTableModel(QList<PathInfo>* group, QObject *parent)
    : QAbstractTableModel{parent}, m_model_data{ group }
{}


Qt::ItemFlags PathsInfoTableModel::flags( const QModelIndex &index ) const
{
    if ( !index.isValid() ) return Qt::NoItemFlags;

    // sets path_col and IsOpenable_col editable
    if ( index.column() == 3 )
    {
        return Qt::ItemIsEditable | Qt::ItemIsEnabled | Qt::ItemIsSelectable;
    }

    return Qt::ItemIsEnabled | Qt::ItemIsSelectable;
}


int PathsInfoTableModel::rowCount( const QModelIndex &parent ) const
{
    Q_UNUSED(parent)
    return m_model_data->size();
}


int PathsInfoTableModel::columnCount( const QModelIndex &parent ) const
{
    Q_UNUSED(parent)
    return 4;
}


QVariant PathsInfoTableModel::data(
    const QModelIndex &index,
    int role
) const
{
    if ( !index.isValid() ) return QVariant();

    int col = index.column();
    int row = index.row();
    PathInfo p = m_model_data->at(row);

    switch (role)
    {
        case Qt::EditRole:
            return p[col];

        case Qt::DisplayRole:
            if ( col == 1 || col == 3 ) { return p[col]; }

            if ( col == 2 )
            {
                return ( p.pathValidity() ) ? QVariant( QString("Valid") ) : QVariant( QString("Invalid") );
            }

            return QVariant();

        case Qt::DecorationRole:        // this will be removed soon
        {
            auto is_path_valid = p[2].toBool();

            if ( col == 2 )
            {
                return ( is_path_valid ) ? QVariant(p.VALID_PIX) : QVariant(p.INVALID_PIX);
            }

            return QVariant();
        }

        case Qt::UserRole:
        {
            auto is_path_valid = p[2].toBool();

            // path doesn't exist
            if ( !is_path_valid ) return QVariant(":/icons/pathEditor/asset/icons/not_existing_icon.svg");

            // exists and is a file
            if ( p[0].toBool() )
            {
                return QVariant(":/icons/pathEditor/asset/icons/file_icon.svg");
            }

            // is a folder
            return QVariant(":/icons/pathEditor/asset/icons/folder_icon.svg");
        }

        default:
        {
            //qDebug() << "\nDefault case Hitted: return QVariant for role:" << role;
            //qDebug() << "row:" << row << ", col:" << col << "\n";
            return QVariant();
        }
    }
}


bool PathsInfoTableModel::setData(
    const QModelIndex& index,
    const QVariant& value,
    int role
)
{

    if ( !index.isValid() || index.row() >= m_model_data->size() )
        return false;

    if ( role == Qt::DisplayRole || role == Qt::EditRole )
    {
        auto row = index.row();
        auto col = index.column();
        QModelIndex fileTypeIdx = createIndex(row, 0);
        QModelIndex validityIdx = createIndex(row, 2);

        // get PathInfo at row index.row()
        PathInfo& pathInfo = (*m_model_data)[row];

        // path_column
        if ( col == 1 )
        {
            // not a path, return
            if ( !value.canConvert<QString>() ) return false;

            QString oldPath = pathInfo.path();
            pathInfo.changePath( value.toString() );

            if ( oldPath != pathInfo.path() )
            {
                emit dataChanged( index, index, { Qt::EditRole, Qt::DisplayRole  } );
                emit dataChanged( fileTypeIdx, fileTypeIdx, { Qt::EditRole, Qt::DisplayRole } );
                emit dataChanged( validityIdx, validityIdx, { Qt::EditRole, Qt::DisplayRole } );
                return true;
            }
            // nothing changed but exhaust the
            // modifying correctly
            return true;
        }

        // isValid chip
        if ( col == 2 )
        {
            // if value is not bool return
            if ( !value.canConvert<bool>() ) return false;

            bool old_state = pathInfo.pathValidity();

            // checks if path is valid
            pathInfo.checkPathValidity();

            qDebug() << "old state:" << old_state << ", new state:" << pathInfo.pathValidity();

            if ( old_state != pathInfo.pathValidity() )
            {
                // update FileType and ValidityChip col
                emit dataChanged( validityIdx, validityIdx, { Qt::DisplayRole, Qt::EditRole } );
                emit dataChanged( fileTypeIdx, fileTypeIdx, { Qt::DisplayRole, Qt::EditRole } );
                return true;
            }
            return true;
        }


        // edit the isOpenable_col
        if ( col == 3 )
        {
            if ( !value.canConvert<bool>() ) return false;

            pathInfo.setOpenable( value.toBool() );
            emit dataChanged( index, index, { Qt::EditRole, Qt::DisplayRole } );
            return true;
        }

        return false;
    }

    return false;
}


QVariant PathsInfoTableModel::headerData(
    int section,
    Qt::Orientation orientation,
    int role
) const
{
    if ( orientation != Qt::Horizontal || role != Qt::DisplayRole ) return QVariant();

    switch ( section ) {
    case 0:
        return "Type";
    case 1:
        return "Full Path";
    case 2:
        return "Validity State";
    case 3:
        return "Openable";
    default:
        return QVariant();
    }
}


void PathsInfoTableModel::setListData( QList<PathInfo>* new_paths_list )
{
    beginResetModel();
    m_model_data = new_paths_list;
    endResetModel();
}



/**********************************************
 *
 *  implement ADD ROW and DELETE ROW
 *
 *********************************************/
bool PathsInfoTableModel::removeRows(int row, int count, const QModelIndex& parent )
{
    if ( row < 0 || row + count > m_model_data->count() )
    {
        qDebug() << "[removeRows] index out of range ( row < 0 || row + count > rowCount() )";
        return false;
    }

    // tell the model that we are about
    // to change the internal structure
    beginRemoveRows( parent, row, row + count - 1 );
    for (int i = 0; i < count; ++i)
    {
        m_model_data->removeAt( row );
    }
    endRemoveRows();
    return true;
}


bool PathsInfoTableModel::insertRows( int row, int count, const QModelIndex& parent )
{
    Q_UNUSED(parent)

    if ( row < 0 || row > m_model_data->count() )
    {
        qDebug() << "[insertRows] index out of range ( row < 0 || row > rowCount() )";
        return false;
    }

    // tell the model that we are about
    // to change the internal structure
    beginInsertRows( QModelIndex(), row, row + count - 1 );
    for (int i = 0; i < count; ++i)
    {
        m_model_data->insert( row + i, PathInfo() );
    }
    endInsertRows();
    return true;
}


void PathsInfoTableModel::fillNewRow( int row, QString path )
{
    PathInfo& p_info = (*m_model_data)[row];
    p_info.changePath( path );

    auto filetype_idx = createIndex( row, 0 );
    auto path_idx = createIndex( row, 1 );
    auto validity_idx = createIndex( row, 2 );
    auto openable_idx = createIndex( row, 3 );

    // notify the views to update the data
    emit dataChanged( filetype_idx, filetype_idx, { Qt::DisplayRole, Qt::EditRole } );
    emit dataChanged( path_idx, path_idx,         { Qt::DisplayRole, Qt::EditRole } );
    emit dataChanged( validity_idx, validity_idx, { Qt::DisplayRole, Qt::EditRole } );
    emit dataChanged( openable_idx, openable_idx, { Qt::DisplayRole, Qt::EditRole } );
}

