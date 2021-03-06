#ifndef MODERNTEXTFIELD_H
#define MODERNTEXTFIELD_H

#include <QPointer>
#include <QQuickPaintedItem>

class Painter;

class ModernTextField : public QQuickPaintedItem
{
    Q_OBJECT
    QML_ELEMENT

public:
    ModernTextField(QQuickItem *parent = nullptr);

    void paint(QPainter *painter) override;

protected:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;

private:
    QPointer<Painter> _painter;
};

#endif // MODERNTEXTFIELD_H
