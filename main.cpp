#include "svgeditor.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    SVGEditor window;
    window.show();
    return app.exec();
}
