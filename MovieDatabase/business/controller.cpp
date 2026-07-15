#include "domain.h"
#include "repository.h"
#include "controller.h"
#include "Action.h"
#include <string>
#include <algorithm>
#include <iterator>
#include <memory>


Controller::Controller(Repository& r, FileWatchList* wl) : repo(r), watchList(wl) {}

void Controller::addMovie(const Movie& m) {
	repo.addMovie(m);
	undoStack.push_back(std::make_unique<AddAction>(repo, m));
	redoStack.clear();
}

void Controller::removeMovie(const std::string& title) {
	std::string t = title;
	Movie removed = repo.getMovieByTitle(t);
	repo.removeMovie(title);
	undoStack.push_back(std::make_unique<RemoveAction>(repo, removed));
	redoStack.clear();
}

void Controller::updateMovie(const std::string& title, const std::string& newTitle, const std::string& genre, int year, int likes, const std::string& trailer) {
	std::string t = title;
	Movie oldMovie = repo.getMovieByTitle(t);
	repo.updateMovie(title, newTitle, genre, year, likes, trailer);
	Movie newMovie(newTitle, genre, year, likes, trailer);
	undoStack.push_back(std::make_unique<UpdateAction>(repo, oldMovie, newMovie));
	redoStack.clear();
}

std::vector<Movie> Controller::getAllMovies() const {
	return repo.getAllMovies();
}

std::vector<Movie> Controller::filterByGenre(const std::string& genre) {
	auto movies = repo.getAllMovies();
	std::vector<Movie> movies_by_genre;
	std::copy_if(movies.begin(), movies.end(), std::back_inserter(movies_by_genre), [&](const Movie& m){
		return m.getGenre() == genre;
	});
	return movies_by_genre;
}

void Controller::addMovieToWatchList(Movie& movie) {
	this->watchList->addMovie(movie);
}

void Controller::removeMovieFromWatchList(std::string& title) {
	watchList->removeMovie(title);
}

std::vector<Movie> Controller::getAllMoviesFromWatchList() {
	return watchList->getAllMovies();
}

Movie& Controller::getMovieByTitle(std::string& title) {
	return repo.getMovieByTitle(title);
}

std::vector<Movie> Controller::filterByDuration(int nr) const {
	auto movies = getAllMovies();
	std::vector<Movie> filteredMovies;
	std::copy_if(movies.begin(), movies.end(), std::back_inserter(filteredMovies), [&](const Movie& m){
		return m.getMovieDuration() > nr;
	});
	return filteredMovies;
}

// Delegates to the OS shell; excluded from unit-test coverage (opens a real shell window).
void Controller::displayWatchList() const { watchList->displayList(); }

void Controller::undo() {
	if (undoStack.empty()) {
		throw std::runtime_error("Nothing to undo!");
	}
	std::unique_ptr<Action> action = std::move(undoStack.back());
	undoStack.pop_back();
	action->executeUndo();
	redoStack.push_back(std::move(action));
}

void Controller::redo() {
	if (redoStack.empty()) {
		throw std::runtime_error("Nothing to redo!");
	}
	std::unique_ptr<Action> action = std::move(redoStack.back());
	redoStack.pop_back();
	action->executeRedo();
	undoStack.push_back(std::move(action));
}

bool Controller::canUndo() const {
	return !undoStack.empty();
}

bool Controller::canRedo() const {
	return !redoStack.empty();
}
