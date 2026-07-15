#include "UserWindow.h"
#include "WatchListWindow.h"

#include <QPushButton>
#include <QMessageBox>
#include <QDesktopServices>
#include <QUrl>
#include <QString>
#include <set>
#include <string>


UserWindow::UserWindow(Controller& ctrl, QWidget* parent)
    : QWidget(parent), ctrl(ctrl)
{
    ui.setupUi(this);

    connect(ui.applyButton,         &QPushButton::clicked, this, &UserWindow::onApplyFilter);
    connect(ui.nextButton,          &QPushButton::clicked, this, &UserWindow::onNext);
    connect(ui.addWatchListButton,  &QPushButton::clicked, this, &UserWindow::onAddToWatchlist);
    connect(ui.showWatchListButton, &QPushButton::clicked, this, &UserWindow::onShowWatchlist);
    connect(ui.openTrailerButton,   &QPushButton::clicked, this, &UserWindow::onOpenTrailer);
    connect(ui.exitButton,          &QPushButton::clicked, this, &QWidget::close);

    populateGenres();

    currentSlice = ctrl.getAllMovies();
    currentIndex = 0;
    showCurrentMovie();
}

UserWindow::~UserWindow() {}

void UserWindow::populateGenres()
{
    ui.genreCombo->clear();
    ui.genreCombo->addItem("All");

    std::set<std::string> genres;
    for (const Movie& m : ctrl.getAllMovies()) {
        genres.insert(m.getGenre());
    }
    for (const std::string& g : genres) {
        ui.genreCombo->addItem(QString::fromStdString(g));
    }
}

void UserWindow::showCurrentMovie()
{
    if (currentSlice.empty()) {
        ui.titleValue   ->setText("(no movies)");
        ui.genreValue   ->setText("");
        ui.yearValue    ->setText("");
        ui.likesValue   ->setText("");
        ui.trailerValue ->setText("");
        ui.progressLabel->setText("Showing 0 / 0");
        return;
    }

    const Movie& m = currentSlice[currentIndex];
    ui.titleValue   ->setText(QString::fromStdString(m.getTitle()));
    ui.genreValue   ->setText(QString::fromStdString(m.getGenre()));
    ui.yearValue    ->setText(QString::number(m.getYear()));
    ui.likesValue   ->setText(QString::number(m.getLikes()));
    ui.trailerValue ->setText(QString::fromStdString(m.getTrailer()));
    ui.progressLabel->setText(QString("Showing %1 / %2")
        .arg(currentIndex + 1)
        .arg(currentSlice.size()));
}

void UserWindow::onApplyFilter()
{
    QString choice = ui.genreCombo->currentText();
    if (choice == "All") {
        currentSlice = ctrl.getAllMovies();
    } else {
        currentSlice = ctrl.filterByGenre(choice.toStdString());
    }

    currentIndex = 0;

    if (currentSlice.empty()) {
        QMessageBox::information(this, "No matches",
            "No movies found for genre '" + choice + "'.");
    }
    showCurrentMovie();
}

void UserWindow::onNext()
{
    if (currentSlice.empty()) return;
    currentIndex = (currentIndex + 1) % currentSlice.size();
    showCurrentMovie();
}

void UserWindow::onAddToWatchlist()
{
    if (currentSlice.empty()) {
        QMessageBox::information(this, "Nothing to add", "No movie is currently shown.");
        return;
    }
    try {
        ctrl.addMovieToWatchList(currentSlice[currentIndex]);
        QMessageBox::information(this, "Added",
            QString::fromStdString(
                "Added '" + currentSlice[currentIndex].getTitle() + "' to watchlist."));
    } catch (std::exception& e) {
        QMessageBox::warning(this, "Add failed", e.what());
    }
}

void UserWindow::onShowWatchlist()
{
    WatchListWindow* dlg = new WatchListWindow(ctrl, this);
    dlg->setAttribute(Qt::WA_DeleteOnClose);
    dlg->show();
}

void UserWindow::onOpenTrailer()
{
    if (currentSlice.empty()) return;
    QString url = QString::fromStdString(currentSlice[currentIndex].getTrailer());
    QDesktopServices::openUrl(QUrl::fromUserInput(url));
}
