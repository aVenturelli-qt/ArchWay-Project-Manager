#ifndef STATECHIP_H
#define STATECHIP_H

#include <QObject>
#include <QPaintEvent>
#include <QPainter>
#include <QBrush>
#include <QPen>
#include <QWidget>

class StateChip : public QWidget
{
    Q_OBJECT

    bool m_isValid{ false };

    short PADDING_LEFT_RIGHT = 8;
    short PADDING_TOP_BOTTOM = 4;

    QBrush VALID_COL{ QColor(41, 136, 52) };
    QBrush INVALID_COL{ QColor(165, 62, 62) };

public:
    explicit StateChip(QWidget *parent = nullptr);

    QSize sizeHint() const override;
    void setValidityState( bool isValid );

protected:
    void paintEvent(QPaintEvent *event) override;

signals:
    void stateChanged(bool state);
};

#endif // STATECHIP_H
