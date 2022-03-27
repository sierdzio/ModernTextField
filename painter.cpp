#include "painter.h"

#include <QPainter>

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
    painter->save();

    const auto mainRect = QRect(0, 0, _size.width(), _size.height());

    int line = 0;
    int column = 0;
    for (qsizetype i = 0; i < _blocks.size(); ++i) {
        const auto &block = _blocks.at(i);

        if (column + 5 >= mainRect.width()) {
            // Line break!
            line += block.size().height();
            column = 0;
        }

        if (column + block.size().width() > mainRect.width()) {
            const auto chunks = block.split(mainRect.width() - column);

            _blocks.replace(i, chunks.first);
            _blocks.insert(i + 1, chunks.second);

            drawText(column, line, mainRect, _alignment, chunks.first, painter);

            // Line break!
            line += block.size().height();
            column = 0;
            continue;
        }

        drawText(column, line, mainRect, _alignment, block, painter);

        column += block.size().width();
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

void Painter::drawText(const int column, const int line,
                       const QRect &rectangle,
                       const Qt::Alignment alignment,
                       const Block &block,
                       QPainter *painter) const
{
    painter->setFont(block.font());

    if (const auto &color = block.color(); color != painter->pen().color()) {
        auto pen = painter->pen();
        pen.setColor(block.color());
        painter->setPen(pen);
    }

    painter->drawText(column, line,
                      rectangle.width() - column, rectangle.height() - line,
                      alignment, block.text);
}
