#include "painter.h"

#include <QPainter>
#include <QFontMetrics>

#include <QDebug>

Painter::Painter(QObject *parent) : QObject(parent)
{
}

void Painter::paint(QPainter *painter)
{
    painter->save();

    const QString text("Hello world!");
//    const auto metrics = painter->fontMetrics().boundingRect(text);

    const auto mainRect = QRect(0, 0, _size.width(), _size.height());
    painter->setClipRect(mainRect);
//    painter->drawText(QPoint((_size.width() - metrics.width()) * .5,
//                             (_size.height() - metrics.height()) * .5),
//                      text);
    painter->drawText(mainRect, _alignment, text);

    painter->restore();
}

QSize Painter::size() const
{
    return _size;
}

void Painter::setSize(const QSize &newSize)
{
    _size = newSize;
}

const Qt::Alignment &Painter::alignment() const
{
    return _alignment;
}

void Painter::setAlignment(const Qt::Alignment &newAlignment)
{
    _alignment = newAlignment;
}
