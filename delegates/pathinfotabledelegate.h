#ifndef PATHINFOTABLEDELEGATE_H
#define PATHINFOTABLEDELEGATE_H

#include <QStyledItemDelegate>
#include <QWidget>
#include <QPainter>
#include <QColor>
#include <QFontMetrics>

class PathInfoTableDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    explicit PathInfoTableDelegate( QWidget* parent = nullptr );

    void paint(QPainter* painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override;

private:
    static constexpr char FILE_PIX_PATH[]    { ":/icons/pathEditor/asset/icons/file_icon.svg" };
    static constexpr char DIR_PIX_PATH[]     { ":/icons/pathEditor/asset/icons/folder_icon.svg" };
    static constexpr char INVALID_PIX_PATH[] { ":/icons/pathEditor/asset/icons/not_existing_icon.svg" };

    static constexpr int PADDING_LEFT_RIGHT{ 8 };
    static constexpr int PADDING_TOP_BOTTOM{ 12 };

    static const QColor VALID_COL  ;
    static const QColor INVALID_COL;
};


#endif // PATHINFOTABLEDELEGATE_H
