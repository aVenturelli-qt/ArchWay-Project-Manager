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

#include "delegates/groupslistdelegate.h"
#include "models/groupslistmodel.h"

#include <QDebug>


/*!----------------------------------------------------+
 *                                                     |
 *      @author: Andrea Venturelli                     |
 *      @mail  : aventurelli.github@gmail.com          |
 *                                                     |
 *-----------------------------------------------------+
*/


GroupsListDelegate::GroupsListDelegate(QWidget *parent)
    : QStyledItemDelegate{parent}
{
    m_view = qobject_cast<QListView*>(parent);
    if (!m_view) {
        qWarning() << "Parent must be a QListView";
        return;
    }
    m_view->setMouseTracking(true);

    // CHANGE: Connect to the model's button state signals
    auto* model = qobject_cast<GroupsListModel*>(m_view->model());
    if (model)
    {
        connect(
            model, &GroupsListModel::topButtonStateChanged,
            this, [this](const QModelIndex& index) {
            m_view->update(index);
            }
            );

        connect(
            model, &GroupsListModel::bottomButtonStateChanged,
            this, [this](const QModelIndex& index) {
            m_view->update(index);
        }
        );
    }
}



bool GroupsListDelegate::editorEvent(
    QEvent* event, QAbstractItemModel* model,
    const QStyleOptionViewItem& option, const QModelIndex& index
    )
{
    Q_UNUSED(option)

    if (!_isMouseEvent(event)) {
        return false; // Ignore non-mouse events
    }

    auto* mouseEvent = static_cast<QMouseEvent*>(event);
    QPoint cursorPosition = mouseEvent->pos();

    // Define button rectangles
    QRect editBtn{
        option.rect.topRight().x() - PADDING - BUTTONS_WIDTH,
        option.rect.center().y() - BUTTONS_SPACING - BUTTON1_HEIGHT,
        BUTTONS_WIDTH,
        BUTTON1_HEIGHT
    };

    QRect openAllBtn{
        option.rect.topRight().x() - PADDING - BUTTONS_WIDTH,
        option.rect.center().y(),
        BUTTONS_WIDTH,
        BUTTON2_HEIGHT
    };

    // Get the model to update button states
    auto* groupsModel = qobject_cast<GroupsListModel*>(model);
    if (!groupsModel) {
        return false; // Model is not valid
    }



    // Handle different mouse events
    switch (event->type())
    {
        case QEvent::MouseMove:
        case QEvent::MouseButtonRelease:
        {
            // Update top button state
            if ( _updateStateButton(cursorPosition, editBtn, groupsModel->topButtonState(index), btnState::Hover, index ))
            {
                if (event->type() == QEvent::MouseButtonRelease)
                {
                    emit editButtonClicked(index);
                    emit editButtonClickedGroup(index.data(Qt::DisplayRole).toString());
                }
                return true;
            }

            // Update bottom button state
            if ( _updateStateButton(cursorPosition, openAllBtn, groupsModel->bottomButtonState(index), btnState::Hover, index, false ))
            {
                if (event->type() == QEvent::MouseButtonRelease)
                {
                    emit openAllButtonClicked(index);
                    emit openAllButtonClickedGroup(index.data(Qt::DisplayRole).toString());
                }
                return true;
            }
            break;
        }

        case QEvent::MouseButtonPress: {
            if (mouseEvent->buttons() == Qt::LeftButton)
            {
                // Update top button state
                if (_updateStateButton(cursorPosition, editBtn, groupsModel->topButtonState(index), btnState::Pressed, index))
                {
                    return true;
                }

                // Update bottom button state
                if (_updateStateButton(cursorPosition, openAllBtn, groupsModel->bottomButtonState(index), btnState::Pressed, index, false))
                {
                    return true;
                }
            }
            break;
        }

        default:
            break;
    }

    return false;
}


void GroupsListDelegate::paint(
        QPainter* p, const QStyleOptionViewItem& option,
        const QModelIndex& index
    ) const
{
    p->save();
    p->setRenderHint( QPainter::Antialiasing, true );
    p->setRenderHint( QPainter::TextAntialiasing, true );

    const bool isCardSelected = option.state & QStyle::State_Selected ;

    // draw the bg
    QString groupName  = index.data( Qt::DisplayRole ).toString();
    QString elem_count = index.data( Qt::UserRole + 1 ).toString();

    // qDebug() << "\nGroupName: " << groupName << ", element counts: " << elem_count;

    QPainterPath clipPath;
    clipPath.addRoundedRect( option.rect, BG_BORDER_RAD, BG_BORDER_RAD );
    p->setClipPath( clipPath );

    p->setPen( Qt::NoPen );
    p->fillRect( option.rect, BG_COLOR );

    // change border color of an item when selected
    if ( isCardSelected )
    {
        auto border_brush = QBrush(GroupsListDelegate::_createConicalGradient( option.rect.center()));
        // bg selected state
        p->setPen( QPen( border_brush, 6.0 ));
        p->drawRoundedRect( option.rect, BG_BORDER_RAD, BG_BORDER_RAD );
    } else {
        // normal bg color
        p->setPen( Qt::NoPen );
        p->drawRoundedRect( option.rect, BG_BORDER_RAD, BG_BORDER_RAD );
    }

    //              DRAWING THE TWO TEXT
    //---------------------------------------------------
    auto txt_color = (isCardSelected)? QColor(231, 143,  26) : Qt::black;
    p->setPen( txt_color );

    auto upper_txt_rect = this->_rectFromFontMetrics( GROUP_TEXT_FONT, groupName, p );

    upper_txt_rect.moveBottomLeft(
        QPoint(
            option.rect.x() + PADDING,
            option.rect.center().y()
            )
        );

    // draw the groupText offsetted by 16px on the x
    p->drawText( upper_txt_rect, groupName );


    // BOTTOM TEXT ------------------
    QRect lower_txt_rect = this->_rectFromFontMetrics( SUB_TEXT_FONT, elem_count, p );

    lower_txt_rect.moveTopLeft(
        QPoint(
            upper_txt_rect.bottomLeft() + QPoint( 0, TEXT_SPACING)    // padding 12px on the y
            )
        );

    p->setPen( QPen( Qt::black ) );
    p->drawText( lower_txt_rect, elem_count );

    //              DRAWING TWO PUSHBUTTON
    //---------------------------------------------------

    // Draw buttons
    QRect editBtn{
        option.rect.topRight().x() - PADDING - BUTTONS_WIDTH,
        option.rect.center().y() - BUTTONS_SPACING - BUTTON1_HEIGHT,
        BUTTONS_WIDTH,
        BUTTON1_HEIGHT
    };

    QRect openAllBtn{
        option.rect.topRight().x() - PADDING - BUTTONS_WIDTH,
        option.rect.center().y(),
        BUTTONS_WIDTH,
        BUTTON2_HEIGHT
    };

    // CHANGE: Retrieve button states from the model
    auto* model = qobject_cast<GroupsListModel*>( const_cast<QAbstractItemModel*>(index.model() ));

    if ( model )
    {
        btnState topState    = model->topButtonState( index );
        btnState bottomState = model->bottomButtonState( index );

        _drawButton( editBtn, "Edit Group", p, topState, true );
        _drawButton( openAllBtn, "Open All", p, bottomState, false );
    }

    // reset for next item
    p->restore();
}


QSize GroupsListDelegate::sizeHint( const QStyleOptionViewItem& option, const QModelIndex& index ) const
{
    Q_UNUSED(option)
    Q_UNUSED(index)

    return QSize(
        280,
        2*PADDING + BUTTON1_HEIGHT + BUTTONS_SPACING + BUTTON2_HEIGHT + 12
        );
}


/*-------------------------------------------------------------
 *
 *                       HELPER FUNCTIONS
 *
 *-------------------------------------------------------------*/
QRect GroupsListDelegate::_rectFromFontMetrics(
        const QFont& font, const QString& text,
        QPainter* painter
    ) const
{
    if ( painter )
        painter->setFont( font );

    QFontMetrics fnt_m( font );
    QRect temp = fnt_m.boundingRect( text );
    temp.setWidth( temp.width() + 5);
    return temp;
}


void GroupsListDelegate::_drawButton(
        QRect button_rect, QString text,
        QPainter* p, btnState state,
        bool isTopBtn
    ) const
{
    auto previous_font  = p->font();
    auto previous_brush = p->brush();
    auto previous_pen   = p->pen();
    p->setFont( BTN_FONT );

    std::pair<QColor, QPen> fill_pen_pair;

    switch ( state )
    {
        case btnState::Default:
            fill_pen_pair = ( isTopBtn ) ? m_top_btn_style.fillAndPenStroke() : m_bottom_btn_style.fillAndPenStroke();
            break;

        case btnState::Hover:
            fill_pen_pair =
                ( isTopBtn ) ?
                m_top_btn_style.fillAndPenStroke   ( btnState::Hover ) :
                m_bottom_btn_style.fillAndPenStroke( btnState::Hover );
            break;

        case btnState::Pressed:
            fill_pen_pair =
                ( isTopBtn ) ?
                m_top_btn_style.fillAndPenStroke   ( btnState::Pressed ) :
                m_bottom_btn_style.fillAndPenStroke( btnState::Pressed );
            break;

        case btnState::Disabled:
            fill_pen_pair =
                ( isTopBtn ) ?
                m_top_btn_style.fillAndPenStroke   ( btnState::Disabled ) :
                m_bottom_btn_style.fillAndPenStroke( btnState::Disabled );
            break;

        default:
            qDebug() << "\nInvalid state..." ;
    }

    auto [ fill, pen ] = fill_pen_pair;
    p->setBrush( fill );
    p->setPen( pen );

    // frame btn
    p->drawRoundedRect( button_rect, 4, 4 );

    // text
    p->setPen( Qt::black );
    p->drawText( button_rect, Qt::AlignCenter, text );


    // restore original font, brush and pen
    p->setFont( previous_font );
    p->setBrush( previous_brush );
    p->setPen( previous_pen );
    return;

}


QRect GroupsListDelegate::_offsetXYRect(int x_offset, int y_offset, const QRect& originalRect ) const
{
    auto newRect = originalRect.translated( x_offset, y_offset );
    return newRect;
}


bool GroupsListDelegate::_isMouseEvent( QEvent* event ) const
{
    switch ( event->type() ) {
    case QEvent::MouseMove:
    case QEvent::MouseButtonPress:
    case QEvent::MouseButtonRelease:
    case QEvent::MouseButtonDblClick:
    case QEvent::Wheel:
        return true;
    default:
        return false;
    }
}


/**
 *  If the click is inside the button
 *  but the button is Disabled, simply return true
 *  to consume the mouseEvent
 *
 * Otherwise checks the button state
*/
bool GroupsListDelegate::_updateStateButton(QPoint mousePos, QRect btnRect, btnState currState, btnState newState, const QModelIndex& index, bool isTopButton )
{
    // Ignore event if the button is disabled
    if (currState == btnState::Disabled)
    {
        qDebug() << "Button is disabled; no state change.";
        return false;
    }

    auto* model = qobject_cast<GroupsListModel*>(m_view->model());
    if (!model) {
        return false;
    }

    bool stateChanged = false;

    // Check if the mouse is inside the button rectangle
    if (btnRect.contains(mousePos))
    {
        // Update the button state if it's different from the new state
        if (currState != newState)
        {
            if (isTopButton)
                model->setTopButtonState(index, newState);
            else
                model->setBottomButtonState(index, newState);

            stateChanged = true;
        }
    }
    else
    {
        // Reset the button state to Default if the mouse is outside the button
        if (currState != btnState::Default)
        {
            if (isTopButton)
                model->setTopButtonState(index, btnState::Default);
            else
                model->setBottomButtonState(index, btnState::Default);

            stateChanged = true;
        }
    }

    return stateChanged;
}
