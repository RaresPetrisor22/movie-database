#include "AdministratorWindow.h"

#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QListWidgetItem>
#include <QMessageBox>
#include <QString>
#include <QShortcut>
#include <QKeySequence>

AdministratorWindow::AdministratorWindow(Controller& c, QWidget *parent)
    : QWidget(parent), controller(c)
{
    ui.setupUi(this);

    connect(ui.addButton, &QPushButton::clicked, this, &AdministratorWindow::onAdd);
    connect(ui.removeButton, &QPushButton::clicked, this, &AdministratorWindow::onRemove);
    connect(ui.updateButton, &QPushButton::clicked, this, &AdministratorWindow::onUpdate);
    connect(ui.applyButton, &QPushButton::clicked, this, &AdministratorWindow::onApplyFilter);
    connect(ui.undoButton, &QPushButton::clicked, this, &AdministratorWindow::onUndo);
    connect(ui.redoButton, &QPushButton::clicked, this, &AdministratorWindow::onRedo);

    auto* undoShortcut = new QShortcut(QKeySequence(QKeySequence::Undo), this);
    connect(undoShortcut, &QShortcut::activated, this, &AdministratorWindow::onUndo);

    // QKeySequence::Redo already maps to Ctrl+Y on Windows (and Ctrl+Shift+Z).
    // Registering an explicit Ctrl+Y in addition makes the binding ambiguous,
    // so Qt fires activatedAmbiguously instead of activated and onRedo never runs.
    auto* redoShortcut = new QShortcut(QKeySequence(QKeySequence::Redo), this);
    connect(redoShortcut, &QShortcut::activated, this, &AdministratorWindow::onRedo);

    ui.displayCombo->addItem("Long Form");
    ui.displayCombo->addItem("Short Form");
    this->shortForm = false;
    populateList();
    updateUndoRedoButtons();
}

AdministratorWindow::~AdministratorWindow() {}


void AdministratorWindow::populateList()
{
    ui.movieList->clear();
    for (const Movie& m : controller.getAllMovies()) {
        QString display;
        if (shortForm == false) {
            display = QString::fromStdString(m.getTitle())
                + " (" + QString::number(m.getYear()) + ") - "
                + QString::fromStdString(m.getGenre())
                + " - " + QString::number(m.getLikes()) + " likes";
        }
        else
            display = QString::fromStdString(m.getTitle());

        auto* item = new QListWidgetItem(display);
        item->setData(Qt::UserRole, QString::fromStdString(m.getTitle()));
        ui.movieList->addItem(item);
    }
}

void AdministratorWindow::clearFields()
{
    ui.titleLine->clear();
    ui.genreLine->clear();
    ui.yearLine->clear();
    ui.likesLine->clear();
    ui.trailerLine->clear();
}

void AdministratorWindow::updateUndoRedoButtons()
{
    ui.undoButton->setEnabled(controller.canUndo());
    ui.redoButton->setEnabled(controller.canRedo());
}

void AdministratorWindow::onAdd() {
    try {
        Movie m(
            ui.titleLine->text().toStdString(),
            ui.genreLine->text().toStdString(),
            ui.yearLine->text().toInt(),
            ui.likesLine->text().toInt(),
            ui.trailerLine->text().toStdString()
        );
        controller.addMovie(m);
    }
    catch (std::exception& e) {
        QMessageBox::warning(this, "Add failed", e.what());
        return;
    }
    populateList();
    clearFields();
    updateUndoRedoButtons();
}

void AdministratorWindow::onUpdate() {
    auto* item = ui.movieList->currentItem();
    if (!item) {
        QMessageBox::information(this, "No Selection", "Select a movie first!");
        return;
    }
    std::string title = item->data(Qt::UserRole).toString().toStdString();

    try {
        controller.updateMovie(
            title,
            ui.titleLine->text().toStdString(),
            ui.genreLine->text().toStdString(),
            ui.yearLine->text().toInt(),
            ui.likesLine->text().toInt(),
            ui.trailerLine->text().toStdString()
        );
    }
    catch (std::exception& e) {
        QMessageBox::warning(this, "Update failed!", e.what());
        return;
    }
    populateList();
    updateUndoRedoButtons();
}

void AdministratorWindow::onRemove()
{
    auto* item = ui.movieList->currentItem();
    if (!item) {
        QMessageBox::information(this, "No selection", "Select a movie first.");
        return;
    }
    std::string title = item->data(Qt::UserRole).toString().toStdString();
    try {
        controller.removeMovie(title);
    }
    catch (std::exception& e) {
        QMessageBox::warning(this, "Delete failed", e.what());
        return;
    }
    populateList();
    clearFields();
    updateUndoRedoButtons();
}

void AdministratorWindow::onApplyFilter()
{
    QString choice = ui.displayCombo->currentText();
    if (choice == "Long Form") {
        this->shortForm = false;
    }
    else {
        this->shortForm = true;
    }
    populateList();
}

void AdministratorWindow::onUndo()
{
    try {
        controller.undo();
    }
    catch (std::exception& e) {
        QMessageBox::information(this, "Undo", e.what());
        return;
    }
    populateList();
    updateUndoRedoButtons();
}

void AdministratorWindow::onRedo()
{
    try {
        controller.redo();
    }
    catch (std::exception& e) {
        QMessageBox::information(this, "Redo", e.what());
        return;
    }
    populateList();
    updateUndoRedoButtons();
}
