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

#ifndef GROUPEDITOR_H
#define GROUPEDITOR_H

#include <QWidget>
#include <QFileDialog>


#include "pathsinfotablemodel.h"
#include "customWidgets/mapperpathinfo.h"


namespace Ui {
class GroupEditor;
}

class GroupEditor : public QWidget
{
    Q_OBJECT

public:
    explicit GroupEditor(PathsInfoTableModel* model, QWidget *parent = nullptr);
    ~GroupEditor();

    const QString groupName() const;
    void setGroupName( const QString& name );

private slots:
    /*!
     * DESCRIPTION:
     * \n - Add a new row to the model and call
     * \n   the FileSystem to select a path
     */
    void on_ctrl_new_btn_clicked();

    /*!
     * DESCRIPTION:
     * \n - for every row in the model
     * \n   checks for every path, the "isValid()"
     * \n   member has changed
     */
    void on_ctrl_validity_btn_clicked();

    /*!
     * DESCRIPTION:
     * \n - Delete selected rows from the model
     */
    void on_ctrl_delete_btn_clicked();

    void on_modelResetted();

private:
    Ui::GroupEditor *ui;
    MapperPathInfo* m_mapper_wdg;
    PathsInfoTableModel* m_model;
};

#endif // GROUPEDITOR_H
