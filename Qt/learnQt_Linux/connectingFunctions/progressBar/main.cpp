#include <QApplication>
#include <QProgressBar>
#include <QSlider>

int main(int argc, char** argv)
{
    QApplication app(argc, argv);

    //create container window
    QWidget window;
    window.setFixedSize(200, 200);
    window.setMaximumSize(1920, 1080);

    //create a progress bar
    //with the range between 0 and 100, and a starting value of 0
    QProgressBar *progressBar = new QProgressBar(&window);
    progressBar->setRange(0, 100);//range = 0 - 101
    progressBar->setValue(0);//start at 0
    progressBar->setGeometry(10, 10, 180, 30);

    //create horizontal slider
    //with range 0-100, and start with val = 0
    QSlider *slider = new QSlider(&window);
    slider->setOrientation(Qt::Horizontal);
    slider->setRange(0, 100);
    slider->setValue(0);
    slider->setGeometry(10, 40, 180, 30);

    window.show();

    //connect slider value with progress bar value
    QObject::connect(slider, SIGNAL(valueChanged(int)), progressBar, SLOT(setValue(int)));

    return app.exec();
}
