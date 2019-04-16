#ifndef WINDOW_H
#define WINDOW_H

#include <QWidget>

class QPushButton;
class Window : public QWidget
{
    Q_OBJECT
public:
     Window(QWidget * parent = nullptr);
private slots:
    void slotButtonClicked(bool);
private:
    QPushButton *m_button;
};

#endif // WINDOW_H
