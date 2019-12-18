#include <iostream>
#include <csignal>

#include <QApplication>
#include "gui/MainWindow.h"

using namespace std;

void my_handler (int param)
{
    cerr << "Received signal: " << param << '\n';
}


int main(int argc, char* argv[]) {

    //ignore most of errors
    signal (SIGINT, my_handler);
    signal (SIGFPE, my_handler);
    signal (SIGSEGV, my_handler);
    signal (SIGABRT, my_handler);
    signal (SIGHUP, my_handler);
    signal (SIGILL, my_handler);
    signal (SIGPIPE, my_handler);
    signal (SIGQUIT, my_handler);

    QApplication app(argc, argv);
    MainWindow window;
    window.show();
    return app.exec();
}
