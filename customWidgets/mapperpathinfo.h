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


#ifndef MAPPERPATHINFO_H
#define MAPPERPATHINFO_H

#include <QWidget>
#include <QDataWidgetMapper>
#include <QEvent>
#include <QMessageBox>
#include <QFileInfo>
#include <QFileDialog>

#include "statechip.h"
#include "./pathsinfotablemodel.h"
#include "viewtogglesw.h"
#include "pathinfo.h"

namespace Ui {
class MapperPathInfo;
}

class MapperPathInfo : public QWidget
{
    Q_OBJECT

public:
    explicit MapperPathInfo(PathsInfoTableModel* model, QWidget *parent = nullptr);
    ~MapperPathInfo();

    enum ColDescription {
        FileType = 0,
        FilePath = 1,
        IsValidChip = 2,
        IsOpenable = 3
    };

public slots:
    /*
     *  DESCRIPTION:
     *  \n This is supposed to be connected
     *  \n to the View::selectionModel::currentChanged() signal
     *  \n
     *  \n Whenever the view's row change, update the icon and set
     *  \n the correct index to the mapper
    */
    void on_rowSelectionChanged( const QModelIndex &current, const QModelIndex &previous );

private slots:
    /*
     *  DESCRIPTION:
     *  \n If the current path exists (folder or file)
     *  \n it will be opened
    */
    void on_open_path_btn_clicked();

    /*
     *  DESCRIPTION:
     *  \n Opens the fileDialog selection to choose a path
     *  \n and if is not empty set it to the model, calling model.setData()
    */
    void on_change_path_btn_clicked();

    /*
     *  DESCRIPTION:
     *  \n Simply calls the model.setData() with
     *  \n the modelIndex (row, Col=2) and the
     *  \n check is performed inside the setData()
    */
    void on_check_path_btn_clicked();

    /*
     *  DESCRIPTION:
     *  \n Synch the toggled_sw state to the model
    */
    void openableToggled(bool checked);

private:
    Ui::MapperPathInfo* ui;
    QDataWidgetMapper* m_mapper;
    PathsInfoTableModel* m_model;

    PathInfo* p_info{};

    StateChip* validity_chip{ new StateChip(this) };
    ViewToggleSw* toggle_sw{ new ViewToggleSw(this) };

    // helper functions
    //------------------
    int currentModelRow();
    QModelIndex currentModelIndex( int row, int col );
    QString currentPath();


};

#endif // MAPPERPATHINFO_H
