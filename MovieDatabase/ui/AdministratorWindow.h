#pragma once

#include "ui_AdministratorWindow.h"
#include <QtWidgets/QMainWindow>
#include "controller.h"

class AdministratorWindow : public QWidget
{
    Q_OBJECT

public:
    AdministratorWindow(Controller& controller, QWidget *parent = nullptr);
    ~AdministratorWindow();

private slots:
    void onAdd();
    void onUpdate();
    void onRemove();
    void onApplyFilter();
    void onUndo();
    void onRedo();

private:
    Ui::Form ui;

    void populateList();
    void clearFields();
    void updateUndoRedoButtons();
    bool shortForm;

    Controller& controller;
};
