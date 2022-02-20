#include "painter.h"

#include <QPainter>
#include <QFontMetrics>

#include <QDebug>

QPair<Block, Block> Block::split(const int where) const
{
    Block first(*this), second(*this);

    const int index = where * text.length() / size().width();

    first.text = text.left(index);
    first.compute();
    second.text = text.mid(index);
    second.compute();

    return { first, second };
}

QSize Block::size() const
{
    return _size;
}

QFont Block::font() const
{
    // TODO: custom font...
    QFont font;

    if (types.testFlag(Type::Bold)) {
        font.setBold(true);
    }

    if (types.testFlag(Type::Italic)) {
        font.setItalic(true);
    }

    return font;
}

void Block::compute()
{
    if (types.testFlag(Type::Text)) {
        const QFontMetrics metrics(font());

        _size = metrics.boundingRect(0, 0, 1000, 1000, Qt::AlignLeft | Qt::AlignTop, text).size();
    }
}

Painter::Painter(QObject *parent) : QObject(parent)
{
    _blocks = {
        { Block::Type::Text, "Hello world!" },
        { Block::Type::Text | Block::Type::Bold, "Bold world, too!" },
        { Block::Type::Text | Block::Type::Italic, "Here, have some italics, mate." }
    };
}

void Painter::paint(QPainter *painter)
{
    painter->save();

//    const QString text("Hello world!");
//    const auto metrics = painter->fontMetrics().boundingRect(text);

    const auto mainRect = QRect(0, 0, _size.width(), _size.height());
    painter->setClipRect(mainRect);
//    painter->drawText(QPoint((_size.width() - metrics.width()) * .5,
//                             (_size.height() - metrics.height()) * .5),
//                      text);

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

QSize Painter::size() const
{
    return _size;
}

void Painter::setSize(const QSize &newSize)
{
    _size = newSize;
}

const Qt::Alignment &Painter::alignment() const
{
    return _alignment;
}

void Painter::setAlignment(const Qt::Alignment &newAlignment)
{
    _alignment = newAlignment;
}
