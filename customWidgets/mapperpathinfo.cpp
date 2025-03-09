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

#include "customWidgets/mapperpathinfo.h"
#include "ui_mapperpathinfo.h"


/*!----------------------------------------------------+
 *                                                     |
 *      @author: Andrea Venturelli                     |
 *      @mail  : aventurelli.github@gmail.com          |
 *                                                     |
 *-----------------------------------------------------+
*/


MapperPathInfo::MapperPathInfo(PathsInfoTableModel* model, QWidget* parent)
    : QWidget(parent), ui(new Ui::MapperPathInfo), m_model{ model }, p_info{ new PathInfo }
{
    ui->setupUi(this);
    ui->full_path_pte->setWordWrapMode(QTextOption::WrapAnywhere);

    m_mapper = new QDataWidgetMapper( this );
    m_mapper->setOrientation( Qt::Horizontal );
    m_mapper->setModel( m_model );

    // widgets mapping
    m_mapper->addMapping( ui->full_path_pte,  ColDescription::FilePath );
    m_mapper->addMapping( validity_chip,      ColDescription::IsValidChip );
    m_mapper->addMapping( toggle_sw,          ColDescription::IsOpenable );

    m_mapper->setCurrentIndex(0);

    // add custom widgets to UI
    //toggle_sw->setMinimumWidth(80);
    toggle_sw->setMinimumHeight(60);

    ui->state_hlay->insertWidget(1, validity_chip);
    ui->buttons_vlay->addWidget(toggle_sw, 0);

    // connecting Toggle_sw toggled signal to slot openableToggled
    // sync the state with the model and with the view (as a side effect)
    connect( toggle_sw, &ViewToggleSw::toggled, this, &MapperPathInfo::openableToggled );
}


MapperPathInfo::~MapperPathInfo()
{
    delete p_info;
    delete ui;
}


void MapperPathInfo::setMapperIdx( int row )
{
    // Handle empty model or invalid row
    if ( row < 0 || row >= m_model->rowCount() )
    {
        ui->full_path_pte->setPlainText(" ");
        toggle_sw->setChecked( false );
        toggle_sw->setEnabled( false );
        validity_chip->setValidityState( false );
        return;
    }

    // Update the mapper only if the row is valid
    toggle_sw->setEnabled( true );
    m_mapper->setCurrentIndex( row );
}


/********************************************************************************
 *
 *                  helper methods to retrive model's indexes
 *
 *********************************************************************************/
int MapperPathInfo::currentModelRow()
{
    return m_mapper->currentIndex();
}


QModelIndex MapperPathInfo::currentModelIndex( int row, int col )
{
    return m_model->index( row, col );
}


QString MapperPathInfo::currentPath()
{
    auto curr_row = this->currentModelRow();
    // no model set
    if ( curr_row == -1 ) return QString();


    QModelIndex idx = this->currentModelIndex( curr_row, ColDescription::FilePath );
    return m_model->data( idx, Qt::DisplayRole ).toString();
}



void MapperPathInfo::setPathTypePix( QPixmap pix )
{
    // scaled to 90% of the height of the label
    pix = pix.scaledToHeight( ui->path_type_icon->height() * 0.9, Qt::SmoothTransformation );

    ui->path_type_icon->setPixmap( pix );
}



/********************************************************************************
 *
 *                                   mapper's slots
 *
 *********************************************************************************/


void MapperPathInfo::on_open_path_btn_clicked()
{
    auto curr_path = this->currentPath();
    if ( curr_path.isEmpty() )
    {
        QMessageBox::warning(this, "Invalid Row", "Please select a row from the table");
        return;
    }

    p_info->changePath( curr_path );
    if ( !p_info->pathValidity() )
    {
        QMessageBox::warning(this, "Invalid Path", "Can't Open a not existing path");
        return;
    }

    p_info->openPath( true );
}


void MapperPathInfo::on_change_path_btn_clicked()
{

    // get current path
    // "" if is invalid
    QString old_path = this->currentPath();

    // checks index's validity
    if ( old_path.isEmpty() )
    {
        QMessageBox::warning(this, "Invalid Row", "Please select a row from the table");
        return;
    }

    // get the parent dir of the
    // path as the default_path
    p_info->changePath( old_path );
    auto default_path = ( QFileInfo::exists(p_info->absPath()) ) ? p_info->absPath() : QDir::homePath();

    // prompt the user to choose a new path
    QString newPath = QFileDialog::getOpenFileName(this, "Select new Path", default_path );
    if ( !newPath.isEmpty() && newPath != old_path )
    {
        QVariant newPathVariant = QVariant( newPath );

        QModelIndex idx = this->currentModelIndex( this->currentModelRow(), ColDescription::FilePath );
        m_model->setData( idx, newPathVariant, Qt::EditRole );
    }
}


void MapperPathInfo::on_check_path_btn_clicked()
{
    int curr_row = this->currentModelRow();

    if ( curr_row == -1 )
    {
        QMessageBox::warning(this, "Invalid Row", "Please select a row from the table");
        return;
    }

    auto idx = this->currentModelIndex( curr_row, ColDescription::IsValidChip );

    // let the model performe the check
    // and update the chips
    m_model->setData( idx, QVariant(true),  Qt::DisplayRole );
}


void MapperPathInfo::openableToggled( bool checked )
{
    int curr_row = this->currentModelRow();

    if ( curr_row == -1 )
    {
        return;
    }

    // updates the model's 3 column
    auto idx = currentModelIndex( curr_row, ColDescription::IsOpenable );
    QVariant toggle_sw_state = QVariant( checked );
    m_model->setData( idx, toggle_sw_state, Qt::EditRole );
}


void MapperPathInfo::on_rowSelectionChanged( const QModelIndex& current, const QModelIndex& previous )
{
    Q_UNUSED(previous)

    auto new_row = current.row();

    m_mapper->setCurrentIndex( new_row );
    auto isValid_idx = m_model->index( new_row, ColDescription::IsValidChip );

    QPixmap pix_filetype( p_info->NOT_A_FILE_PIX );

    if ( m_model->data( isValid_idx ).toBool() )
    {
        // set icons to file || dir
        auto isFile_idx = m_model->index( new_row, ColDescription::FileType );
        pix_filetype = ( m_model->data( isFile_idx ).toBool() ) ? p_info->FILE_PIX : p_info->FOLDER_PIX ;
    }

    auto lbl_height = ui->path_type_icon->height();
    pix_filetype = pix_filetype.scaledToHeight( lbl_height - (lbl_height / 10 ), Qt::SmoothTransformation );

    ui->path_type_icon->setPixmap( pix_filetype );
}


void MapperPathInfo::on_full_path_pte_textChanged()
{
    // when the the plain-text-edit is changed,
    // this method will update the file name Label
    // accordingly with the path

    // take the text from the plainTextEdit
    auto new_path = ui->full_path_pte->toPlainText();
    p_info->changePath( new_path );

    // use the PathInfo::fileName() to keep only
    // the name
    ui->path_name_lbl->setText( p_info->fileName() );
}

