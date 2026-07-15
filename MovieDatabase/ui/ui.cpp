#include <windows.h>
#include "domain.h"
#include "repository.h"
#include "controller.h"
#include "ui.h"
#include <iostream>
#include <string>
#include <stdexcept>

UI::UI(Controller& c) : ctrl(c) {};

void UI::administratorMode() {
	Validator validator;
	while (true) {
		std::cout << "\n-----Local Movie Database-----";
		std::cout << "\n1. Add a movie\n2. Remove a movie\n3. Update a movie\n4. Display all movies\n5. Filter by Movie Duration\n0. Exit\n";
		std::cout << "Choose an option: ";

		int option;
		std::cin >> option;

		switch (option) {
		case 1: {
			std::string title, genre, trailer;
			int year, likes;
			std::cout << "Enter title: ";
			std::getline(std::cin >> std::ws, title);
			std::cout << "Enter genre: ";
			std::getline(std::cin >> std::ws, genre);
			std::cout << "Enter year: ";
			std::cin >> year;
			std::cout << "Enter likes: ";
			std::cin >> likes;
			std::cout << "Enter trailer link: ";
			std::cin >> trailer;
			Movie m(title, genre, year, likes, trailer);
			try {
				validator.validate(m);
			}
			catch (const ValidationException& e) {
				std::cout << e.what() << "\n";
			}
			try {
				ctrl.addMovie(m);
				std::cout << "Movie added successfully!\n";
			}
			catch (const RepoException& e) {
				std::cout << e.what() << "\n";
			}
			break;
		}

		case 2: {
			std::string title;
			std::cout << "Enter title of the movie to remove: ";
			std::getline(std::cin >> std::ws, title);
			try {
				ctrl.removeMovie(title);
				std::cout << "Movie removed successfully!\n";
			}
			catch (const RepoException& e) {
				std::cout << e.what() << "\n";
			}
			break;
		}
		case 3: {
			std::string title, newTitle, genre, trailer;
			int year, likes;
			std::cout << "Enter title of the movie to update: ";
			std::getline(std::cin >> std::ws, title);
			std::cout << "Enter new title: ";
			std::getline(std::cin >> std::ws, newTitle);
			std::cout << "Enter new genre: ";
			std::getline(std::cin >> std::ws, genre);
			std::cout << "Enter new year: ";
			std::cin >> year;
			std::cout << "Enter new likes: ";
			std::cin >> likes;
			std::cout << "Enter new trailer link: ";
			std::cin >> trailer;
			Movie m(newTitle, genre, year, likes, trailer);
			try {
				validator.validate(m);
			}
			catch (const ValidationException& e) {
				std::cout << e.what() << "\n";
			}
			try {
				ctrl.updateMovie(title, newTitle, genre, year, likes, trailer);
				std::cout << "Movie updated successfully!\n";
			}
			catch (const RepoException& e) {
				std::cout << e.what() << "\n";
			}
			break;
		}
		case 4: {
			std::vector<Movie> movies = ctrl.getAllMovies();
			for (const auto& mv : movies) {
				std::cout << mv << std::endl;
			}
			break;
		}
		case 0: {
			std::cout << "Exiting...\n";
			return;
		}
		case 5: {
			int nr;
			std::cout << "Get movies with duration less than: ";
			std::cin >> nr;
			std::vector<Movie> movies = ctrl.filterByDuration(nr);
			for (const auto& mv : movies) {
				std::cout << "Duration - " << mv.getMovieDuration() << std::endl;
				std::cout << mv << std::endl;
				std::cout << std::endl;
			}

			break;
		}
		default: {
			std::cout << "Invalid option! Please try again.\n";
			break;
		}
		}
	}
}

void UI::userMode() {
	while (true) {
		std::cout << "\n-----Local Movie Database-----";
		std::cout << "\n1. Find Movie by Genre \n2. Remove a movie from watch list\n3. Display all movies in watch list\n0. Exit\n";
		std::cout << "Choose an option: ";
		int option;
		std::cin >> option;
		getchar();

		std::vector<Movie> movies;
		if (option == 1) {
			std::cout << "Enter genre: ";
			std::string genre;
			std::getline(std::cin, genre);

			if (genre.empty()) movies = this->ctrl.getAllMovies();
			else movies = this->ctrl.filterByGenre(genre);

			size_t size = movies.size();
			bool keepBrowsing = true;


			if (size == 0) {
				std::cout << "No movies found for this genre!" << std::endl;
				keepBrowsing = false;
			}

			int currentIndex = 0;

			while (keepBrowsing) {
				Movie currentMovie = movies[currentIndex];

				std::cout << currentMovie << std::endl;
				ShellExecuteA(NULL, "open", currentMovie.getTrailer().c_str(), NULL, NULL, SW_SHOWNORMAL);

				std::cout << "\nOptions:" << std::endl;
				std::cout << "1. Add to Watchlist" << std::endl;
				std::cout << "2. Skip (Next Movie)" << std::endl;
				std::cout << "3. Stop Browsing" << std::endl;
				std::cout << "Choose an option: ";

				int choice;
				std::cin >> choice;
				std::cout << std::endl;

				if (choice == 1) {
					try {
						this->ctrl.addMovieToWatchList(currentMovie);
						std::cout << "Added to Watchlist!" << std::endl;
					}
					catch (const std::invalid_argument& e) {
						std::cout << e.what() << std::endl;
					}

					currentIndex = (currentIndex + 1) % size;
				}
				else if (choice == 2) {
					currentIndex = (currentIndex + 1) % size;
				}
				else if (choice == 3) {
					keepBrowsing = false;
				}
				else {
					std::cout << "Invalid choice. Try again." << std::endl;
				}
			}
		}
		else if (option == 2) {
			std::cout << "Movie to be removed: ";
			std::string name;
			std::getline(std::cin >> std::ws, name);
			try {
				std::cout << "Do you want to rate the movie with a like? (y/n)" << std::endl;
				std::string rate_choice;
				std::getline(std::cin >> std::ws, rate_choice);
				if (rate_choice == "y") {
					try {
						Movie& m = ctrl.getMovieByTitle(name);
						m.setLikes(m.getLikes() + 1);
						std::cout << "Movie has been liked!" << std::endl;
					}
					catch (const std::invalid_argument& e) {
						std::cout << e.what() << std::endl;
					}
				}
				ctrl.removeMovieFromWatchList(name);
			}
			catch (const std::invalid_argument& e) {
				std::cout << e.what() << std::endl;
			}

		}
		else if (option == 3) {
			this->ctrl.displayWatchList();
		}
		else if (option == 0) {
			return;
		}
		else {
			std::cout << "Invalid Command!!" << std::endl;
		}
	}
}

void UI::startUI() {
	std::cout << "1. Enter administrator mode\n2. Enter user mode" << std::endl;
	std::cout << "Choose an option: ";
	int option;
	std::cin >> option;
	if (option == 1)
		this->administratorMode();
	else
		this->userMode();
}