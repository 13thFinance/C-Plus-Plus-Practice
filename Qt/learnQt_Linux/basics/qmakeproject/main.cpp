#include <QApplication>
#include <QPushButton>

int main(int argc, char**argv)
{
    QApplication app(argc, argv);

    //create a window
    QWidget window;
    window.setFixedSize(400, 200);



    //create pushbutton
    QPushButton button("crack me, daddy", &window);
    button.setText("Crack me, father!");
    button.setToolTip("Crack me!");

    //set font
    QFont font("Courier");
    button.setFont(font);

    //set icon
    QIcon icon ("/root/Desktop/hateWaifu.jpeg");
    QIcon smiley = QIcon::fromTheme("face-smile");
    button.setIcon(icon);
    button.setIconSize(QSize(100, 100));
    //x, y, width, height
    button.setGeometry(0, 0, 400, 200);//sets the button at a certain location on the window

    //set function for button


    //show window
    window.show();
    return app.exec();
}
