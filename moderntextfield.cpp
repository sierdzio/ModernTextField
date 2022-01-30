#include "moderntextfield.h"

#include "painter.h"

#include <QPainter>

ModernTextField::ModernTextField(QQuickItem *parent) : QQuickPaintedItem(parent),
    _painter(new Painter(this))
{
}

void ModernTextField::paint(QPainter *painter)
{
    _painter->setSize(boundingRect().size().toSize());
    _painter->paint(painter);
}
