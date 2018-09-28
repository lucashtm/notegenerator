#include "notegenerator.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    NoteGenerator w;
    w.show();

    return a.exec();
}
