#ifndef PAINTER_H
#define PAINTER_H

#include <QObject>
#include <QSize>
#include <QUrl>
#include <QFlag>

struct Block {
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

    QSize size() const;
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

class QPainter;

class Painter : public QObject
{
    Q_OBJECT
public:
    enum Mode {
        Preview,
        Edit,
        ReadOnly,
        Expanded
    };

    explicit Painter(QObject *parent = nullptr);

    void paint(QPainter *painter);

    QSize size() const;
    void setSize(const QSize &newSize);

    const Qt::Alignment &alignment() const;
    void setAlignment(const Qt::Alignment &newAlignment);

    bool addText(const Block &block);
    bool removeCharacter(const int index = -1);
    bool removeString(const int from, const int to = -1);

private:
    QSize _size = QSize(300, 200);
    Qt::Alignment _alignment = Qt::AlignmentFlag::AlignLeft | Qt::AlignmentFlag::AlignTop;

    QList<Block> _blocks;
};

#endif // PAINTER_H
