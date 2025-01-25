#ifndef PATHSINFOTABLEMODEL_H
#define PATHSINFOTABLEMODEL_H

#include <QObject>
#include <QWidget>
#include <QAbstractTableModel>
#include <QList>
#include <QDebug>
#include <QIcon>

#include "customWidgets/pathinfo.h"

class PathsInfoTableModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    explicit PathsInfoTableModel( QList<PathInfo> group= QList<PathInfo>(), QObject *parent = nullptr );


    // virtual override
    int rowCount( const QModelIndex &parent = QModelIndex() ) const override;
    int columnCount( const QModelIndex &parent = QModelIndex() ) const override;

    QVariant data( const QModelIndex &index, int role = Qt::DisplayRole ) const override;
    bool setData( const QModelIndex& index, const QVariant& value, int role ) override;

    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;

    Qt::ItemFlags flags(const QModelIndex &index) const override;

    QList<PathInfo>& getPathInfoList() { return m_model_data; }
    void setListData( QList<PathInfo>& new_paths_list );

private:
    QList<PathInfo> m_model_data;

};

#endif // PATHSINFOTABLEMODEL_H
