#include "toggleswitchdelegate.h"

ToggleSwitchDelegate::ToggleSwitchDelegate(QObject* parent )
    : QStyledItemDelegate(parent)
{}


ToggleSwitchDelegate::~ToggleSwitchDelegate()
{}


void ToggleSwitchDelegate::paint(
    QPainter *painter,
    const QStyleOptionViewItem &option,
    const QModelIndex &index
) const
{

    if ( index.data().canConvert<bool>() )
    {
        auto is_checked = qvariant_cast<bool>( index.data() );

        // *** DRAW DIRECTLY - NO WIDGET INSTANCE ***

        painter->setRenderHint(QPainter::Antialiasing, true);

        int handle_radius = std::round(0.24 * option.rect.height());
        int w_fx_h = std::round((option.rect.height() * 11.0) / 13.0); // Use 11.0 and 13.0 for floating-point division

        QRectF frame_slider(option.rect.x(), option.rect.y(), w_fx_h - handle_radius, option.rect.height() * 0.40);
        frame_slider.moveCenter(option.rect.center());

        int border_radius = frame_slider.height() / 2;
        int handle_path_len = w_fx_h - handle_radius;
        int x_position = frame_slider.x() + (handle_path_len * (is_checked ? 1.0 : 0.0));

        // Draw Frame (using colors from ViewToggleSw or define them here)
        QPen framePen(is_checked ? QColor(152, 73, 249) : QColor(113, 113, 113), 3);
        QBrush frameBrush(is_checked ? QColor(255, 255, 255) : QColor(207, 207, 207));
        painter->setPen(framePen);
        painter->setBrush(frameBrush);
        painter->drawRoundedRect(frame_slider, border_radius, border_radius);

        // Draw Handle
        QPen handlePen(QColor(81, 80, 102), 3);
        QBrush handleBrush(is_checked ? QColor(161, 86, 253) : QColor(132, 131, 151));
        painter->setPen(handlePen);
        painter->setBrush(handleBrush);
        painter->drawEllipse(QPointF(x_position, option.rect.center().y()), handle_radius, handle_radius);
    }
}


bool ToggleSwitchDelegate::editorEvent(
    QEvent *event,
    QAbstractItemModel *model,
    const QStyleOptionViewItem &option,
    const QModelIndex &index
)
{

    bool idx_isValid = index.isValid();
    bool isCol3_isTypeBool = ( index.column() == 3 && index.data().canConvert<bool>() );
    bool isMouseReleaseEvent = ( event->type() == QEvent::MouseButtonRelease );
    if ( idx_isValid && isCol3_isTypeBool && isMouseReleaseEvent )
    {
        QMouseEvent* mouseEvent = static_cast<QMouseEvent*>( event );
        if ( option.rect.contains(mouseEvent->pos()) )
        {
            bool is_checked = model->data( index, Qt::DisplayRole ).toBool();
            model->setData( index, !is_checked, Qt::EditRole );
            return true; // needed to consume the event
        }

        return true;
    }

    return false;
}


QSize ToggleSwitchDelegate::sizeHint(
    const QStyleOptionViewItem &option,
    const QModelIndex &index
) const
{
    Q_UNUSED(option)

    if ( index.data().canConvert<bool>() )
    {
        return QSize(30, 60);
    }

    return QSize();
}
