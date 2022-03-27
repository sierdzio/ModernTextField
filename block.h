#ifndef BLOCK_H
#define BLOCK_H

#include <QObject>
#include <QFlag>
#include <QSize>
#include <QUrl>

struct Block
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

    Types types = Type::None;
    bool showThumbnail = true;

    QUrl linkDestination;
    QString text;

    QPair<Block, Block> split(const int where) const;

    const QSize &size() const;
    QFont font() const;

    Block (const Types types_, const QString &text_) : types(types_), text(text_)
    {
        compute();
    }

private:
    void compute();

    QSize _size;
};

 Q_DECLARE_OPERATORS_FOR_FLAGS(Block::Types)

#endif // BLOCK_H
