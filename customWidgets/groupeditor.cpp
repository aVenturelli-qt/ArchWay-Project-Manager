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

#include "groupeditor.h"
#include "ui_groupeditor.h"

#include "delegates/toggleswitchdelegate.h"
#include "delegates/pathinfotabledelegate.h"

/*!----------------------------------------------------+
 *                                                     |
 *      @author: Andrea Venturelli                     |
 *      @mail  : aventurelli.github@gmail.com          |
 *                                                     |
 *-----------------------------------------------------+
*/


GroupEditor::GroupEditor(PathsInfoTableModel* model, QWidget *parent)
    : QWidget(parent), ui(new Ui::GroupEditor), m_model{ model }
{
    ui->setupUi(this);
    ui->path_info_mapper->setLayout( new QVBoxLayout() );

    // bottom mapper widget
    m_mapper_wdg = new MapperPathInfo( m_model );
    ui->path_info_mapper->layout()->addWidget( m_mapper_wdg );

    // customize TableView behavior
    this->_setupTableView();

    // create the pathSelector Dialog -> showed on "CreateNewBtn_clicked"
    m_dialog = new PathTypeSelectorDialog( this );
    m_dialog->setMinimumSize( 240, 240 );
    m_dialog->hide();

    connect( m_dialog, &PathTypeSelectorDialog::pathTypeRequested, this, &GroupEditor::selectPath );

    qApp->installEventFilter( this );
}


void GroupEditor::_setupTableView()
{
    // table view customization
    ui->table_view->setModel( m_model );
    ui->table_view->verticalHeader()->setDefaultSectionSize(64);
    ui->table_view->horizontalHeader()->setFixedHeight( 56 );
    ui->table_view->setSelectionBehavior(QAbstractItemView::SelectionBehavior::SelectRows);
    ui->table_view->setSelectionMode( QAbstractItemView::SingleSelection );


    // setting toggle switch delegate for col 3
    auto* toggle_sw_delegate = new ToggleSwitchDelegate( ui->table_view );
    ui->table_view->setItemDelegateForColumn( 0, new PathInfoTableDelegate );     // path icon centered
    ui->table_view->setItemDelegateForColumn( 2, new PathInfoTableDelegate );     // chip (valid/invalid) centered
    ui->table_view->setItemDelegateForColumn( 3, toggle_sw_delegate );            // toggle switch for "isOpenable"


    // when the user double-click the path cell, will
    // be asked to choose a new path to replace the old with
    connect(
        ui->table_view, &QTableView::doubleClicked,
        this, &GroupEditor::on_TblViewDoubleClicked );

    //---------------------------------------------------------------------------------

    // update the mapper's index whenever the row in the view change
    auto* selectionModel = ui->table_view->selectionModel();
    connect(
        selectionModel, &QItemSelectionModel::currentChanged,
        this, &GroupEditor::updateMapperValues
        );

}


GroupEditor::~GroupEditor()
{
    delete ui;
}


/********************************************************
 *                getters and setter
 *******************************************************/

const QString GroupEditor::groupName() const
{
    return ui->group_name_lbl->text();
}


void GroupEditor::setGroupName( const QString& name )
{
    ui->group_name_lbl->setText( name );
}


void GroupEditor::setActiveGroup( QList<PathInfo>& data )
{
    // ask the model to change is internal data
    // and the mapper and editor data is automatically
    // refreshed
    m_model->setListData( &data );
}


/********************************************************
 *                  private slots
 *******************************************************/

void GroupEditor::on_ctrl_new_btn_clicked()
{
    // get the bottom left corner of AddNew
    // to place the dialog underneath
    auto GlobPos = ui->ctrl_new_btn->mapToGlobal( ui->ctrl_new_btn->rect().bottomLeft() );
    auto localPos = this->mapFromGlobal( GlobPos );

    m_dialog->move( localPos.x(), localPos.y() + 8 );
    m_dialog->show();
}


/*
 *  This function is called from the PathTypeSelectorDialog
 *  and prompt the user to select either a Dir or a file
 *  cause Qt doesn't have a method to select one or the other
 *  in one go
*/
QString GroupEditor::selectPath( PathTypeSelectorDialog::PathType pathType )
{
    QString newPath{};

    if ( pathType ==  PathTypeSelectorDialog::PathType::File )
    {
        newPath = QFileDialog::getOpenFileName( this, "Select a path", QDir::homePath() );
    } else {
        newPath = QFileDialog::getExistingDirectory( this, "Select a Directory",  QDir::homePath() );
    }

    // canceled operation
    if ( newPath.isEmpty() )
        return QString();

    // duplicate file
    if ( m_model->getPathInfoList()->contains( PathInfo( newPath ) ) )
    {
        QString message = QString( "the path ' %1 ' is already present" ).arg( newPath );
        QMessageBox::warning( this, "Duplicate path", message );
        return QString();
    }

    this->addPathToModel( newPath );
    return newPath;
}


void GroupEditor::addPathToModel( QString path )
{
    // add one row to the end
    m_model->insertRows( m_model->rowCount(), 1 );
    // fill the row
    m_model->fillNewRow( m_model->rowCount() - 1, path );  // with 4 elemnts [0 to 3], rowCount return 4

    // select the last inserted row.
    // if model is empty, rowCount() equals 0.
    // adding a row results in rowCount() equals 1, but the value is at index 0; so rowCount() -1
    const auto idx = m_model->index( m_model->rowCount() - 1, 0 );

    ui->table_view->setCurrentIndex( idx );
    ui->table_view->selectionModel()->select( idx, QItemSelectionModel::ClearAndSelect | QItemSelectionModel::Rows );
    ui->table_view->scrollTo( idx );
}


void GroupEditor::on_ctrl_validity_btn_clicked()
{
    const int validityCol{ 2 };

    // loop every ModelIndex row at col 2
    for ( int row = 0; row < m_model->rowCount(); ++row )
    {
        const QModelIndex& idx = m_model->index( row, validityCol );

        // the logic to check whether the path is still valid or no
        // is performed inside the model's setData() method
        // that also refresh the data of pathType (col 0) and isValid ( col 2)
        m_model->setData( idx, QVariant( false), Qt::EditRole );
    }
}


void GroupEditor::on_ctrl_delete_btn_clicked()
{
    QModelIndex current_idx = ui->table_view->currentIndex();
    if (!current_idx.isValid()) return;

    // remove one row from the model
    int row_to_remove = current_idx.row();
    m_model->removeRows( row_to_remove, 1 );

    // case zero items remained
    if ( m_model->rowCount() == 0 )
    {
        m_mapper_wdg->setMapperIdx(-1); // Clear the mapper
        return;
    }

    // Determine the new current row:
    // - If the last row was removed, select the new last row
    // - Otherwise, select the same row (if still valid)
    int new_row = row_to_remove;
    if ( row_to_remove >= m_model->rowCount() ) {
        new_row = m_model->rowCount() - 1;
    }

    // Set the new current index and select it
    QModelIndex new_idx = m_model->index(new_row, 0); // Use column 0 for stability
    ui->table_view->setCurrentIndex(new_idx);
    ui->table_view->selectionModel()->select(
        new_idx,
        QItemSelectionModel::ClearAndSelect | QItemSelectionModel::Rows
        );
    ui->table_view->scrollTo(new_idx);
}


void GroupEditor::updateMapperValues(const QModelIndex& new_selection, const QModelIndex& previous_selection )
{
    Q_UNUSED( previous_selection );

    // synch the TableView's selected row
    // with the Mapper
    int current_row = new_selection.row();
    m_mapper_wdg->setMapperIdx( current_row );


    // get the img PathType
    auto pix_path = m_model->data( new_selection, Qt::UserRole ).toString();
    QPixmap pathTypePix( pix_path ) ;
    m_mapper_wdg->setPathTypePix( pathTypePix );

}


void GroupEditor::on_closeGroupBtn_clicked()
{
    emit hideEditor();
}


void GroupEditor::on_TblViewDoubleClicked( const QModelIndex& idx )
{
    if ( !idx.isValid() ||  idx.column() != 1 ) return;

    // retrive model and check is valid
    auto* model = qobject_cast<PathsInfoTableModel*>( ui->table_view->model() );
    if ( !model ) return;

    // get current path in the table
    QString current_path = model->data( idx, Qt::DisplayRole ).toString();

    // if the parent of current path exist, use
    // it as default location else, Home
    PathInfo path_info( current_path );
    auto default_path = (QFileInfo::exists(path_info.absPath())) ? path_info.absPath() : QDir::homePath();

    // ask for a new path
    QString newPath = QFileDialog::getOpenFileName(this, "Select new Path", default_path );
    if ( !newPath.isEmpty() && newPath != current_path )
    {
        QVariant newPathVariant = QVariant( newPath );
        model->setData( idx, newPathVariant, Qt::EditRole );
    }
}



/********************************************************
 *              public support methods
 *******************************************************/

// doesn't highlight the selected row
void GroupEditor::selectFirstTableViewRow()
{
    // the model must be valid and have
    // at least 1 item
    if ( !m_model || m_model->rowCount() == 0 )
    {
        ui->table_view->setCurrentIndex( QModelIndex() );
        m_mapper_wdg->setMapperIdx( -1 );
        return;
    }


    // select first row
    //----------------------------------------------
    const QModelIndex& first_idx = m_model->index( 0, 0 );

    ui->table_view->setCurrentIndex( first_idx );

    // Select and set current index
    ui->table_view->selectionModel()->setCurrentIndex(first_idx, QItemSelectionModel::Current | QItemSelectionModel::SelectCurrent);
    ui->table_view->setCurrentIndex(first_idx);
}


/********************************************************
 *              EVENTS methods
 *******************************************************/

void GroupEditor::resizeEvent( QResizeEvent* event )
{
    // set the colums' width to take
    // all the available space
    int tot_width = ui->table_view->width();
    int pathColWidgt =  tot_width / 5 * 3;

    ui->table_view->setColumnWidth( 1, pathColWidgt );
    tot_width -= pathColWidgt;

    ui->table_view->setColumnWidth( 0, tot_width / 3 - 16 );
    ui->table_view->setColumnWidth( 2, tot_width / 3 + 16 );
    ui->table_view->setColumnWidth( 3, tot_width / 3 );

    QWidget::resizeEvent( event );
}


bool GroupEditor::eventFilter( QObject* obj, QEvent* event )
{
    // ignore events that are not MouseButtonPress
    if ( event->type() != QEvent::MouseButtonPress )
        return false;

    QMouseEvent* mouseEvent = static_cast<QMouseEvent *>( event );

    if ( m_dialog->isVisible() )
    {
        // Convert global click position to myWidget's local coordinates
        QPoint widgetPos = m_dialog->mapFromGlobal( mouseEvent->globalPosition().toPoint() );

        // Check if click is outside myWidget
        if ( !m_dialog->rect().contains(widgetPos) )
        {
            m_dialog->hide();  // Hide if clicked outside
        }
    }

    return QWidget::eventFilter( obj, event );
}
