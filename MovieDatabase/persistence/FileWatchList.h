#pragma once
#include <vector>
#include <string>
#include <fstream>
#include <windows.h>
#include "domain.h"


class FileWatchList {
protected:
    std::vector<Movie> movies;
    std::string filename;

public:
    FileWatchList(std::string file) : filename(file) {}
    virtual ~FileWatchList() = default;

    void addMovie(const Movie& m) {
        movies.push_back(m);
        this->saveToFile(); 
    }

    void removeMovie(const std::string& title) {
        auto it = std::find_if(movies.begin(), movies.end(), [&](const Movie& mm) { return mm.getTitle() == title; });
        if (it != movies.end()) {
            movies.erase(it);
            this->saveToFile();
        }
        else {
            throw std::invalid_argument("Movie not found in watchlist!");
        }
    }

    std::vector<Movie> getAllMovies() const { return movies; }

    virtual void saveToFile() = 0;
    virtual void displayList() const = 0;
};

class CSVWatchList : public FileWatchList {
public:
    CSVWatchList(std::string file) : FileWatchList(file) {}

    void saveToFile() override {
        std::ofstream file(this->filename);
        for (const auto& m : this->movies) {
            file << m << "\n";
        }
        file.close();
    }

    // Launches the OS-associated app for the file; excluded from unit-test coverage (opens a real shell window).
    void displayList() const override { ShellExecuteA(NULL, "open", this->filename.c_str(), NULL, NULL, SW_SHOWNORMAL); }
};

class HTMLWatchList : public FileWatchList {
public:
    HTMLWatchList(std::string file) : FileWatchList(file) {}

    void saveToFile() override {
        std::ofstream file(this->filename);
        file << "<!DOCTYPE html>\n<html>\n<head><title>Watchlist</title></head>\n<body>\n";
        file << "<table border=\"1\">\n";
        file << "<tr><th>Title</th><th>Genre</th><th>Year</th><th>Likes</th><th>Trailer</th></tr>\n";

        for (const auto& m : this->movies) {
            file << "<tr>"
                << "<td>" << m.getTitle() << "</td>"
                << "<td>" << m.getGenre() << "</td>"
                << "<td>" << m.getYear() << "</td>"
                << "<td>" << m.getLikes() << "</td>"
                << "<td><a href=\"" << m.getTrailer() << "\">Link</a></td>"
                << "</tr>\n";
        }

        file << "</table>\n</body>\n</html>\n";
        file.close();
    }

    // Launches the OS-associated app for the file; excluded from unit-test coverage (opens a real shell window).
    void displayList() const override { ShellExecuteA(NULL, "open", this->filename.c_str(), NULL, NULL, SW_SHOWNORMAL); }
};