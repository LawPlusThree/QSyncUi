#include <QApplication>
#include "loginwindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    //测试用户
    User testuser("newuser", "newuser@example.com", "123456");
    testuser.enroll();

    LoginWindow w;
    w.show();
    return a.exec();
}
