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

#ifndef GROUPSLISTMODEL_H
#define GROUPSLISTMODEL_H

#include <QWidget>
#include <QAbstractListModel>
#include <QStringList>
#include <QPainter>
#include <QMessageBox>
#include <QHash> // CHANGE: Added for button state tracking

#include "core/pathinfo.h"
#include "core/buttonStyle.h"


/*!----------------------------------------------------+
 *                                                     |
 *      @author: Andrea Venturelli                     |
 *      @mail  : aventurelli.github@gmail.com          |
 *                                                     |
 *-----------------------------------------------------+
*/


class GroupsListModel : public QAbstractListModel
{
    Q_OBJECT
public:
    using btnState = ButtonStyle::ButtonState;

    enum Roles {
        ItemCount               = Qt::UserRole + 1,
        TopButtonStateRole      = Qt::UserRole + 2,  // CHANGE: Added for top button state
        BottomButtonStateRole   = Qt::UserRole + 3   // CHANGE: Added for bottom button state
    };

    enum RowsOperation {
        Add    = 0,
        Remove = 1
    };

    explicit GroupsListModel(QMap<QString, QList<PathInfo>>& map_groups, QWidget *parent = nullptr);

    /* overridden virtual methods */
    int rowCount(const QModelIndex& parent = QModelIndex()) const override;
    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;

    // CHANGE: Added methods for button state management
    void setTopButtonState   (const QModelIndex& index, btnState state);
    void setBottomButtonState(const QModelIndex& index, btnState state);

    btnState topButtonState   (const QModelIndex& index) const;
    btnState bottomButtonState(const QModelIndex& index) const;

    void addGroup(const QString& groupName, QList<PathInfo> pInfoList = QList<PathInfo>() );
    QList<PathInfo> removeGroup(const QString& groupName);

    inline QString groupName(int row) const { return m_groupsName[row]; }
    inline int itemCount(int row) const { return m_groups[m_groupsName[row]].count(); }
    inline int indexOfGroup(const QString& groupName) const { return m_groupsName.indexOf(groupName); }

signals:
    // CHANGE: Added signals for button state changes
    void topButtonStateChanged(const QModelIndex& index);
    void bottomButtonStateChanged(const QModelIndex& index);

private:
    QMap<QString, QList<PathInfo>>& m_groups;
    QStringList m_groupsName;
    QHash<QModelIndex, btnState> m_topButtonStates;     // CHANGE: Added for top button state
    QHash<QModelIndex, btnState> m_bottomButtonStates;  // CHANGE: Added for bottom button state
};


#endif // GROUPSLISTMODEL_H
