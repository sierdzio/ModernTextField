#include "moderntextfield.h"

#include "painter.h"

#include <QPainter>

ModernTextField::ModernTextField(QQuickItem *parent) : QQuickPaintedItem(parent),
    _painter(new Painter(this))
{
    setAcceptTouchEvents(true);
    setAcceptedMouseButtons(Qt::MouseButton::LeftButton);
}

void ModernTextField::paint(QPainter *painter)
{
    _painter->setSize(boundingRect().size().toSize());
    _painter->paint(painter);
}

void ModernTextField::mousePressEvent(QMouseEvent *event)
{
    event->setAccepted(true);
}

void ModernTextField::mouseReleaseEvent(QMouseEvent *event)
{
    _painter->mouseReleaseEvent(event);
}
