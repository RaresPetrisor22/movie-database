#pragma once
#include "ui_UserWindow.h"
#include "controller.h"
#include "domain.h"
#include <QtWidgets/QMainWindow>

class UserWindow : public QWidget {
	Q_OBJECT
public:
	UserWindow(Controller& ctrl, QWidget* parent = nullptr);
	~UserWindow();

private slots:
	void onApplyFilter();
	void onNext();
	void onAddToWatchlist();
	void onShowWatchlist();
	void onOpenTrailer();

private:
	Ui::UserWindow ui;

	void populateGenres();
	void showCurrentMovie();

	Controller& ctrl;

	std::vector<Movie> currentSlice;
	size_t currentIndex = 0;
};