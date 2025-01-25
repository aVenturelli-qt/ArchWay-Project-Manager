#include "mapperpathinfo.h"
#include "ui_mapperpathinfo.h"
#include "./mapperpathinfo.h"


MapperPathInfo::MapperPathInfo(PathsInfoTableModel* model, QWidget* parent)
    : QWidget(parent), ui(new Ui::MapperPathInfo), m_model{ model }
{
    ui->setupUi(this);

    m_mapper = new QDataWidgetMapper(this);
    m_mapper->setModel( m_model );
    m_mapper->addMapping( ui->full_path_pte,  ColDescription::FilePath );
    m_mapper->addMapping( validity_chip,      ColDescription::IsValidChip );
    m_mapper->addMapping( toggle_sw,          ColDescription::IsOpenable );

    // add custom widgets to UI
    toggle_sw->setMinimumWidth(80);

    ui->state_hlay->insertWidget(1, validity_chip);
    ui->buttons_vlay->addWidget(toggle_sw, 0);

    // connecting Toggle_sw toggled signal to slot openableToggled
    // sync the state with the model and with the view (as a side effect)
    connect( toggle_sw, &ViewToggleSw::toggled, this, &MapperPathInfo::openableToggled );
}


MapperPathInfo::~MapperPathInfo()
{
    delete ui;
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
    m_model->setData( idx, QVariant(true), Qt::DisplayRole );
}


void MapperPathInfo::openableToggled(bool checked)
{
    int curr_row = this->currentModelRow();

    if ( curr_row == -1 )
    {
        QMessageBox::warning(this, "Invalid Row", "Please select a row from the table");
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













