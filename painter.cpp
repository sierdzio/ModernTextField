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
        { Block::Type::Text | Block::Type::Strikethrough, " This will be crossed out. " },
        { Block::Type::Text | Block::Type::Strikethrough | Block::Type::Bold
          | Block::Type::Italic | Block::Type::Underline,
          " Massive combination! " },
    };

    recalculate();
}

void Painter::paint(QPainter *painter) const
{
    // TODO: after painting, save result as image?

    painter->save();

    for (const auto &block : qAsConst(_blocks)) {
        for (const auto &chunk : block.chunks()) {
            drawText(block, chunk, _alignment, painter);
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
    if (_size != newSize) {
        _size = newSize;
        recalculate();
    }
}

Qt::Alignment Painter::alignment() const
{
    return _alignment;
}

void Painter::setAlignment(const Qt::Alignment &newAlignment)
{
    if (_alignment != newAlignment) {
        _alignment = newAlignment;
        recalculate();
    }
}

void Painter::mouseReleaseEvent(QMouseEvent *event) const
{
    for (const auto &block : qAsConst(_blocks)) {
        for (const auto &chunk : block.chunks()) {
            if (block.linkDestination.isEmpty() == false) {
                const QRect region(chunk.position, chunk.size);

                //qDebug() << "Chunk region" << event->pos() << region << chunk.text;

                if (region.contains(event->pos())) {
                    event->setAccepted(true);
                    qDebug() << "Clicked! Please go to:"
                             << block.linkDestination;
                    emit clicked(block.linkDestination);
                }
            }
        }
    }
}

void Painter::drawText(const Block &block,
                       const Chunk &chunk,
                       const Qt::Alignment alignment,
                       QPainter *painter) const
{
    if (painter->font() != block.font()) {
        painter->setFont(block.font());
    }

    if (const auto &color = block.color(); color != painter->pen().color()) {
        auto pen = painter->pen();
        pen.setColor(block.color());
        painter->setPen(pen);
    }

    painter->drawText(QRect(chunk.position, chunk.size), alignment, chunk.text);
}

void Painter::recalculate()
{
    const auto mainRect = QRect(0, 0, _size.width(), _size.height());

    int line = 0;
    int column = 0;

    for (qsizetype blockIndex = 0; blockIndex < _blocks.size(); ++blockIndex) {
        auto block = _blocks.at(blockIndex);
        block.clearChunks();

        for (qsizetype chunkIndex = 0; chunkIndex < block.chunks().size(); ++chunkIndex) {
            const auto &chunk = block.chunk(chunkIndex);

            if (column + 5 >= mainRect.width()) {
                // Line break!
                line += chunk.size.height();
                column = 0;
            }

            block.updateChunkPosition(chunkIndex, QPoint(column, line));

            // If current chunk does not fit, split it!
            if (column + chunk.size.width() > mainRect.width()) {
                block.split(chunkIndex, mainRect.width() - column);

                // Line break!
                // TODO: get the highest block from this line!
                line += chunk.size.height();
                column = 0;
                continue;
            }

            column += chunk.size.width();
        }

        _blocks.replace(blockIndex, block);
    }
}
