#ifndef PAINTER_H
#define PAINTER_H

#include <QObject>
#include <QSize>

class QPainter;

class Painter : public QObject
{
    Q_OBJECT
public:
    enum Mode {
        Preview,
        Edit,
        ReadOnly,
        Expanded
    };

    explicit Painter(QObject *parent = nullptr);    

    QSize size() const;
    void setSize(const QSize &newSize);

    void paint(QPainter *painter);

private:
    QSize _size = QSize(300, 200);
};

#endif // PAINTER_H
