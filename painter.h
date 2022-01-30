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

    void paint(QPainter *painter);

    QSize size() const;
    void setSize(const QSize &newSize);

    const Qt::Alignment &alignment() const;
    void setAlignment(const Qt::Alignment &newAlignment);

private:
    QSize _size = QSize(300, 200);
    Qt::Alignment _alignment = Qt::AlignmentFlag::AlignLeft | Qt::AlignmentFlag::AlignTop;
};

#endif // PAINTER_H
