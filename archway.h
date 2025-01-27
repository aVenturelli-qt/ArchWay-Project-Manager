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

#ifndef ARCHWAY_H
#define ARCHWAY_H

#include <QMainWindow>
#include <QList>
#include <QMap>
#include <QDebug>
#include <QAbstractItemView>
#include <QLabel>

#include "pathsinfotablemodel.h"
#include "groupeditor.h"
#include "customWidgets/pathinfo.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class ArchWay;
}
QT_END_NAMESPACE


class ArchWay : public QMainWindow
{
    Q_OBJECT

public:
    ArchWay(QWidget* parent = nullptr);
    ~ArchWay();

protected:
    /*
     * DESCRIPTION:
     * \n - before closing the window
     * \n   calls "saveDictionary()" to
     * \n   store the data
     */
    void closeEvent(QCloseEvent *event) override;

private slots:

    void on_testGroup1_clicked();

    void on_testGroup2_clicked();

    void on_testGroup3_clicked();

    void on_testToggleEditor_clicked(bool checked);


private:
    Ui::ArchWay* ui;
    QMap<QString, QList<PathInfo> > m_dictionary;
    GroupEditor* m_group_editor;
    PathsInfoTableModel* m_model;

    /*!
     * DESCRIPTION:
     * \n - take the current QMap and store it in a json file
     */
    void saveDictionary();

    /*!
     * DESCRIPTION:
     * \n - read a json file and store the data
     * \n   inside the QMap member
     */
    void loadJsonDictionary();

};
#endif // ARCHWAY_H
