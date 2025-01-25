#include "groupeditor.h"
#include "ui_groupeditor.h"

#include "customWidgets/toggleswitchdelegate.h"

GroupEditor::GroupEditor(PathsInfoTableModel* model, QWidget *parent)
    : QWidget(parent), ui(new Ui::GroupEditor)
{
    ui->setupUi(this);
    ui->path_info_mapper->setLayout( new QVBoxLayout() );

    m_mapper_wdg = new MapperPathInfo( m_model, this );
    ui->path_info_mapper->layout()->addWidget( m_mapper_wdg );


    // table view customization
    ui->table_view->setModel( model );
    ui->table_view->verticalHeader()->setDefaultSectionSize(64);
    ui->table_view->horizontalHeader()->setFixedHeight( 56 );
    ui->table_view->setSelectionBehavior(QAbstractItemView::SelectionBehavior::SelectRows);

    // update the mapper's index whenever the row in the view change
    auto* selection_model = ui->table_view->selectionModel();
    connect(
        selection_model, &QItemSelectionModel::currentRowChanged,
        m_mapper_wdg, &MapperPathInfo::on_rowSelectionChanged
    );

    // setting toggle switch delegate for col 3
    auto* toggle_sw_delegate = new ToggleSwitchDelegate( ui->table_view );
    ui->table_view->setItemDelegateForColumn( 3, toggle_sw_delegate );

    // when the model reset, select the first item
    connect( ui->table_view->model(), &PathsInfoTableModel::modelReset, this, &GroupEditor::on_modelResetted );

    // when the user double-click the path cell, will
    // is asked to choose a new path to substitute the old with
    connect(
        ui->table_view, &QTableView::doubleClicked,
        this, [this] ( const QModelIndex& index ) {
            if ( !index.isValid() ) return;

            if ( index.column() == 1 )
            {
                auto* model = qobject_cast<PathsInfoTableModel*>( ui->table_view->model() );
                if ( !model ) return;

                QString current_path = model->data( index, Qt::DisplayRole ).toString();

                // if the parent of current path exist, use
                // it as default location else, Home
                PathInfo path_info( current_path );
                auto default_path = (QFileInfo::exists(path_info.absPath())) ? path_info.absPath() : QDir::homePath();

                QString newPath = QFileDialog::getOpenFileName(this, "Select new Path", default_path );
                if ( !newPath.isEmpty() && newPath != current_path )
                {
                    QVariant newPathVariant = QVariant( newPath );
                    model->setData( index, newPathVariant, Qt::EditRole );
                }
            }
        }
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


/********************************************************
 *                  private slots
 *******************************************************/
void GroupEditor::on_ctrl_new_btn_clicked()
{

}


void GroupEditor::on_ctrl_validity_btn_clicked()
{

}


void GroupEditor::on_ctrl_delete_btn_clicked()
{
    //! @bold must implement the model's deleteRows() method before
}


void GroupEditor::on_modelResetted()
{
    auto idx0 = ui->table_view->model()->index(0 , 0);
    qDebug() << "\n-----------> index 0 is: \t" << idx0.isValid();

    if ( ui->table_view->model() && ui->table_view->model()->rowCount() > 0)
        // ui->table_view->selectRow(0);
        return;
}
