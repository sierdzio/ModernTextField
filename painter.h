#ifndef PAINTER_H
#define PAINTER_H

#include <QObject>
#include <QSize>
#include <QRect>

#include "block.h"

class QPainter;
class QMouseEvent;

struct BlockRegion
{
    Block block;
    // TODO: QList of rectangles representing all chunks
    QRect paintedRectangle;
};

class Painter : public QObject
{
    Q_OBJECT

    friend class ModernTextField;
    friend class ModernTextFieldWidget;

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

signals:
    void clicked(const QUrl &url) const;

protected:
    void mouseReleaseEvent(QMouseEvent *event);

private:
    QRect drawText(const int column, const int line, const QRect &rectangle,
                   const Qt::Alignment alignment,
                   const Block &block, const int chunkIndex,
                   QPainter *painter) const;

    void addClickableBlock(const QRect &rectangle, const Block &block);
    void removeClickableBlock(const Block &block);

    QSize _size = QSize(300, 200);
    Qt::Alignment _alignment = Qt::AlignmentFlag::AlignLeft | Qt::AlignmentFlag::AlignTop;

    QList<Block> _blocks;
    QList<BlockRegion> _clickable;
};

#endif // PAINTER_H
