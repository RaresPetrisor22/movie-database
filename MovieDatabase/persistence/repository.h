#pragma once
#include "domain.h"
#include "DynamicVector.h"
#include <string>
#include <vector>
#include <exception>

class Repository {
private:
	std::vector<Movie> movies;
	std::string filename;

	void loadFromFile();
	void saveToFile();
public:
	Repository(const std::string& filename);
	void addMovie(const Movie& m);
	void removeMovie(const std::string& title);
	void updateMovie(const std::string& title, const std::string& newTitle, const std::string& genre, const int year, const int likes, const std::string& trailer);
	Movie& getMovieByTitle(std::string& title);
	std::vector<Movie> getAllMovies() const;

};

class RepoException : public std::exception {
private:
	std::string message;
public:
	RepoException(std::string msg) : message(msg) {}

	const char* what() const noexcept override {
		return message.c_str();
	}
};

