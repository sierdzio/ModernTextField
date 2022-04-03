#include "block.h"

#include <QFont>
#include <QFontMetrics>

bool Chunk::operator==(const Chunk &other) const
{
    return text == other.text && position == other.position && size == other.size;
}

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
    const int index = where * current.text.length() / current.size.width();

    Chunk left, right;
    left.text = current.text.left(index);
    left.position = current.position;
    left.size = computeChunkSize(left);
    _chunks.replace(chunkIndex, left);

    right.text = current.text.mid(index);
    right.size = computeChunkSize(right);

    const int rightIndex = chunkIndex + 1;
    _chunks.insert(rightIndex, right);
}

const QList<Chunk> &Block::chunks() const
{
    return _chunks;
}

const Chunk &Block::chunk(const int index) const
{
    return _chunks.at(index);
}

void Block::updateChunkPosition(const int index, const QPoint &position)
{
    if (index < 0 || index >= _chunks.size()) {
        return;
    }

    auto chunk = _chunks.at(index);
    chunk.position = position;
    _chunks.replace(index, chunk);
}

void Block::clearChunks()
{
    if (_chunks.size() != 1) {
        _chunks.clear();
        computeDefaultChunk();
    }
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
    return types == other.types
            && text == other.text
            && showThumbnail == other.showThumbnail
            && linkDestination == other.linkDestination
            && _chunks == other._chunks
            && _font == other._font
            && _color == other._color;
}

void Block::compute()
{
    computeFont();
    computeColor();
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

    if (types.testFlag(Type::Strikethrough)) {
        font.setStrikeOut(true);
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

void Block::computeDefaultChunk()
{
    if (types.testFlag(Type::Text)) {
        const QFontMetrics metrics(font());

        const auto size = metrics.boundingRect(0, 0, 1000, 1000, Qt::AlignLeft | Qt::AlignTop, text).size();
        _chunks.append({ text, QPoint(), size });
    }
}

QSize Block::computeChunkSize(const Chunk &chunk) const
{
    const QFontMetrics metrics(font());
    return metrics.boundingRect(0, 0, 1000, 1000,
                                Qt::AlignLeft | Qt::AlignTop,
                                chunk.text).size();
}
