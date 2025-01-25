#include "archway.h"
#include "ui_archway.h"

ArchWay::ArchWay(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::ArchWay), m_dictionary{ new QMap<QString, QList<PathInfo>>() }
{
    ui->setupUi(this);
    ui->left_wdg->setLayout( new QVBoxLayout );

    QString grp_1("Group1");
    auto lst_1 = this->createList1();

    QString grp_2("Group2");
    auto lst_2 = this->createList2();

    QString grp_3("Group3");
    auto lst_3 = this->createList3();

    m_dictionary->insert(grp_1, lst_1);
    m_dictionary->insert(grp_2, lst_2);
    m_dictionary->insert(grp_3, lst_3);

    // create the model
    auto& model = this->getListByIndex( m_dictionary, 0);
    m_model = new PathsInfoTableModel( model , this );

    // istanciate the groupEditor_widget
    //m_group_editor = new GroupEditor( m_model, this );
    //ui->left_wdg->layout()->addWidget( m_group_editor );


    // connect the group_list changed signal to the groupEditor
    //! when the user select another item in the list, update the QList<PathInfo> of the model
}

ArchWay::~ArchWay()
{
    delete m_dictionary;
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


void ArchWay::updateModelData( QList<PathInfo>& data )
{
    m_model->setListData( data );
}


QString ArchWay::getKeyByIndex(const QMap<QString, QList<PathInfo>>* map, int index) const
{
    if (index < 0 || index >= map->size()) {
        qWarning() << "Index out of bounds";
        return QString();
    }

    auto it = map->begin();
    std::advance(it, index);
    return it.key();
}


QList<PathInfo>& ArchWay::getListByIndex(QMap<QString, QList<PathInfo>>* map, int index) const
{
    if (index < 0 || index >= map->size()) {
        qWarning() << "Index out of bounds";
        static QList<PathInfo> empty;
        return empty;
    }

    auto it = map->begin();
    std::advance(it, index);
    return it.value();
}


void ArchWay::on_testGroup1_clicked()
{
    auto group = this->getKeyByIndex( m_dictionary, 0);
    //this->m_group_editor->setGroupName( group );

    this->updateModelData( (*m_dictionary)[group] );
}


void ArchWay::on_testGroup2_clicked()
{
    auto group = this->getKeyByIndex( m_dictionary, 1);
    //this->m_group_editor->setGroupName( group );

    this->updateModelData( (*m_dictionary)[group] );
}


void ArchWay::on_testGroup3_clicked()
{

    auto group = this->getKeyByIndex( m_dictionary, 2);
    //this->m_group_editor->setGroupName( group );

    this->updateModelData( (*m_dictionary)[group] );
}


QList<PathInfo> ArchWay::createList1() const
{
    PathInfo info(QString("C:\\Users\\test\\Downloads\\12 weeks scoreboard.pdf"));

    QList<PathInfo> p_inf;
    p_inf << info;
    return p_inf;
}


QList<PathInfo> ArchWay::createList2() const
{
    PathInfo info(QString("C:\\Users\\test\\OneDrive\\Documenti\\Desktop\\dati buste paga.xlsx"));
    PathInfo info2(QString("C:\\Users\\test\\OneDrive\\Documenti\\Desktop\\QuickRunPro"));
    PathInfo info3(QString("C:\\Users\\test\\OneDrive\\Doci"));

    QList<PathInfo> p_inf;
    p_inf << info << info2 << info3;

    return p_inf;
}


QList<PathInfo> ArchWay::createList3() const
{
    PathInfo info(QString("C:\\Users\\test\\Downloads\\mic.png"));
    PathInfo info1(QString("C:\\Users\\test\\Downloads"));
    PathInfo info2(QString("C:\\Users\\test\\Documenti"));
    PathInfo info3(QString("C:\\Users\\test\\OneDrive\\Documenti\\Books\\VB Scripting For Catia V5 (Nick Weisenberger) (z-lib.org).pdf"));
    PathInfo info4(QString("C:\\Users\\test\\OneDrive"));

    QList<PathInfo> p_inf;
    p_inf << info << info2 << info3 << info4;

    return p_inf;
}






void ArchWay::on_testToggleEditor_toggled(bool checked)
{
    this->m_group_editor->setVisible( checked );
}

