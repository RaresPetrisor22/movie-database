#include "domain.h"
#include "DynamicVector.h"
#include "repository.h"
#include <string>
#include <stdexcept>
#include <algorithm>
#include<iostream>
#include <fstream>

Repository::Repository(const std::string& filename) : filename(filename) {
	if(this->filename != "")
		this->loadFromFile();
}

void Repository::loadFromFile() {
	std::ifstream file(this->filename);
	if (!file.is_open()) {
		return;
	}

	Movie m;
	while (file >> m) {
		this->movies.push_back(m);
	}

	file.close();
}

void Repository::saveToFile() {
	std::ofstream file(this->filename);
	if (!file.is_open()) {
		std::cerr << "Error opening file for writing!" << std::endl;
		return;
	}

	for (const auto& m : this->movies) {
		file << m << "\n";
	}

	file.close();
}

void Repository::addMovie(const Movie& m) {
	if (std::any_of(movies.begin(), movies.end(), [&](const Movie& mm){ return mm.getTitle() == m.getTitle(); })) {
		throw RepoException("Movie already exists!");
	}

	movies.push_back(m);
	this->saveToFile();
}

void Repository::removeMovie(const std::string& title) {
	auto it = std::find_if(movies.begin(), movies.end(), [&](const Movie& mm){ return mm.getTitle() == title; });
	if (it == movies.end()) {
		throw RepoException("Movie not found!");
	}
	movies.erase(it);
	this->saveToFile();
}

void Repository::updateMovie(const std::string& title, const std::string& newTitle, const std::string& genre, const int year, const int likes, const std::string& trailer) {
	auto it = std::find_if(movies.begin(), movies.end(), [&](const Movie& mm){ return mm.getTitle() == title; });
	if (it == movies.end()) {
		throw RepoException("Movie not found!");
	}
	Movie& old_movie = *it;
	old_movie.setTitle(newTitle);
	old_movie.setGenre(genre);
	old_movie.setYear(year);
	old_movie.setLikes(likes);
	old_movie.setTrailer(trailer);
	this->saveToFile();
}


std::vector<Movie> Repository::getAllMovies() const {
    return movies;
}

Movie& Repository::getMovieByTitle(std::string& title) {
    auto it = std::find_if(movies.begin(), movies.end(), [&](const Movie& mm){ return mm.getTitle() == title; });
	if (it == movies.end()) {
		throw RepoException("Movie not found in the database!");
	}
	return *it;
}
