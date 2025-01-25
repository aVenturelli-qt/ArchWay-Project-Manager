#ifndef GROUPEDITOR_H
#define GROUPEDITOR_H

#include <QWidget>
#include <QFileDialog>


#include "pathsinfotablemodel.h"
#include "customWidgets/mapperpathinfo.h"


namespace Ui {
class GroupEditor;
}

class GroupEditor : public QWidget
{
    Q_OBJECT

public:
    explicit GroupEditor(PathsInfoTableModel* model, QWidget *parent = nullptr);
    ~GroupEditor();

    const QString groupName() const;
    void setGroupName( const QString& name );

private slots:
    /*!
     * DESCRIPTION:
     * \n - Add a new row to the model and call
     * \n   the FileSystem to select a path
     */
    void on_ctrl_new_btn_clicked();

    /*!
     * DESCRIPTION:
     * \n - for every row in the model
     * \n   checks for every path, the "isValid()"
     * \n   member has changed
     */
    void on_ctrl_validity_btn_clicked();

    /*!
     * DESCRIPTION:
     * \n - Delete selected rows from the model
     */
    void on_ctrl_delete_btn_clicked();

    void on_modelResetted();

private:
    Ui::GroupEditor *ui;
    MapperPathInfo* m_mapper_wdg;
    PathsInfoTableModel* m_model;
};

#endif // GROUPEDITOR_H
