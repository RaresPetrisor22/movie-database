#pragma once

#include <QDialog>
#include <QTableView>
#include "controller.h"
#include "MovieTableModel.h"

class WatchListWindow : public QDialog {
    Q_OBJECT

public:
    WatchListWindow(Controller& ctrl, QWidget* parent = nullptr);
    ~WatchListWindow();

private:
    Controller& controller;
    QTableView* tableView;
    MovieTableModel* model;
};
