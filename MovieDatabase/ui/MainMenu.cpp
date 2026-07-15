#include "MainMenu.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QLabel>

MainMenu::MainMenu(QWidget* parent) : QDialog(parent) {
	build_menu();
}

MainMenu::~MainMenu() {}

void MainMenu::build_menu() {
	setWindowTitle("Movie Manager - Main Menu");

	auto* diagLayout = new QVBoxLayout(this);

	diagLayout->addWidget(new QLabel("Choose mode:"));

	auto* btnsLayout = new QHBoxLayout(this);
	auto* adminBtn = new QPushButton("Admin");
	auto* userBtn = new QPushButton("User");

	btnsLayout->addWidget(adminBtn);
	btnsLayout->addWidget(userBtn);
	diagLayout->addLayout(btnsLayout); \

	connect(adminBtn, &QPushButton::clicked, this, [this]() {
	this->admin_mode = true;
	accept();
		});

	connect(userBtn, &QPushButton::clicked, this, [this]() {
		this->admin_mode = false;
		accept();
		});
	
}