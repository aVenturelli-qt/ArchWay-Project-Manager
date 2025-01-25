#include "statechip.h"

StateChip::StateChip( QWidget *parent )
    : QWidget{parent}
{}


void StateChip::paintEvent(QPaintEvent* event)
{
    Q_UNUSED(event)

    QPainter p( this );
    p.setRenderHints( QPainter::Antialiasing | QPainter::TextAntialiasing );

    auto frame_rect = this->rect();

    // equivalent to frame_rect - padding
    auto chip_rect = QRect(
                        frame_rect.left(),
                        frame_rect.top(),
                        frame_rect.width() - (2 * PADDING_LEFT_RIGHT),
                        frame_rect.height() - (2 * PADDING_TOP_BOTTOM)
        );

    // move the chip rect to center
    chip_rect.moveCenter( frame_rect.center() );

    // draw chip with correct color
    auto chip_bg_col = ( m_isValid ) ? VALID_COL : INVALID_COL;
    p.setBrush( chip_bg_col );
    p.setPen( chip_bg_col.color().darker() );

    int border_rad = chip_rect.height() / 2;
    p.drawRoundedRect( chip_rect, border_rad, border_rad);

    // draw text centered
    p.setBrush(Qt::white);
    p.setPen(Qt::white);

    QString chip_text = ( m_isValid ) ? QString("Valid") : QString("Invalid");
    p.setFont( QFont("Inter", 16, 650) );

    QFontMetrics fnt_mtrx( p.font() );
    QRect fnt_rect = fnt_mtrx.boundingRect( chip_text );
    fnt_rect.moveCenter( chip_rect.center() );

    p.drawText( (fnt_rect.topLeft() + QPoint(0, fnt_mtrx.ascent()) ), chip_text );


    p.end();
}


QSize StateChip::sizeHint() const
{
    /*QString chip_text = ( m_isValid ) ? QString("Valid") : QString("Invalid");
    auto font_text = QFont( "Inter", 16, 550 );

    QFontMetrics fnt_mtrx( font_text );
    QRect fnt_rect = fnt_mtrx.boundingRect( chip_text );-*/
    return QSize(
        rect().width() + 12,
        rect().height() + 16
    );
}

void StateChip::setValidityState( bool isValid )
{
    // check old state
    auto old_state = m_isValid;

    m_isValid = isValid;

    // if they are different emit signal
    if ( old_state != isValid )
    {
        // re-paint
        this->update();

        // notify state changed
        emit stateChanged( isValid );
    }

}
