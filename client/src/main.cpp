#include <QApplication>
#include <iostream>



#include <QPushButton>

int main(int argc, char const *argv[])
{
    QApplication app(argc, (char **) argv);

    QPushButton button("haiiiiiiii wrld!!");
    button.show();

    return app.exec();
}