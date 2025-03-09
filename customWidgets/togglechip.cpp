/****************************************************************************
**
** Copyright (C) 2012 Nokia Corporation and/or its subsidiary(-ies).
** Contact: http://www.qt-project.org/
**
** This file is part of the QtSql module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL$
** GNU Lesser General Public License Usage
** This file may be used under the terms of the GNU Lesser General Public
** License version 2.1 as published by the Free Software Foundation and
** appearing in the file LICENSE.LGPL included in the packaging of this
** file. Please review the following information to ensure the GNU Lesser
** General Public License version 2.1 requirements will be met:
** http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** In addition, as a special exception, Nokia gives you certain additional
** rights. These rights are described in the Nokia Qt LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU General
** Public License version 3.0 as published by the Free Software Foundation
** and appearing in the file LICENSE.GPL included in the packaging of this
** file. Please review the following information to ensure the GNU General
** Public License version 3.0 requirements will be met:
** http://www.gnu.org/copyleft/gpl.html.
**
** Other Usage
** Alternatively, this file may be used in accordance with the terms and
** conditions contained in a signed written agreement between you and Nokia.
**
**
**
** $QT_END_LICENSE$
**
****************************************************************************/


#include "customWidgets/togglechip.h"

/*!----------------------------------------------------+
 *                                                     |
 *      @author: Andrea Venturelli                     |
 *      @mail  : aventurelli.github@gmail.com          |
 *                                                     |
 *-----------------------------------------------------+
*/


// default constructor
ToggleChip::ToggleChip(QWidget *parent)
    : QCheckBox{ parent }
{
    this->setCheckable( true );
}


// parametrized constructor
ToggleChip::ToggleChip(QString left_option, QString right_option, QWidget* parent)
    : QCheckBox{ parent }, m_text_left{ left_option }, m_text_right{ right_option }
{
    this->setCheckable( true );
}


// deconstructor
ToggleChip::~ToggleChip() {}



ToggleChip::DoubleStrPair ToggleChip::leftAndRightText() const
{
    return std::make_pair( m_text_left, m_text_right );
}


void ToggleChip::setLeftRightText( QString left, QString right )
{
    m_text_left = left;
    m_text_right = right;
    emit leftAndRightTextChanged( m_text_left, m_text_right );
}


/*!
 *
 *  OVERRIDDEN MEMBERS
 *
 */
QSize ToggleChip::sizeHint() const
{
    QFontMetrics fnt_mt( m_font );
    int larger_text_w{};

    if ( m_text_left >= m_text_right )
    {
        larger_text_w = fnt_mt.horizontalAdvance( m_text_left );
    } else {
        larger_text_w = fnt_mt.horizontalAdvance( m_text_right );
    }

    larger_text_w += 16 * 2;
    int min_height = 7 * fnt_mt.boundingRect( m_text_left + m_text_right ).height();

    return QSize( 2 * larger_text_w, min_height );
}


void ToggleChip::paintEvent(QPaintEvent* event)
{
    /*
     *  the frame bg height is equivalent to
     *  Hw = width * 0.303030
     *
     *  the single slider is
     *  Slider_width = 0.45 * frame_bg.width()     (45% of frame bg)
     *  Slider_height = Hw * 0.75                  (77% of frame bg height)
    */
    Q_UNUSED(event)

    int frame_height = rect().height() * HEIGHT_FACTOR;
    QRect FRAME_RECT(
        rect().x(), rect().center().y() - ( frame_height / 2 ), // centered vertically
        rect().width(), frame_height
    );

    int relativePadding = (0.26 * frame_height) / 2;

    QRect leftSliderRect(
        FRAME_RECT.x() + relativePadding,
        FRAME_RECT.y() + relativePadding,
        FRAME_RECT.width() * 0.45,
        FRAME_RECT.height() * 0.75
    );

    QRect rightSliderRect(
        FRAME_RECT.width() - relativePadding - leftSliderRect.width(),
        FRAME_RECT.y() + relativePadding,
        FRAME_RECT.width() * 0.45,
        FRAME_RECT.height() * 0.75
    );

    int slider_border_radius = leftSliderRect.height() / 2;

    QPainter p(this);
    p.fillRect( rect(), Qt::transparent );
    p.setRenderHint( QPainter::Antialiasing, true );
    p.setRenderHint( QPainter::TextAntialiasing, true );
    p.setRenderHint(QPainter::SmoothPixmapTransform, true);

    int border_radius = FRAME_RECT.height() / 2;

    // draw the background frame
    p.setBrush( FRAME_BG );
    p.setPen( Qt::NoPen );
    p.drawRoundedRect( FRAME_RECT, border_radius, border_radius );

    //! DRAW THE SLIDER IN THE CORRECT POSITION
    p.setBrush( SLIDER_COL );
    if ( this->isChecked() == IndicatorPosition::Left )
    {
        // draw the slider on the left
        //----------------------------
        p.drawRoundedRect( leftSliderRect, slider_border_radius, slider_border_radius);
    } else {
        // draw the slider on the right
        //----------------------------
        p.drawRoundedRect( rightSliderRect, slider_border_radius, slider_border_radius);
    }

    QFontMetrics fnt_mtrx( m_font );
    QTextOption txt_option( Qt::AlignCenter );
    p.setFont( m_font );

    // DRAW LEFT TEXT CENTERED INSIDE LEFT_RECT_SECTOR
    //- - - - - - - - - - - - - - - - - - - - - - - - -
    QRect leftText_rect = fnt_mtrx.boundingRect( m_text_left );
    leftText_rect.setWidth( fnt_mtrx.horizontalAdvance( m_text_left ) );

    leftText_rect.moveCenter( leftSliderRect.center() );
    // draw text
    p.setPen( (this->isChecked()) ? TEXT_COL_DEFAULT : TEXT_COL_SELECTED );   // if checked() == true the slider is on the right
    p.drawText( leftText_rect, m_text_left, txt_option );


    // DRAW RIGHT TEXT CENTERED INSIDE RIGHT_RECT_SECTOR
    //- - - - - - - - - - - - - - - - - - - - - - - - -
    QRect rightText_rect = fnt_mtrx.boundingRect( m_text_right );
    rightText_rect.setWidth( fnt_mtrx.horizontalAdvance( m_text_right ) + 4 );
    rightText_rect.moveCenter( rightSliderRect.center() );
    // draw text
    p.setPen( (this->isChecked()) ? TEXT_COL_SELECTED : TEXT_COL_DEFAULT );   // opposite color than before
    p.drawText( rightText_rect, m_text_right, txt_option );

    p.end();
}


void ToggleChip::mouseReleaseEvent(QMouseEvent* event)
{
    // Get click position relative to the widget
    QPoint clickPos = event->pos();

    // Toggle state based on click position
    bool wasChecked = isChecked();
    bool nowChecked = (clickPos.x() > width() / 2);

    if (wasChecked != nowChecked) {
        setChecked(nowChecked);
        update(); // Force repaint
        emit selectionChanged( (nowChecked) ? IndicatorPosition::Right : IndicatorPosition::Left ); // Emit signal if needed
    }
}


void ToggleChip::enterEvent(QEnterEvent* event)
{
    Q_UNUSED(event)
    setCursor( Qt::PointingHandCursor );
}


void ToggleChip::leaveEvent(QEvent* event)
{
    Q_UNUSED(event)
    setCursor( Qt::ArrowCursor );
}










