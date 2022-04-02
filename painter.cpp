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
        const auto &block = _blocks.at(blockIndex);

        for (qsizetype chunkIndex = 0; chunkIndex < block.chunks().size(); ++chunkIndex) {
            if (column + 5 >= mainRect.width()) {
                // Line break!
                line += block.size().height();
                column = 0;
            }

            // TODO: fix to include blocks last chunk width, not the width
            // of the entire block
            if (column + block.size().width() > mainRect.width()) {
                auto newBlock = block;
                newBlock.split(chunkIndex, mainRect.width() - column);
                _blocks.replace(blockIndex, newBlock);

                // TODO: fix
//                removeClickableBlock(block);

//                _blocks.replace(i, chunks.first);
//                _blocks.insert(i + 1, chunks.second);

                /*const auto region =*/ drawText(column, line, mainRect, _alignment,
                                             newBlock, chunkIndex, painter);

                // TODO: fix
//                addClickableBlock(region, chunks.first);

                // Line break!
                line += block.size().height();
                column = 0;
                continue;
            }

            const auto region = drawText(column, line, mainRect, _alignment, block,
                                         0, painter);

            addClickableBlock(region, block);

            column += block.size().width();
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
    for (const auto &blockRegion : qAsConst(_clickable)) {
        if (blockRegion.paintedRectangle.contains(event->pos())) {
            event->setAccepted(true);
            qDebug() << "Clicked! Please go to:"
                     << blockRegion.block.linkDestination;
            emit clicked(blockRegion.block.linkDestination);
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

void Painter::addClickableBlock(const QRect &rectangle, const Block &block)
{
    if (block.types.testFlag(Block::Type::Link)) {
        _clickable.append({ block, rectangle });
    }
}

void Painter::removeClickableBlock(const Block &block)
{
    for (qsizetype i = 0; i < _clickable.size(); ++i) {
        if (_clickable.at(i).block == block) {
            _clickable.removeAt(i);
            return;
        }
    }
}
