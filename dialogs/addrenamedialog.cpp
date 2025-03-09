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

#include "dialogs/addrenamedialog.h"
#include "ui_addrenamedialog.h"


/*!----------------------------------------------------+
 *                                                     |
 *      @author: Andrea Venturelli                     |
 *      @mail  : aventurelli.github@gmail.com          |
 *                                                     |
 *-----------------------------------------------------+
*/


AddRenameDialog::AddRenameDialog(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::AddRenameDialog)
{
    ui->setupUi(this);
    this->setWindowFlag( Qt::FramelessWindowHint, true );

    // setup init
    this->showEditorAs( "", ShowMod::AddGroup, false );
}


AddRenameDialog::~AddRenameDialog()
{
    delete ui;
}


/*
 *
 *  PUBLIC SLOTS
 *
*/
void AddRenameDialog::hide()
{
    emit editorHidden();
    QWidget::hide();
}


/*
 *
 *  PUBLIC METHODS
 *
*/
void AddRenameDialog::showEditorAs( const QString& groupName, ShowMod mode, bool visible  )
{
    m_mode = mode;
    visible ? this->show() : this->hide();

    if ( mode == ShowMod::AddGroup)
    {
        ui->top_le->setReadOnly( false );

        // hide rename lineEdit and label
        ui->bottom_lbl->hide();
        ui->bottom_le->hide();

        // clean the top lineEdit
        ui->top_le->clear();
        ui->top_le->setFocus();

        // dynamic button text based on state
        ui->submit_btn->setText("Create new Group");
        return;
    }

    // case Rename group
    //-----------------------------
    ui->bottom_lbl->show();
    ui->bottom_le->show();

    // set current group name as read-only
    ui->top_le->setText( groupName );
    ui->top_le->setReadOnly( true );

    // clear the edit group name and set focus
    ui->bottom_le->clear();
    ui->bottom_le->setFocus();

    // dynamic button text
    ui->submit_btn->setText( "Rename Group" );
}


void AddRenameDialog::on_submit_btn_clicked()
{
    if ( m_mode == ShowMod::AddGroup )
    {
        auto groupName = ui->top_le->text();
        if ( groupName.isEmpty() )
        {
            QMessageBox::warning(
                this, "Empty Name",
                "The group's name can not be empty.\nChoose a name before submit it"
            );

        } else {
            emit addedGroup( groupName );
            // hide widget after submitting
            this->hide();
            emit editorHidden();
        }

        return;
    }

    // Rename group case
    auto old_name = ui->top_le->text();
    auto new_name = ui->bottom_le->text();

    if ( new_name.isEmpty() )
    {
        QMessageBox::warning(
            this, "Empty Name",
            "The group's name can not be empty.\nChoose a name before submit it"
        );

    } else {
        emit renamedGroup( old_name, new_name );
        // hide widget after submitting
        this->hide();
        emit editorHidden();
    }
}


void AddRenameDialog::on_closeEditorBtn_clicked()
{
    this->hide();
}

