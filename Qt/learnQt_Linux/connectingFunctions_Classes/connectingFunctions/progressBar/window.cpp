#include "window.h"

#include <QPushButton>
Window::Window(QWidget* parent): QWidget (parent)
{
    //set fixed size of window
    setFixedSize(600, 480);
    setMaximumSize(1920, 1080);

    //create and position a button
    m_button = new QPushButton("Crack me!", this);
    m_button->setGeometry(10, 10, 600, 480);
    m_button->setIcon(QIcon("hateWaifu.jpeg"));
    m_button->setIconSize(QSize(200, 300));

    //make sure the button can be checked/clicked
    m_button->setCheckable(true);
    //connects the buttons clicked button with the function slotButtonClicked
    connect(m_button, SIGNAL(clicked(bool)), this, SLOT(slotButtonClicked(bool)));
}

void Window::slotButtonClicked(bool checked)
{
    if(checked)
    {
        m_button->setText("Harder!");
    }
    else
    {
        m_button->setText("Cracked!");
    }
}
