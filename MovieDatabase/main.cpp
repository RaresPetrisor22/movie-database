#include "AdministratorWindow.h"
#include "UserWindow.h"
#include "repository.h"
#include "controller.h"
#include "FileWatchList.h"
#include "tests.h"
#include <QtWidgets/QApplication>
#include<MainMenu.h>
#include <string>

int main(int argc, char *argv[])
{
    if (argc > 1 && std::string(argv[1]) == "--run-tests") {
        return runAllTests() ? 0 : 1;
    }

    QApplication app(argc, argv);

    Repository repo("data/movies.txt");
    CSVWatchList watchList("data/watchlist.csv");
    Controller controller(repo, &watchList);

    
    MainMenu menu;
    //if (menu.exec() != QDialog::Accepted) return 0;
    menu.exec();

    if (menu.isAdminMode()) {
       AdministratorWindow window(controller);
       window.show();
       return app.exec();

    }
    else {
        UserWindow window(controller);
        window.show();
        return app.exec();

    }

}
