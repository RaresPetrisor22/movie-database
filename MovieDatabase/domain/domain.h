#pragma once
#include <string>
#include <iostream>

class Movie {
private:
	std::string title;
	std::string genre;
	int year;
	int likes;
	std::string trailer;
	int movieDuration;

public:

	friend std::istream& operator>>(std::istream& is, Movie& m);

	friend std::ostream& operator<<(std::ostream& os, const Movie& m);

	Movie();
	Movie(std::string t, std::string g, int y, int l, std::string tr);

	bool operator<(int nr);

	std::string getTitle() const;
	std::string getGenre() const;
	int getYear() const;
	int getLikes() const;
	std::string getTrailer() const;
	int getMovieDuration() const;

	void setTitle(std::string t);
	void setGenre(std::string g);
	void setYear(int y);
	void setLikes(int l);
	void setTrailer(std::string tr);
	void setMovieDuration(int dr);

	std::string toString() const;
};

class ValidationException : public std::exception {
private:
	std::string message;
public:
	ValidationException(std::string msg) : message(msg) {}

	const char* what() const noexcept override {
		return message.c_str();
	}
};

class Validator {
public:
	void validate(const Movie& m) {
		std::string errors = "";

		if (m.getTitle().empty())
			errors += "The movie title cannot be empty!\n";

		if (m.getGenre().empty()) {
			errors += "The movie genre cannot be empty!\n";
		}

		if (m.getYear() < 1888 || m.getYear() > 2100) {
			errors += "The release year must be a valid year (e.g., between 1888 and 2100)!\n";
		}
		if (m.getLikes() < 0) {
			errors += "The number of likes cannot be negative!\n";
		}

		if (m.getTrailer().empty()) {
			errors += "The trailer link cannot be empty!\n";
		}

		if (!errors.empty()) {
			throw ValidationException(errors);
		}

	}

};

