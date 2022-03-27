#include "painter.h"

#include <QPainter>

#include <QDebug>

Painter::Painter(QObject *parent) : QObject(parent)
{
    _blocks = {
        { Block::Type::Text, "Hello world! " },
        { Block::Type::Text | Block::Type::Bold, "Bold world, too! " },
        { Block::Type::Text | Block::Type::Italic, "Here, have some italics, mate." }
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

            painter->setFont(chunks.first.font());

            painter->drawText(column, line,
                              mainRect.width() - column, mainRect.height() - line,
                              _alignment, chunks.first.text);


            // Line break!
            line += block.size().height();
            column = 0;
            continue;
        }

        painter->setFont(block.font());

        painter->drawText(column, line,
                          mainRect.width() - column, mainRect.height() - line,
                          _alignment, block.text);

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
