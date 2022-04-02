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

void Block::split(const int chunkIndex, const int where)
{
    const auto current = _chunks.at(chunkIndex);
    const int index = where * current.text.length() / current.width;

    Chunk left, right;
    left.text = current.text.left(index);
    left.width = computeChunkWidth(left);
    _chunks.replace(chunkIndex, left);

    right.text = current.text.mid(index);
    right.width = computeChunkWidth(right);

    const int rightIndex = chunkIndex + 1;
    _chunks.insert(rightIndex, right);

    merge(rightIndex, _chunks.size() - chunkIndex - 2);
}

void Block::merge(const int chunkIndex, const int chunkCount)
{
    if (chunkIndex >= _chunks.size() - 1) {
        return;
    }

    Chunk chunk;

    for (qsizetype i = chunkIndex; i < chunkIndex + chunkCount && i < _chunks.size(); ++i) {
        chunk.text.append(_chunks.takeAt(chunkIndex).text);
    }

    chunk.width = computeChunkWidth(chunk);
    _chunks.insert(chunkIndex, chunk);
}

const QSize &Block::size() const
{
    return _size;
}

const QList<Chunk> &Block::chunks() const
{
    return _chunks;
}

const Chunk &Block::chunk(const int index) const
{
    return _chunks.at(index);
}

const QFont &Block::font() const
{
    return _font;
}

const QColor &Block::color() const
{
    return _color;
}

bool Block::operator==(const Block &other) const
{
    return types == other.types && text == other.text
            && linkDestination == other.linkDestination
            && size() == other.size() && font() == other.font()
            && color() == other.color();
}

void Block::compute()
{
    computeFont();
    computeColor();

    if (types.testFlag(Type::Text)) {
        const QFontMetrics metrics(font());

        _size = metrics.boundingRect(0, 0, 1000, 1000, Qt::AlignLeft | Qt::AlignTop, text).size();
        _chunks.append({ text, _size.width() });
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

int Block::computeChunkWidth(const Chunk &chunk) const
{
    const QFontMetrics metrics(font());
    const auto size = metrics.boundingRect(0, 0, 1000, 1000,
                                           Qt::AlignLeft | Qt::AlignTop,
                                           chunk.text).size();
    return size.width();
}
