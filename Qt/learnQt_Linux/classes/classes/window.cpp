#include "window.h"
#include <QPushButton>

Window::Window(QWidget *parent) : QWidget(parent)
{
    //set the size of the window
    setFixedSize(100, 100);

    //create and position the button
    m_button = new QPushButton("I'm a dynamic button!", this);

}
