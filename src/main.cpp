#include "glwindow.h"

#include <QApplication>
#include <qloggingcategory.h>
int main(int argc, char *argv[])
{
    QLoggingCategory::setFilterRules("qt.opengl.debug=true");
    QApplication a(argc, argv);
    GLWindow w;
    w.show();
    return QApplication::exec();
}
