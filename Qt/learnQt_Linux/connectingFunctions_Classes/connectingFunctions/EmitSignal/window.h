#ifndef WINDOW_H
#define WINDOW_H

#include <QWidget>
#include <QPushButton>

class Window : public QWidget
{
    Q_OBJECT
public:
    Window(QWidget *parent = nullptr);
signals:
    void counterOverFlow();//emit function no need to implement this
private slots:
    void slotButtonClicked(bool checked);
private:
    int m_counter;
    QPushButton * m_button;
};

#endif // WINDOW_H
