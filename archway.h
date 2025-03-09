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
#include <QDir>
#include <QList>
#include <QMap>
#include <QDebug>
#include <QAbstractItemView>
#include <QLabel>
#include <QGraphicsDropShadowEffect>

#include "models/pathsinfotablemodel.h"
#include "models/groupslistmodel.h"
//-------------------------------------
#include "core/pathinfo.h"
#include "core/jsondatabridge.h"
//-------------------------------------
#include "customWidgets/groupeditor.h"
#include "customWidgets/crudcontrols.h"
//-------------------------------------
#include "dialogs/addrenamedialog.h"


/*!----------------------------------------------------+
 *                                                     |
 *      @author: Andrea Venturelli                     |
 *      @mail  : aventurelli.github@gmail.com          |
 *                                                     |
 *-----------------------------------------------------+
*/


QT_BEGIN_NAMESPACE
namespace Ui {
class ArchWay;
}
QT_END_NAMESPACE


class ArchWay : public QMainWindow
{
    Q_OBJECT

public:
    enum TopLvlBtnAlignment {
        TopLeft,
        TopRight,
        BottomLeft,
        BottomRight,
        MiddleOffsetted
    };

    ArchWay(QWidget* parent = nullptr);
    ~ArchWay();

protected:
    /*!
     * DESCRIPTION:
     * before closing the window
     * calls "saveDictionary()" to store the data
     */
    void closeEvent( QCloseEvent *event ) override;

    /*! DESCRIPTION:
     * Close the Dialog, if visible, when we press
     * in the mainWindow frame (where children don't consume the event)
    */
    void mousePressEvent( QMouseEvent* event  ) override;

    void paintEvent( QPaintEvent* event ) override {
        QPainter painter(this);

        // Enable Antialiasing
        painter.setRenderHint(QPainter::Antialiasing);
        painter.setRenderHint(QPainter::TextAntialiasing);
        painter.setRenderHint(QPainter::SmoothPixmapTransform);

        QMainWindow::paintEvent( event );
    }

    /*! DESCRIPTION:
     *  Updates the position of the fixed topLevelButtons
    */
    bool eventFilter(QObject *watched, QEvent *event) override;

signals:
    void wdgRightResized( QSize newSize );

public slots:
      /*!
     * DESCRIPTION: This slots is triggered in response to
     * the signal CRUDcontrols::deleteClicked and calls the
     * model->removeGroup( "currentGroupSelected" )
    */
    void on_removeGroup();

    /*!
     * DESCRIPTION: This slot is intended to be
     * connected with the CRUDcontrols::addGroupClicked.
     * As in respose to that signal, hide the controls and
     * show the approriate editorDialog
    */
    void on_addGroupBtnPressed();

    /*!
     * DESCRIPTION: This slot is intended to be
     * connected with the CRUDcontrols::editClicked.
     * As in respose to that signal, hide the controls and
     * show the approriate editorDialog
    */
    void on_editGroupBtnPressed();

private slots:
    void groupItemSelectionChanged( const QModelIndex& current_selection, const QModelIndex& previous_selection );

    void insertGroupInView( const QString& grName );

    void on_openAllButtonClicked( const QString grName );

    /*!
     * DESCRIPTION: every time a listItem's editBtn signal is emitted
     * this slot react to it, and open the sideEditor for pathsInfo.
     *  - @param const QString group: name of the group selected
    */
    void on_editBtnClicked( const QString grName );

    void onWdgRightResized( QSize newSize );

private:
    QString CURR_DIR{ QDir::currentPath() };

    Ui::ArchWay* ui;
    QMap<QString, QList<PathInfo> > m_map;
    GroupEditor* m_group_editor;
    PathsInfoTableModel* m_model;
    //------------------------------------------
    GroupsListModel* m_lstModel;
    QString m_lastGrpSelected{};
    //-------------------------------------------
    JsonDataBridge m_jBridge;
    //-----------------------------------------
    CRUDcontrols* m_crud_btns;
    AddRenameDialog* m_groupEditingDiag;

    static constexpr int BTNS_CNTR_PADDING{ 16 };

    /*!
     * DESCRIPTION:
     *  @param topLvlWdg: ptr to the widget that will be moved.
     *  @param parentWdg: ptr to the widget (usually MainWindow) where the topLvlWgt should be relative to.
     *  @param padding_x: padding on x axis to keep from the parent's frame.
     *  @param padding_y: padding on y axis to keep from the parent's frame.
     *  @param alignment: enum that indicate in which region of the parent should be drawn the TopLvlWdg
    */
    void repositionTopLvlWidget(
        QWidget* topLvlWgd,
        QWidget* parentWidget,
        int padding_x = 0, int padding_y = 0,
        TopLvlBtnAlignment alignment = TopLvlBtnAlignment::BottomRight
        );

    void _initLeftWdg();
    void _setupListGroup();
    void _setupTopWdgs();

};
#endif // ARCHWAY_H
