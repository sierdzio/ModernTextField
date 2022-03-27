#ifndef BLOCK_H
#define BLOCK_H

#include <QObject>
#include <QFlag>
#include <QSize>
#include <QFont>
#include <QColor>
#include <QUrl>

class Block
{
    Q_GADGET

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

    QPair<Block, Block> split(const int where) const;

    // TODO: add merge() method to merge chunks when widget grows. It should
    // merge together blocks which have the same properties and which are
    // adjacent

    const QSize &size() const;
    const QFont &font() const;
    const QColor &color() const;

    bool operator==(const Block &other) const;

private:
    void compute();
    void computeFont();
    void computeColor();

    QSize _size;
    QFont _font;
    QColor _color;
};

 Q_DECLARE_OPERATORS_FOR_FLAGS(Block::Types)

#endif // BLOCK_H
