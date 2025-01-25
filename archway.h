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

    void on_testToggleEditor_toggled(bool checked);

private:
    Ui::ArchWay* ui;
    PathsInfoTableModel* m_model;
    QMap<QString, QList<PathInfo>>* m_dictionary;
    GroupEditor* m_group_editor;

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

    void updateModelData( QList<PathInfo>& data );


    QString getKeyByIndex(const QMap<QString, QList<PathInfo>>* map, int index) const;
    QList<PathInfo>& getListByIndex(QMap<QString, QList<PathInfo>>* map, int index) const;

    QList<PathInfo> createList1() const;
    QList<PathInfo> createList2() const;
    QList<PathInfo> createList3() const;

};
#endif // ARCHWAY_H
