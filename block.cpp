#include "block.h"

#include <QFont>
#include <QFontMetrics>

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
