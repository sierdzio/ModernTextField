#include "block.h"

#include <QFont>
#include <QFontMetrics>

Block::Block(const Types types_, const QString &text_)
    : types(types_), text(text_)
{
    compute();
}

Block::Block(const Types types_, const QString &text_, const QUrl &url)
    : types(types_), text(text_), linkDestination(url)
{
    compute();
}

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

const QSize &Block::size() const
{
    return _size;
}

const QFont &Block::font() const
{
    return _font;
}

const QColor &Block::color() const
{
    return _color;
}

void Block::compute()
{
    computeFont();
    computeColor();

    if (types.testFlag(Type::Text)) {
        const QFontMetrics metrics(font());

        _size = metrics.boundingRect(0, 0, 1000, 1000, Qt::AlignLeft | Qt::AlignTop, text).size();
    }
}

void Block::computeFont()
{
    // TODO: custom font...
    QFont font;

    if (types.testFlag(Type::Bold)) {
        font.setBold(true);
    }

    if (types.testFlag(Type::Italic)) {
        font.setItalic(true);
    }

    if (types.testFlag(Type::Link)) {
        font.setUnderline(true);
    }

    _font = font;
}

void Block::computeColor()
{
    if (types.testFlag(Type::Link)) {
        _color = QColor(50, 50, 255);
        return;
    }

    _color = QColor(0, 0, 0);
}
