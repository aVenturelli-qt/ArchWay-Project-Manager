#include "viewtogglesw.h"


ViewToggleSw::ViewToggleSw(QWidget *parent)
    : QCheckBox{parent}
{
    // setup checkbox and size
    this->setCheckable(true);
    setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);

    setMouseTracking(true);
}


//-------------------- copy constructor -----------------------------
ViewToggleSw::ViewToggleSw( const ViewToggleSw& other)
    : QCheckBox{ qobject_cast<QWidget*>( other.parent() ) }
{
    // setup checkbox and size
    this->setCheckable(true);
    this->setChecked( other.isChecked() );

    setMouseTracking(true);
}


//------------------ copy assignment ---------------------------------
ViewToggleSw& ViewToggleSw::operator=(const ViewToggleSw& other)
{
    if ( this == &other ) return *this;

    this->setChecked( other.isChecked() );
    return *this;
}


bool ViewToggleSw::hitButton( const QPoint& pos ) const
{
    return this->contentsRect().contains(pos);
}


void ViewToggleSw::setPainterBrushPen(QPainter* p, QPen& pen, QBrush& brush) const
{
    p->setPen( pen );
    p->setBrush( brush );
}

/*********************************
 *
 *        PUBLIC METHODS
 *
 * *******************************/
QSize ViewToggleSw::sizeHint() const
{
    return QSize(30, 60);
}


void ViewToggleSw::paintEvent(QPaintEvent* event)
{
    Q_UNUSED(event)
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);

    int handle_radius = std::round(0.24 * rect().height());
    int w_fx_h = std::round((rect().height() * 11.0) / 13.0); // Use 11.0 and 13.0 for floating-point division

    QRectF frame_slider(
        rect().x(),
        rect().y(),
        w_fx_h - handle_radius,
        rect().height() * 0.40
    );
    frame_slider.moveCenter(rect().center());

    int border_radius = frame_slider.height() / 2;
    int handle_path_len = w_fx_h - handle_radius;
    int x_position = frame_slider.x() + (handle_path_len * (this->isChecked() ? 1.0 : 0.0));

    // Draw Frame (using colors from ViewToggleSw or define them here)
    QPen framePen(
        this->isChecked() ?
        QColor(152, 73, 249) : QColor(113, 113, 113), 3
    );

    QBrush frameBrush(
        this->isChecked() ?
        QColor(255, 255, 255) : QColor(207, 207, 207)
    );

    painter.setPen(framePen);
    painter.setBrush(frameBrush);
    painter.drawRoundedRect(frame_slider, border_radius, border_radius);

    // Draw Handle
    QPen handlePen(QColor(81, 80, 102), 3);
    QBrush handleBrush(
        this->isChecked() ?
        QColor(161, 86, 253) : QColor(132, 131, 151)
    );
    painter.setPen(handlePen);
    painter.setBrush(handleBrush);
    painter.drawEllipse(
        QPointF(x_position, rect().center().y()),
        handle_radius, handle_radius
    );
}
