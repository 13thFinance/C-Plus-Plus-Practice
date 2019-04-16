#include "window.h"

#include <string>
#include <string.h>
#include <QApplication>

Window::Window(QWidget *parent) : QWidget(parent)
{
    //set window size
    setFixedSize(500, 500);
    setMaximumSize(1000, 1000);

    //create and postion the button
    m_button = new QPushButton("I am a button!", this);
    m_button->setGeometry(10, 10, 100, 30);
    m_button->setCheckable(true);

    //set up counter
    m_counter = 0;

    connect(m_button, SIGNAL(clicked(bool)), this, SLOT(slotButtonClicked(bool)));
    connect(this, SIGNAL(counterOverFlow()), QApplication::instance(), SLOT(quit()));

}

void Window::slotButtonClicked(bool checked)
{
    m_button->setText(QString(std::to_string(m_counter).c_str()));
    m_counter++;

    //emit signal when counter reaches a certain number
    if(m_counter >= 10 && checked)//doesnt ncessaraly have to be checked
    {
        emit counterOverFlow();//make sure to connect this signal
    }
}


