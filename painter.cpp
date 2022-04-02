#include "painter.h"

#include <QPainter>
#include <QMouseEvent>

#include <QDebug>

Painter::Painter(QObject *parent) : QObject(parent)
{
    _blocks = {
        { Block::Type::Text, "Hello world! " },
        { Block::Type::Text | Block::Type::Bold, "Bold world, too! " },
        { Block::Type::Text | Block::Type::Italic, "Here, have some italics, mate. " },
        { Block::Type::Text | Block::Type::Link, "And this is a link.", QUrl("https://www.google.com") },
    };
}

void Painter::paint(QPainter *painter)
{
    // TODO: after painting, save result as image?

    painter->save();

    const auto mainRect = QRect(0, 0, _size.width(), _size.height());

    int line = 0;
    int column = 0;
    for (qsizetype blockIndex = 0; blockIndex < _blocks.size(); ++blockIndex) {
        auto block = _blocks.at(blockIndex);

        for (qsizetype chunkIndex = 0; chunkIndex < block.chunks().size(); ++chunkIndex) {
            if (column + 5 >= mainRect.width()) {
                // Line break!
                line += block.size().height();
                column = 0;
            }

            // If current chunk does not fit, split it!
            if (column + block.chunk(chunkIndex).width > mainRect.width()) {
                block.split(chunkIndex, mainRect.width() - column);
                _blocks.replace(blockIndex, block);

                drawText(column, line, mainRect, _alignment,
                         block, chunkIndex, painter);

                // Line break!
                // TODO: get the highest block from this line!
                line += block.size().height();
                column = 0;
                continue;
            }

            // If current and next chunk will fit, merge them!
            int checkIndex = chunkIndex;
            int amountToMerge = 0;
            while (checkIndex < block.chunks().size() && column + block.chunk(checkIndex).width <= mainRect.width()) {
                ++checkIndex;
                ++amountToMerge;
            }

            if (amountToMerge > 0) {
                block.merge(chunkIndex, amountToMerge);
                _blocks.replace(blockIndex, block);
            }

            drawText(column, line, mainRect, _alignment, block, chunkIndex,
                     painter);

            column += block.chunk(chunkIndex).width;
        }
    }

    painter->restore();
}

const QSize &Painter::size() const
{
    return _size;
}

void Painter::setSize(const QSize &newSize)
{
    _size = newSize;
}

Qt::Alignment Painter::alignment() const
{
    return _alignment;
}

void Painter::setAlignment(const Qt::Alignment &newAlignment)
{
    _alignment = newAlignment;
}

void Painter::mouseReleaseEvent(QMouseEvent *event)
{
    int x = 0;
    int y = 0;
    const auto width = _size.width();

    for (const auto &block : qAsConst(_blocks)) {
        for (const auto &chunk : block.chunks()) {
            if (block.linkDestination.isEmpty() == false) {
                const QRect region(x, y, chunk.width, block.size().height());

                if (region.contains(event->pos())) {
                    event->setAccepted(true);
                    qDebug() << "Clicked! Please go to:"
                             << block.linkDestination
                             << event->pos() << region;
                    emit clicked(block.linkDestination);
                }
            }

            if ((x + chunk.width) > width) {
                x = 0;
                y += block.size().height();
            } else {
                x += chunk.width;
            }
        }
    }
}

QRect Painter::drawText(const int column, const int line,
                        const QRect &rectangle,
                        const Qt::Alignment alignment,
                        const Block &block,
                        const int chunkIndex,
                        QPainter *painter) const
{
    painter->setFont(block.font());

    if (const auto &color = block.color(); color != painter->pen().color()) {
        auto pen = painter->pen();
        pen.setColor(block.color());
        painter->setPen(pen);
    }

    const QRect result = QRect(column, line, rectangle.width() - column,
                               rectangle.height() - line);

    painter->drawText(result, alignment, block.chunk(chunkIndex).text);

    return result;
}
