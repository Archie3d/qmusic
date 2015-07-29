#include <QDebug>

#include "Application.h"
#include "MainWindow.h"

int main(int argc, char *argv[])
{
    Application app(argc, argv);
    MainWindow mainWindow;

    app.setMainWindow(&mainWindow);

    // Run the application
    return app.launch();
}
