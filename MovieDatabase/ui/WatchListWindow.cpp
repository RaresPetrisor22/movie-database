#include "WatchListWindow.h"
#include <QVBoxLayout>
#include <QHeaderView>
#include <QAbstractItemView>

WatchListWindow::WatchListWindow(Controller& ctrl, QWidget* parent)
    : QDialog(parent), controller(ctrl)
{
    setWindowTitle("Watchlist");
    resize(700, 400);

    auto* layout = new QVBoxLayout(this);

    tableView = new QTableView(this);
    model = new MovieTableModel(controller.getAllMoviesFromWatchList(), this);

    tableView->setModel(model);
    tableView->horizontalHeader()->setStretchLastSection(true);
    tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    tableView->resizeColumnsToContents();

    layout->addWidget(tableView);
    setLayout(layout);
}

WatchListWindow::~WatchListWindow() {}
