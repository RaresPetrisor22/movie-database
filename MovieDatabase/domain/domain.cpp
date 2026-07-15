#include <iostream>
#include <string>
#include <sstream>
#include "domain.h"

Movie::Movie() {
	title = "Unknown";
	genre = "Unknown";
	year = 0;
	likes = 0;
	trailer = "";
	movieDuration = 0;
}

Movie::Movie(std::string t, std::string g, int y, int l, std::string tr) {

	title = t;
	genre = g;
	year = y;
	likes = l;
	trailer = tr;
	movieDuration = 0;
}

std::string Movie::getTitle() const { return title; }
std::string Movie::getGenre() const { return genre; }
int Movie::getYear() const { return year; }
int Movie::getLikes() const { return likes; }
std::string Movie::getTrailer() const { return trailer; }
int Movie::getMovieDuration() const { return movieDuration; }


void Movie::setTitle(std::string t) { title = t; }
void Movie::setGenre(std::string g) { genre = g; }
void Movie::setYear(int y) { year = y; }
void Movie::setLikes(int l) { likes = l; }
void Movie::setTrailer(std::string tr) { trailer = tr; }
void Movie::setMovieDuration(int dr) { movieDuration = dr; }

std::string Movie::toString() const {
	std::string result = "";
	result += "Title: " + this->title + "\n";
	result += "Genre: " + this->genre + "\n";
	result += "Year: " + std::to_string(this->year) + "\n";
	result += "Likes: " + std::to_string(this->likes) + "\n";
	result += "Trailer Link: " + this->trailer + "\n";
	result += "--------------------------\n";

	return result;
}

bool Movie::operator<(int nr) {
	return movieDuration < nr;
}

std::istream& operator>>(std::istream& is, Movie& m) {
	std::string line;
	if (std::getline(is, line)) {
		std::stringstream ss(line);
		std::string yearStr, likesStr;

		std::getline(ss, m.title, ',');
		std::getline(ss, m.genre, ',');
		std::getline(ss, yearStr, ',');
		std::getline(ss, likesStr, ',');
		std::getline(ss, m.trailer, ',');

		if (!yearStr.empty() && !likesStr.empty()) {
			m.year = std::stoi(yearStr);
			m.likes = std::stoi(likesStr);
		}
	}
	return is;
}

std::ostream& operator<<(std::ostream& os, const Movie& m) {
	os << m.title << "," << m.genre << "," << m.year << "," << m.likes << "," << m.trailer;
	return os;
}
