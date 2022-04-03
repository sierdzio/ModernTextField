#ifndef BLOCK_H
#define BLOCK_H

#include <QObject>
#include <QFlag>
#include <QPoint>
#include <QSize>
#include <QFont>
#include <QColor>
#include <QUrl>

struct Chunk {
    QString text;
    QPoint position;
    QSize size;

    bool operator==(const Chunk &other) const;
};

class Block
{
    Q_GADGET

    friend class Painter;

public:
    enum Type {
        None = 0x0000,
        Text = 0x0001,
        Link = 0x0002,
        Bold = 0x0004,
        Italic = 0x0008,
        Code = 0x0010,
        Citation = 0x0020,
        Emoji = 0x0040,
        CustomEmoji = 0x0080,
        Strikethrough = 0x0100,
        Underline = 0x0200
    };
    Q_DECLARE_FLAGS(Types, Type)
    Q_FLAG(Types)

    Block (const Types types_, const QString &text_);
    Block (const Types types_, const QString &text_, const QUrl &url);

    Types types = Type::None;
    bool showThumbnail = true;

    QString text;
    QUrl linkDestination;

    void split(const int chunkIndex, const int where);

    const QList<Chunk> &chunks() const;
    const Chunk &chunk(const int index) const;
    void updateChunkPosition(const int index, const QPoint &position);
    void clearChunks();

    const QFont &font() const;
    const QColor &color() const;

    bool operator==(const Block &other) const;

private:
    void compute();
    void computeFont();
    void computeColor();
    void computeDefaultChunk();

    QSize computeChunkSize(const Chunk &chunk) const;

    QList<Chunk> _chunks;

    QFont _font;
    QColor _color;
};

Q_DECLARE_OPERATORS_FOR_FLAGS(Block::Types)

#endif // BLOCK_H
