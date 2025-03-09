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

#ifndef ADDRENAMEDIALOG_H
#define ADDRENAMEDIALOG_H

#include <QWidget>
#include <QMessageBox>

/*!----------------------------------------------------+
 *                                                     |
 *      @author: Andrea Venturelli                     |
 *      @mail  : aventurelli.github@gmail.com          |
 *                                                     |
 *-----------------------------------------------------+
*/

namespace Ui {
class AddRenameDialog;
}

class AddRenameDialog : public QWidget
{
    Q_OBJECT

public:
    enum ShowMod
    {
        AddGroup,
        RenameGroup
    };

    explicit AddRenameDialog(QWidget *parent = nullptr);
    ~AddRenameDialog();


    /*!
     *  DESCRIPTION:
     *  >>> this method can show the dialog in two versions:
     *      1) for adding a new group (rename label and lineEdit hidden)
     *      2) for Renaming a group, set the top lineEdit to the current group and is read-only
    */
    void showEditorAs( const QString& groupName = "", ShowMod mode = ShowMod::AddGroup, bool visible = true );

    inline ShowMod currentMode() const { return m_mode; }

signals:
    void renamedGroup( const QString& oldName, const QString& newName );
    void addedGroup  ( const QString& groupName );
    void editorHidden();

public slots:
    // reimplements the QWidget::hide() slots
    // to emit the signal editorHidden()
    void hide();

private slots:
    void on_submit_btn_clicked();

    void on_closeEditorBtn_clicked();

private:
    Ui::AddRenameDialog *ui;
    ShowMod m_mode;
};

#endif // ADDRENAMEDIALOG_H
