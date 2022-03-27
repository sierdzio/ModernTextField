#ifndef PAINTER_H
#define PAINTER_H

#include <QObject>
#include <QSize>

#include "block.h"

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

    const QSize &size() const;
    void setSize(const QSize &newSize);

    Qt::Alignment alignment() const;
    void setAlignment(const Qt::Alignment &newAlignment);

    bool addText(const Block &block);
    bool removeCharacter(const int index = -1);
    bool removeString(const int from, const int to = -1);

private:
    void drawText(const int column, const int line, const QRect &rectangle,
                  const Qt::Alignment alignment, const Block &block,
                  QPainter *painter) const;

    QSize _size = QSize(300, 200);
    Qt::Alignment _alignment = Qt::AlignmentFlag::AlignLeft | Qt::AlignmentFlag::AlignTop;

    QList<Block> _blocks;
};

#endif // PAINTER_H
