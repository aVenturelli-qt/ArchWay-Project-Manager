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

#include "archway.h"
#include "ui_archway.h"

ArchWay::ArchWay(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::ArchWay), m_dictionary{ QMap<QString, QList<PathInfo> >() }
{
    ui->setupUi(this);
    ui->left_wdg->setLayout( new QVBoxLayout );

    // dummy lists
    PathInfo info(QString("C:\\Users\\test\\Downloads\\12 weeks scoreboard.pdf"));
    QList<PathInfo> lst1;
    lst1 << info;

    PathInfo info1(QString("C:\\Users\\test\\OneDrive\\Documenti\\Desktop\\dati.xlsx"));
    PathInfo info2(QString("C:\\Users\\test\\OneDrive\\Documenti\\Desktop\\QuickRunPro"));
    PathInfo info3(QString("C:\\Users\\test\\OneDrive\\Doci"));
    QList<PathInfo> lst2;
    lst2 << info << info2 << info3;

    PathInfo infoa(QString("C:\\Users\\test\\qwerty.xlsx"));
    PathInfo infob(QString("C:\\Users\\Desktop\\folder"));
    PathInfo infoc(QString("C:\\Users\\test\\OneDrive\\Dodici"));
    PathInfo infod(QString("C:\\Users\\Desktop\\folder"));
    PathInfo infof(QString("C:\\Users\\basic_file.txt"));
    QList<PathInfo> lst3;
    lst3 << infoa << infob << infoc << infod << infof;

    // fill the QMap
    m_dictionary["Group1"] = lst1;
    m_dictionary["Group2"] = lst2;
    m_dictionary["Group3"] = lst3;

    // create the model
    QList<PathInfo>& curr_lst = m_dictionary["Group1"];
    m_model = new PathsInfoTableModel( &curr_lst , this );

    // istanciate the groupEditor_widget
    m_group_editor = new GroupEditor( m_model, this );
    ui->left_wdg->layout()->addWidget( m_group_editor );


    // connect the group_list changed signal to the groupEditor
    //! when the user select another item in the list, update the QList<PathInfo> of the model
}

ArchWay::~ArchWay()
{
    delete ui;
}


/****************************************************
 *                  Qt's EVENTS
 ****************************************************/
void ArchWay::closeEvent(QCloseEvent* event)
{
    Q_UNUSED(event)
}


/****************************************************
 *              PRIVATE HELPER METHODS
 ****************************************************/

void ArchWay::saveDictionary()
{

}


void ArchWay::loadJsonDictionary()
{

}



void ArchWay::on_testGroup1_clicked()
{
    auto& group1 = m_dictionary["Group1"];
    m_model->setListData( &group1 );
    //QString group("GroupName 1");
    //m_group_editor->setGroupName(group);
}


void ArchWay::on_testGroup2_clicked()
{
    auto& group2 = m_dictionary["Group2"];
    m_model->setListData( &group2 );
    //QString group("GroupName 2");
    //m_group_editor->setGroupName(group);
}


void ArchWay::on_testGroup3_clicked()
{

    auto& group3 = m_dictionary["Group3"];
    m_model->setListData( &group3 );
    //QString group("GroupName 3");
    //m_group_editor->setGroupName(group);
}


void ArchWay::on_testToggleEditor_clicked(bool checked)
{
    m_group_editor->setVisible( checked );
}

