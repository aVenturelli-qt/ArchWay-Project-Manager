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
//-------------------------------------
#include "delegates/groupslistdelegate.h"
//-------------------------------------
#include "core/buttonStyle.h"


/*!----------------------------------------------------+
 *                                                     |
 *      @author: Andrea Venturelli                     |
 *      @mail  : aventurelli.github@gmail.com          |
 *                                                     |
 *-----------------------------------------------------+
*/


ArchWay::ArchWay(QWidget *parent)
    : QMainWindow(parent),
    ui(new Ui::ArchWay),
    m_jBridge{ CURR_DIR }
{
    ui->setupUi(this);
    ui->left_wdg->setLayout( new QVBoxLayout );
    this->setMouseTracking( true );

    this->setWindowIcon( QIcon(":/icons/pathEditor/asset/images/ArchWay_icon.svg") );

    // initialize the QMap
    // - - - - - - - - - - - - - - -
    m_map = m_jBridge.loadData();

    this->_initLeftWdg();
    this->_setupListGroup();
    this->_setupTopWdgs();

    if ( m_lstModel->rowCount() <= 0 )
    {
        // disable rename
        m_crud_btns->setRenameEnabled( false );
    }

    // select first item
    //---------------------------------------------
    auto firstModelIdx = m_lstModel->index( 0, 0 );
    ui->list_group->selectionModel()->setCurrentIndex(
        firstModelIdx, QItemSelectionModel::SelectCurrent
        );
}


ArchWay::~ArchWay()
{
    delete ui;
}


/****************************************************
 *                CONSTRUCTOR INIT
 ****************************************************/
void ArchWay::_initLeftWdg()
{
    // create the model
    // - - - - - - - - -  - - - - -
    QList<PathInfo>& curr_lst = m_map[ m_map.firstKey() ];
    m_model = new PathsInfoTableModel( &curr_lst , this );

    // istanciate the groupEditor_widget
    // - - - - - - - - - - - - - - - - -
    m_group_editor = new GroupEditor( m_model );
    ui->left_wdg->layout()->addWidget( m_group_editor );
    ui->left_wdg->layout()->setContentsMargins(24, 24, 24, 24);

    ui->left_wdg->hide();

    // hide the left widget when the X is pressed
    // - - - - - - - - - - - - - - - - - - - - - -
    connect( m_group_editor, &GroupEditor::hideEditor, this, [this](){ ui->left_wdg->hide(); } );
}


void ArchWay::_setupListGroup()
{
    // QListModel for presenting the data inside QMap
    m_lstModel = new GroupsListModel( m_map, ui->list_group );
    ui->list_group->setModel( m_lstModel );

    // delegate button style
    auto editorBtn_style = ButtonStyle();
    editorBtn_style.setFillStrokeColor( Qt::white, QColor(114, 24, 183), ButtonStyle::ButtonState::Hover );

    // ListView delegate
    auto my_delegate = new GroupsListDelegate( ui->list_group );
    my_delegate->setTopButtonsStyle( editorBtn_style );
    ui->list_group->setItemDelegate( my_delegate );


    auto* selectionModel = ui->list_group->selectionModel();

    // connect signals to slots
    //- - - - - - - - - - - - -

    // update the current idx relative to the selected item
    connect( selectionModel, &QItemSelectionModel::currentChanged, this, &ArchWay::groupItemSelectionChanged );

    // check from the delegate which editBtn is pressed
    QObject::connect(
        my_delegate, &GroupsListDelegate::editButtonClickedGroup,
        this, &ArchWay::on_editBtnClicked
        );

    QObject::connect(
        my_delegate, &GroupsListDelegate::openAllButtonClickedGroup,
        this, &ArchWay::on_openAllButtonClicked
        );
}


void ArchWay::_setupTopWdgs()
{

    // set filter event to wdgRight
    // in order to place correctly the topLvlWdgs
    ui->wdgRight->installEventFilter( this );

    // top level buttons control init + drop-shadow
    //---------------------------------------------
    m_crud_btns = new CRUDcontrols( this );
    auto dropShadow = new QGraphicsDropShadowEffect();
    dropShadow->setBlurRadius( 25.6 );
    dropShadow->setOffset( -4 );
    dropShadow->setColor( QColor( 19, 11, 19, 60 ) );
    m_crud_btns->setGraphicsEffect( dropShadow );

    // connect CRUDcontrols to slots:
    connect( m_crud_btns, &CRUDcontrols::addGroupClicked, this, &ArchWay::on_addGroupBtnPressed );
    connect( m_crud_btns, &CRUDcontrols::editClicked,     this, &ArchWay::on_editGroupBtnPressed );
    connect( m_crud_btns, &CRUDcontrols::deleteClicked,   this, &ArchWay::on_removeGroup );

    // Add and rename Dialog
    //-------------------------------------------------
    m_groupEditingDiag = new AddRenameDialog( this );

    auto dropShadow2 = new QGraphicsDropShadowEffect();
    dropShadow2->setOffset( -4 );
    dropShadow2->setBlurRadius( 25.6 );
    dropShadow2->setColor( QColor( 19, 11, 19, 60 ) );

    m_groupEditingDiag->setGraphicsEffect( dropShadow2 );

    connect( m_groupEditingDiag, &AddRenameDialog::addedGroup, this, &ArchWay::insertGroupInView );

    // to rename a group, delete the old key
    // and add the new key with the previous values
    connect(
        m_groupEditingDiag, &AddRenameDialog::renamedGroup,
        this, [this](const QString& oldName, const QString& newName) {
            QList<PathInfo> data = m_lstModel->removeGroup( oldName );
            m_lstModel->addGroup( newName, data );

            // retrive te new row to use it later for
            // selecting the group
            int row = m_lstModel->indexOfGroup( newName );
            const QModelIndex& idx = m_lstModel->index( row, 0 );

            ui->list_group->selectionModel()->setCurrentIndex( idx, QItemSelectionModel::SelectCurrent );
        }
        );

    // every time the editor is closed, show the controls
    connect( m_groupEditingDiag, &AddRenameDialog::editorHidden, m_crud_btns, &CRUDcontrols::show );

    connect( this, &ArchWay::wdgRightResized, this, &ArchWay::onWdgRightResized );

}


/****************************************************
 *                  Qt's EVENTS
 ****************************************************/

void ArchWay::closeEvent(QCloseEvent* event)
{
    bool successful = m_jBridge.saveData( m_map );
    if ( successful )
    {
        event->accept();
    } else {
        qDebug() << "Unable to save the data to location: " << m_jBridge.path();
    }
}


void ArchWay::mousePressEvent( QMouseEvent* event )
{
    Q_UNUSED(event)

    bool isDialogVisible{ m_groupEditingDiag->isVisible() };
    bool mouseClickedInside{ m_groupEditingDiag->underMouse() };

    // hide the dialog when mouse is clicked
    // outside of it
    if ( isDialogVisible && !mouseClickedInside )
    {
        m_groupEditingDiag->hide();
    }
}


bool ArchWay::eventFilter( QObject* watched, QEvent* event )
{
    if ( watched == ui->wdgRight && event->type() == QEvent::Resize )
    {
        auto *resizeEvent = static_cast<QResizeEvent*>(event);
        emit wdgRightResized( resizeEvent->size() );
    }

    return QMainWindow::eventFilter(watched, event);
}


/****************************************************
 *              PRIVATE HELPER METHODS
 ****************************************************/

void ArchWay::repositionTopLvlWidget(
    QWidget* topLvlWgd, QWidget* parentWidget,
    int padding_x, int padding_y,
    TopLvlBtnAlignment alignment
    )
{
    auto [ parent_width, parent_height ] = parentWidget->geometry().size();
    padding_x = std::abs( padding_x );
    padding_y = std::abs( padding_y );

    switch ( alignment )
    {
    case TopLvlBtnAlignment::BottomRight:
    {
        // top-left corner
        topLvlWgd->move(
            parent_width - padding_x - topLvlWgd->width(),
            parent_height - padding_y - topLvlWgd->height()
            );
        return;
    }

    case TopLvlBtnAlignment::BottomLeft:
    {
        // top-left corner
        topLvlWgd->move(
            padding_x,
            parent_height - padding_y - topLvlWgd->height()
            );
        return;
    }

    case TopLvlBtnAlignment::TopLeft:
    {
        // top-left corner
        topLvlWgd->move(
            padding_x,
            padding_y
            );
        return;
    }

    case TopLvlBtnAlignment::TopRight:
    {
        // top-left corner
        topLvlWgd->move(
            parent_width - padding_x - topLvlWgd->width(),
            padding_y
            );
        return;
    }

    case TopLvlBtnAlignment::MiddleOffsetted:
    {
        // top-left corner
        topLvlWgd->move(
            (parent_width / 2)  - padding_x - topLvlWgd->width(),
            parent_height - padding_y - topLvlWgd->height()
            );
        return;
    }

    default:
        // invalid alignment.
        // recover to default aligment BottomRight (recursivly)
        this->repositionTopLvlWidget( topLvlWgd, parentWidget, padding_x, padding_y, TopLvlBtnAlignment::BottomRight );
    }
}



/****************************************************
 *              PUBLIC SLOTS
 ****************************************************/


/*!
*    TOP LEVEL CRUD WIDGET
*/

void ArchWay::on_removeGroup()
{

    int newIdx{};
    int tot_elem{ m_lstModel->rowCount() };

    // assert that will be at least 1 item
    // after the delete operation
    if ( tot_elem > 1 )
    {
        // item to be selected after delition
        int curr_idx{ m_lstModel->indexOfGroup( m_lastGrpSelected ) };
        if ( tot_elem == ( curr_idx + 1 ) )
        {
            // select the item that will be the lastIdx
            // after the delete operation
            newIdx = curr_idx - 1 ;
        } else {
            // this will select the item after the current selected
            // after the current will be removed
            newIdx = curr_idx;
        }
    }

    // remove the item
    m_lstModel->removeGroup( m_lastGrpSelected );

    if ( m_lstModel->rowCount() <= 0 )
    {
        // disable rename
        m_crud_btns->setRenameEnabled( false );

        // hide left_widget (groupEditor)
        ui->left_wdg->hide();

        return;
    }

    auto* selectionItemModel = ui->list_group->selectionModel();
    auto model_idx = m_lstModel->index( newIdx, 0 );
    selectionItemModel->select( model_idx, QItemSelectionModel::Select );
    emit selectionItemModel->currentChanged( model_idx, QModelIndex() );
}


void ArchWay::on_addGroupBtnPressed()
{
    m_crud_btns->hide();
    m_groupEditingDiag->showEditorAs("", AddRenameDialog::AddGroup);
}


void ArchWay::on_editGroupBtnPressed()
{
    // after the CRUDcontrols::editBtn is clicked
    // triggers this methods to open the Dialog for modification
    m_crud_btns->hide();
    m_groupEditingDiag->showEditorAs( m_lastGrpSelected, AddRenameDialog::RenameGroup );
}


/****************************************************
 *              PRIVATE SLOTS
 ****************************************************/

void ArchWay::on_editBtnClicked( const QString grName )
{
    qDebug() << "\nEdit group requested for: " << grName;
    // if group is not selected
    // select it
    if ( m_lastGrpSelected != grName )
    {
        // Find the index of the newly inserted group
        int newIdx = m_lstModel->indexOfGroup(grName);
        if (newIdx < 0)
            return; // Safety check

        // select whatever Group Item's "Edit Group" is pressed
        auto* selectionItemModel = ui->list_group->selectionModel();
        auto model_idx = m_lstModel->index( newIdx, 0 );

        selectionItemModel->setCurrentIndex(
            model_idx, QItemSelectionModel::Current | QItemSelectionModel::Select
            );
    }


    // set the left panel visible
    ui->left_wdg->show();
    m_group_editor->show();

    // set the new group to the editor
    m_group_editor->setGroupName( grName );

    QList<PathInfo>& data = m_map[ grName ];
    m_group_editor->setActiveGroup( data );

    // select the first row on the editor
    //-----------------------------------
    m_group_editor->selectFirstTableViewRow();
}


void ArchWay::on_openAllButtonClicked( const QString grName )
{
    qDebug() << "\nOpen all file for group: " << grName;

    // check if item Count > 0
    int curr_idx = m_lstModel->indexOfGroup( grName );
    if ( !(m_lstModel->itemCount( curr_idx ) > 0 ))
    {
        QMessageBox::information(
            this, "No Path to be Opend",
            "This group is empty or has the path setted\nto 'not openable'"
            );
        return;
    }

    // loop throug QList<PathInfo> for open it all
    auto& pInfo_list = m_map[ grName ];
    foreach ( const PathInfo& p, pInfo_list)
    {
        p.openPath();
    }
}


/*!
 * After the SlectionModel::selectionChanged() is emitted
 * this method update the lastGroupSelected and the editor in rename mode if visible
*/
void ArchWay::groupItemSelectionChanged(
    const QModelIndex& current_selection, const QModelIndex& previous_selection
    )
{
    Q_UNUSED(previous_selection)

    m_lastGrpSelected = ui->list_group->model()->data( current_selection, Qt::DisplayRole ).toString();

    // after new group is selected
    // if left_wdg open, update items
    if ( ui->left_wdg->isVisible() )
    {
        // set the new group to the editor
        m_group_editor->setGroupName( m_lastGrpSelected );

        QList<PathInfo>& data = m_map[ m_lastGrpSelected ];
        m_group_editor->setActiveGroup( data );

        m_group_editor->selectFirstTableViewRow();
    }


    if ( m_groupEditingDiag->isVisible() )
    {
        if ( auto mode = m_groupEditingDiag->currentMode(); mode == AddRenameDialog::RenameGroup )
        {
            m_groupEditingDiag->showEditorAs( m_lastGrpSelected, mode );
        }
    }

    qDebug() << "\nNew Selection Changed to: '" << m_lastGrpSelected << "'";
}

/*!
 *  When the TopLavelButton::addGroupClicked emit the
 *  new group name, this slots catch it and add it to the
 *  model and select the item in the view
*/
void ArchWay::insertGroupInView( const QString& grName )
{
    // add the key to the map
    m_lstModel->addGroup( grName );
    m_lastGrpSelected = grName;

    // Find the index of the newly inserted group
    int newIdx = m_lstModel->indexOfGroup(grName);
    if (newIdx < 0)
        return; // Safety check

    auto* selectionItemModel = ui->list_group->selectionModel();
    auto model_idx = m_lstModel->index( newIdx, 0 );

    selectionItemModel->setCurrentIndex(
        model_idx, QItemSelectionModel::Current | QItemSelectionModel::Select
        );

    // re-anable renameBtn
    m_crud_btns->setRenameEnabled( true );
}


void ArchWay::onWdgRightResized( QSize newSize )
{
    Q_UNUSED(newSize)
    this->repositionTopLvlWidget(
        m_crud_btns, ui->wdgRight,
        BTNS_CNTR_PADDING, BTNS_CNTR_PADDING, ArchWay::TopLvlBtnAlignment::BottomRight
        );

    this->repositionTopLvlWidget(
        m_groupEditingDiag, ui->wdgRight,
        BTNS_CNTR_PADDING, BTNS_CNTR_PADDING, ArchWay::TopLvlBtnAlignment::BottomRight
        );
}


