#pragma once

#include <QtWidgets/QDialog>

class MainMenu : public QDialog {
	Q_OBJECT
public:
	~MainMenu();
	MainMenu(QWidget* parent = nullptr);
	bool isAdminMode() const { return admin_mode; }

private:
	void build_menu();
	bool admin_mode = false;

};
