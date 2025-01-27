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


#include "pathsinfotablemodel.h"

PathsInfoTableModel::PathsInfoTableModel(QList<PathInfo>* group, QObject *parent)
    : QAbstractTableModel{parent}, m_model_data{ group }
{}


Qt::ItemFlags PathsInfoTableModel::flags(
    const QModelIndex &index
) const
{
    if ( !index.isValid() ) return Qt::NoItemFlags;

    // sets path_col and IsOpenable_col editable
    if ( index.column() == 3 )
    {
        return Qt::ItemIsEditable | Qt::ItemIsEnabled | Qt::ItemIsSelectable;
    }

    return Qt::ItemIsEnabled | Qt::ItemIsSelectable;
}


int PathsInfoTableModel::rowCount(
    const QModelIndex &parent
) const
{
    Q_UNUSED(parent)
    return m_model_data->size();
}


int PathsInfoTableModel::columnCount(
    const QModelIndex &parent
) const
{
    Q_UNUSED(parent)
    return 4;
}


QVariant PathsInfoTableModel::data(
    const QModelIndex &index,
    int role
) const
{

    auto col = index.column();
    auto row = index.row();
    PathInfo p = m_model_data->at(row);

    // add pixmap to first and third column
    if ( role == Qt::DecorationRole && (col == 0 || col == 2) )
    {
        auto is_path_valid = p[2].toBool();

        // select the right icon for
        // the first column
        if ( col == 0 )
        {
            // path doesn't exist
            if ( !is_path_valid )
                return p.NOT_A_FILE_PIX;

            // exists and is a file
            if ( p[0].toBool() )
                return p.FILE_PIX;

            // is a folder
            return p.FOLDER_PIX;
        }

        return ( is_path_valid ) ? p.VALID_PIX : p.INVALID_PIX;
    }

    if ( role == Qt::DisplayRole && ( col == 1 || col == 3 ) )
    {
        return p[col];
    }

    /****************************************************************************
     * change it with a custom QStyledItem
     * to draw the chips instead
     ****************************************************************************/
    // add text Valid-Invalid
    if ( role == Qt::DisplayRole && col == 2 )
    {
        return ( p[col].toBool() ) ? QVariant( QString("Valid") ) : QVariant( QString("Invalid") );
    }


    return QVariant();
}


bool PathsInfoTableModel::setData(
    const QModelIndex& index,
    const QVariant& value,
    int role
)
{
    auto row = index.row(), col = index.column();
    QModelIndex fileTypeIdx = createIndex(row, 0);
    QModelIndex validityIdx = createIndex(row, 2);

    if ( !index.isValid() || row >= m_model_data->size() )
        return false;


    if ( role == Qt::DisplayRole || role == Qt::EditRole )
    {
        // get PathInfo at row index.row()
        PathInfo& pathInfo = (*m_model_data)[row];

        if ( col == 1 )          // edit the path_column
        {
            // not a path, return
            if ( !value.canConvert<QString>() ) return false;

            QString oldPath = pathInfo.path();
            pathInfo.changePath( value.toString() );

            if ( oldPath != pathInfo.path() )
            {
                emit dataChanged( index, index, { Qt::DisplayRole, Qt::EditRole } );
                emit dataChanged( fileTypeIdx, fileTypeIdx, { Qt::DisplayRole } );
                emit dataChanged( validityIdx, validityIdx, { Qt::DisplayRole } );
                return true;
            }

            return true;

        }
        else if ( col == 2 ) // edit the isValid chip
        {
            // check if data is of type bool
            if ( !value.canConvert<bool>() ) return false;

            bool old_state = pathInfo.pathValidity();

            // checks if path is valid
            pathInfo.checkPathValidity();

            if ( old_state != pathInfo.pathValidity() )
            {
                // update FileType and ValidityChip col
                emit dataChanged( fileTypeIdx, fileTypeIdx, { Qt::DisplayRole } );
                emit dataChanged( validityIdx, validityIdx, { Qt::DisplayRole } );
                return true;
            }

            return true;
        }
        else if ( col == 3 ) // edit the isOpenable_col
        {
            if ( !value.canConvert<bool>() ) return false;

            pathInfo.setOpenable( value.toBool() );
            emit dataChanged( index, index, { Qt::DisplayRole, Qt::EditRole } );
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



