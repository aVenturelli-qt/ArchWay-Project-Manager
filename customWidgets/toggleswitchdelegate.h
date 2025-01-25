#ifndef TOGGLESWITCHDELEGATE_H
#define TOGGLESWITCHDELEGATE_H
#include <QStyledItemDelegate>

#include <QPainter>
#include <QPaintEvent>
#include <QMouseEvent>

class ToggleSwitchDelegate: public QStyledItemDelegate
{
    Q_OBJECT
public:
    ToggleSwitchDelegate(QObject* parent);
    ~ToggleSwitchDelegate();

    QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const override;

    void paint(QPainter* painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
    bool editorEvent(QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option, const QModelIndex &index) override;
};

#endif // TOGGLESWITCHDELEGATE_H
