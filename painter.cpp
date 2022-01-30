#include "painter.h"

#include <QPainter>
#include <QFontMetrics>

#include <QDebug>

Painter::Painter(QObject *parent) : QObject(parent)
{
}

QSize Painter::size() const
{
    return _size;
}

void Painter::setSize(const QSize &newSize)
{
    _size = newSize;
}

void Painter::paint(QPainter *painter)
{
    painter->save();

    const QString text("Hello world!");
    const auto metrics = painter->fontMetrics().boundingRect(text);

    painter->setClipRect(QRect(0, 0, _size.width(), _size.height()));
    painter->drawText(QPoint((_size.width() - metrics.width()) * .5,
                             (_size.height() - metrics.height()) * .5),
                      text);

    painter->restore();
}
