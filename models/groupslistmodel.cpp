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

#include "models/groupslistmodel.h"

/*!----------------------------------------------------+
 *                                                     |
 *      @author: Andrea Venturelli                     |
 *      @mail  : insertTheMailHere@gmail.com           |
 *                                                     |
 *-----------------------------------------------------+
*/


GroupsListModel::GroupsListModel(QMap<QString, QList<PathInfo>>& map_groups, QWidget *parent)
    : QAbstractListModel{parent}, m_groups{ map_groups }, m_groupsName{ map_groups.keys() }
{}


/*!
 *
 *  overridden methods
 *
*/

int GroupsListModel::rowCount( const QModelIndex& parent ) const
{
    Q_UNUSED( parent )
    return m_groups.count();
}


QVariant GroupsListModel::data(const QModelIndex& index, int role) const
{
    if (!index.isValid()) {
        qDebug() << "invalid Index: model.data()";
        return QVariant();
    }

    switch (role)
    {
        case Qt::DisplayRole:
            return QVariant(m_groupsName[index.row()]);

        case GroupsListModel::Roles::ItemCount:
        {
            QString elem_count("Elements count: ");
            elem_count += QString::number( m_groups[m_groupsName[index.row()]].count() );
            return QVariant(elem_count);
        }
        case GroupsListModel::Roles::TopButtonStateRole:                // CHANGE: Added for top button state
            return QVariant::fromValue(topButtonState(index));

        case GroupsListModel::Roles::BottomButtonStateRole:             // CHANGE: Added for bottom button state
            return QVariant::fromValue(bottomButtonState(index));

        default:
            return QVariant();
    }
}


void GroupsListModel::addGroup( const QString& groupName,  QList<PathInfo> pInfoList )
{
    if ( m_groups.contains( groupName ) )
    {
        QMessageBox::warning(
            nullptr, "Duplicate Group Name",
            "Sorry, this group name is already in use.\n Try another one."
            );
        return;
    }

    beginResetModel();

    // add the new key to QMap
    m_groups[ groupName ] = pInfoList;

    // update the QStringList for Indexing accessing
    m_groupsName = m_groups.keys();

    endResetModel();

    // CHANGE: Clear button states for all indices
    m_topButtonStates.clear();
    m_bottomButtonStates.clear();
}


QList<PathInfo> GroupsListModel::removeGroup( const QString& groupName )
{
    if ( !m_groups.contains( groupName ) )
    {
        QMessageBox::warning(
            nullptr, "Unexisting Group",
            "Sorry, a not existing group could not be removed.\n Try to select a group first."
            );
        return QList<PathInfo>();
    }

    beginResetModel();
    // remove the key from the map
    QList<PathInfo> temp = m_groups.take( groupName );

    // update the QStringList for Indexing accessing
    m_groupsName = m_groups.keys();
    endResetModel();

    // CHANGE: Clear button states for all indices
    m_topButtonStates.clear();
    m_bottomButtonStates.clear();

    return temp;
}


void GroupsListModel::setTopButtonState(const QModelIndex& index, btnState state)
{
    if (index.isValid()) {
        m_topButtonStates[index] = state;
        emit topButtonStateChanged(index); // Notify the delegate
    }
}


void GroupsListModel::setBottomButtonState(const QModelIndex& index, btnState state)
{
    if (index.isValid()) {
        m_bottomButtonStates[index] = state;
        emit bottomButtonStateChanged(index); // Notify the delegate
    }
}


GroupsListModel::btnState GroupsListModel::topButtonState(const QModelIndex& index) const
{
    return m_topButtonStates.value(index, btnState::Default);
}


GroupsListModel::btnState GroupsListModel::bottomButtonState(const QModelIndex& index) const
{
    return m_bottomButtonStates.value(index, btnState::Default);
}



