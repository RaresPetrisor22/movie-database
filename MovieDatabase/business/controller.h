#pragma once
#include "domain.h"
#include "DynamicVector.h"
#include "repository.h"
#include "FileWatchList.h"
#include "Action.h"
#include <string>
#include <vector>
#include <memory>

class Controller {
private:
	Repository& repo;
	FileWatchList* watchList;

	std::vector<std::unique_ptr<Action>> undoStack;
	std::vector<std::unique_ptr<Action>> redoStack;

public:
	Controller(Repository& r, FileWatchList* wl);
	void addMovie(const Movie& m);
	void removeMovie(const std::string& title);
	void updateMovie(const std::string& title, const std::string& newTitle, const std::string& genre, const int year, const int likes, const std::string& trailer);

	void addMovieToWatchList(Movie& m);
	void removeMovieFromWatchList(std::string& title);
	std::vector<Movie> getAllMoviesFromWatchList();
	std::vector<Movie> filterByGenre(const std::string& genre);
	Movie& getMovieByTitle(std::string& title);

	std::vector<Movie> getAllMovies() const;
	std::vector<Movie> filterByDuration(int nr) const;

	void displayWatchList() const;

	void undo();
	void redo();
	bool canUndo() const;
	bool canRedo() const;
};
