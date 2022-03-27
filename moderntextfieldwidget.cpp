#include "moderntextfieldwidget.h"

#include "painter.h"

#include <QPaintEvent>
#include <QResizeEvent>
#include <QPainter>

ModernTextFieldWidget::ModernTextFieldWidget(QWidget *parent) : QWidget(parent),
    _painter(new Painter(this))
{
}

QSize ModernTextFieldWidget::sizeHint() const
{
    return _painter->size();
}

void ModernTextFieldWidget::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    _painter->paint(&painter);

    event->accept();
}

void ModernTextFieldWidget::resizeEvent(QResizeEvent *event)
{
    _painter->setSize(event->size());
    QWidget::resizeEvent(event);
}

void ModernTextFieldWidget::mouseReleaseEvent(QMouseEvent *event)
{
    _painter->mouseReleaseEvent(event);
}
