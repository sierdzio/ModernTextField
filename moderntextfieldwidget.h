#ifndef MODERNTEXTFIELDWIDGET_H
#define MODERNTEXTFIELDWIDGET_H

#include <QPointer>
#include <QWidget>

class Painter;

class ModernTextFieldWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ModernTextFieldWidget(QWidget *parent = nullptr);

public:
    QSize sizeHint() const override;

protected:
    void paintEvent(QPaintEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;

private:
    QPointer<Painter> _painter;
};

#endif // MODERNTEXTFIELDWIDGET_H
