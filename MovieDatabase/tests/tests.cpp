#include "tests.h"
#include "domain.h"
#include "DynamicVector.h"
#include "repository.h"
#include "controller.h"
#include "FileWatchList.h"
#include <cassert>
#include <cstdio>
#include <iostream>
#include <sstream>
#include <string>

void testMovie() {
    Movie m1;
    assert(m1.getTitle() == "Unknown");
    assert(m1.getGenre() == "Unknown");
    assert(m1.getYear() == 0);
    assert(m1.getLikes() == 0);
    assert(m1.getTrailer() == "");
    assert(m1.getMovieDuration() == 0);

    Movie m2("A", "B", 2000, 10, "http://trailer");
    assert(m2.getTitle() == "A");
    assert(m2.getGenre() == "B");
    assert(m2.getYear() == 2000);
    assert(m2.getLikes() == 10);
    assert(m2.getTrailer() == "http://trailer");

    m2.setTitle("X"); m2.setGenre("Y"); m2.setYear(1999); m2.setLikes(5); m2.setTrailer("t");
    assert(m2.getTitle() == "X");
    assert(m2.getGenre() == "Y");
    assert(m2.getYear() == 1999);
    assert(m2.getLikes() == 5);
    assert(m2.getTrailer() == "t");

    m2.setMovieDuration(120);
    assert(m2.getMovieDuration() == 120);
    assert(!(m2 < 120));
    assert(m2 < 121);

    Movie m3("The Matrix", "Sci-Fi", 1999, 500, "link.com");
    std::string expected =
        "Title: The Matrix\n"
        "Genre: Sci-Fi\n"
        "Year: 1999\n"
        "Likes: 500\n"
        "Trailer Link: link.com\n"
        "--------------------------\n";
    assert(m3.toString() == expected);

    std::ostringstream out;
    out << m3;
    assert(out.str() == "The Matrix,Sci-Fi,1999,500,link.com");

    std::istringstream in("Parsed Title,Parsed Genre,2005,42,parsed-trailer\n");
    Movie parsed;
    in >> parsed;
    assert(parsed.getTitle() == "Parsed Title");
    assert(parsed.getGenre() == "Parsed Genre");
    assert(parsed.getYear() == 2005);
    assert(parsed.getLikes() == 42);
    assert(parsed.getTrailer() == "parsed-trailer");
}

void testValidator() {
    Validator validator;

    Movie valid("Interstellar", "Sci-Fi", 2014, 900, "link");
    validator.validate(valid);

    auto expectThrow = [&](const Movie& m) {
        bool threw = false;
        try { validator.validate(m); }
        catch (const ValidationException& ex) {
            threw = true;
            assert(std::string(ex.what()).find("empty") != std::string::npos
                || std::string(ex.what()).find("valid year") != std::string::npos
                || std::string(ex.what()).find("negative") != std::string::npos);
        }
        assert(threw);
    };

    expectThrow(Movie("", "Sci-Fi", 2014, 900, "link"));
    expectThrow(Movie("T", "", 2014, 900, "link"));
    expectThrow(Movie("T", "Sci-Fi", 1800, 900, "link"));
    expectThrow(Movie("T", "Sci-Fi", 2200, 900, "link"));
    expectThrow(Movie("T", "Sci-Fi", 2014, -1, "link"));
    expectThrow(Movie("T", "Sci-Fi", 2014, 900, ""));
}

void testDynamicVector() {
    DynamicVector<Movie> v(2);
    assert(v.getLength() == 0);

    Movie a("A", "G", 2001, 1, "t1");
    Movie b("B", "G", 2002, 2, "t2");
    Movie c("C", "G", 2003, 3, "t3");

    v.add(a);
    assert(v.getLength() == 1);
    v.add(b);
    assert(v.getLength() == 2);

    v.add(c); // forces a resize (capacity 2 -> 4)
    assert(v.getLength() == 3);
    assert(v.get(0).getTitle() == "A");
    assert(v.get(1).getTitle() == "B");
    assert(v.get(2).getTitle() == "C");

    v.remove(1); // removes "B"
    assert(v.getLength() == 2);
    assert(v.get(0).getTitle() == "A" && v.get(1).getTitle() == "C");

    // Out-of-range removals are no-ops by design (see DynamicVector::remove).
    int lenBefore = v.getLength();
    v.remove(-1);
    v.remove(99);
    assert(v.getLength() == lenBefore);

    DynamicVector<Movie> copy(v);
    assert(copy.getLength() == v.getLength());
    assert(copy.get(0).getTitle() == v.get(0).getTitle());

    DynamicVector<Movie> assigned(1);
    assigned = v;
    assert(assigned.getLength() == v.getLength());
    assert(assigned.get(0).getTitle() == v.get(0).getTitle());

    v = v; // self-assignment must be a safe no-op
    assert(v.getLength() == lenBefore);

    DynamicVector<Movie> plus(1);
    plus = plus + a;
    assert(plus.getLength() == 1);
    assert(plus.get(0).getTitle() == "A");
    plus = b + plus;
    assert(plus.getLength() == 2);
    assert(plus.get(0).getTitle() == "A");
    assert(plus.get(1).getTitle() == "B");
}

void testRepository() {
    const std::string file = "test_repo.txt";
    std::remove(file.c_str());

    Repository repo(file);
    assert(repo.getAllMovies().empty());

    Movie a("A", "Genre", 2001, 1, "t1");
    Movie b("B", "Genre", 2002, 2, "t2");

    repo.addMovie(a);
    assert(repo.getAllMovies().size() == 1);

    bool threw = false;
    try { repo.addMovie(a); }
    catch (const RepoException& ex) {
        threw = true;
        assert(std::string(ex.what()) == "Movie already exists!");
    }
    assert(threw);

    repo.addMovie(b);
    assert(repo.getAllMovies().size() == 2);

    std::string titleA = "A";
    assert(repo.getMovieByTitle(titleA).getGenre() == "Genre");

    std::string missing = "Z";
    threw = false;
    try { repo.getMovieByTitle(missing); }
    catch (const RepoException&) { threw = true; }
    assert(threw);

    repo.updateMovie("A", "A2", "NewGenre", 2010, 7, "newTrailer");
    std::string a2 = "A2";
    assert(repo.getMovieByTitle(a2).getGenre() == "NewGenre");
    assert(repo.getMovieByTitle(a2).getYear() == 2010);

    threw = false;
    try { repo.updateMovie("NoSuchTitle", "X", "G", 2000, 0, "t"); }
    catch (const RepoException&) { threw = true; }
    assert(threw);

    repo.removeMovie("B");
    assert(repo.getAllMovies().size() == 1);

    threw = false;
    try { repo.removeMovie("NoSuchTitle"); }
    catch (const RepoException&) { threw = true; }
    assert(threw);

    // Persistence round-trip: a fresh Repository instance must reload from disk.
    Repository reloaded(file);
    assert(reloaded.getAllMovies().size() == 1);
    assert(reloaded.getAllMovies()[0].getTitle() == "A2");

    std::remove(file.c_str());

    // A filename whose directory doesn't exist makes saveToFile()'s ofstream fail to
    // open; addMovie() must not throw, it just logs the error and keeps the in-memory state.
    Repository unwritable("no_such_directory\\unwritable.txt");
    unwritable.addMovie(Movie("C", "Genre", 2020, 0, "t"));
    assert(unwritable.getAllMovies().size() == 1);
}

void testFileWatchList() {
    const std::string csvFile = "test_watchlist.csv";
    const std::string htmlFile = "test_watchlist.html";
    std::remove(csvFile.c_str());
    std::remove(htmlFile.c_str());

    Movie m1("Dune", "Sci-Fi", 2021, 500, "link4");

    CSVWatchList csv(csvFile);
    csv.addMovie(m1);
    assert(csv.getAllMovies().size() == 1);

    std::string title = "Dune";
    csv.removeMovie(title);
    assert(csv.getAllMovies().empty());

    bool threw = false;
    std::string missing = "Missing";
    try { csv.removeMovie(missing); }
    catch (const std::invalid_argument&) { threw = true; }
    assert(threw);

    HTMLWatchList html(htmlFile);
    html.addMovie(m1);
    assert(html.getAllMovies().size() == 1);

    std::remove(csvFile.c_str());
    std::remove(htmlFile.c_str());
}

void testController() {
    const std::string repoFile = "test_ctrl_repo.txt";
    const std::string watchFile = "test_ctrl_watch.csv";
    std::remove(repoFile.c_str());
    std::remove(watchFile.c_str());

    Repository repo(repoFile);
    CSVWatchList watchList(watchFile);
    Controller ctrl(repo, &watchList);

    assert(!ctrl.canUndo());
    assert(!ctrl.canRedo());

    bool threw = false;
    try { ctrl.undo(); } catch (const std::runtime_error&) { threw = true; }
    assert(threw);

    threw = false;
    try { ctrl.redo(); } catch (const std::runtime_error&) { threw = true; }
    assert(threw);

    Movie m1("The Matrix", "Sci-Fi", 1999, 100, "link1");
    Movie m2("Inception", "Sci-Fi", 2010, 200, "link2");
    Movie m3("Gladiator", "Action", 2000, 150, "link3");
    m1.setMovieDuration(100);
    m2.setMovieDuration(200);
    m3.setMovieDuration(150);

    ctrl.addMovie(m1);
    ctrl.addMovie(m2);
    ctrl.addMovie(m3);
    assert(ctrl.getAllMovies().size() == 3);
    assert(ctrl.canUndo());

    assert(ctrl.filterByDuration(101).size() == 2);  // Inception(200), Gladiator(150)
    assert(ctrl.filterByDuration(199).size() == 1);  // Inception(200)
    assert(ctrl.filterByDuration(200).size() == 0);  // strictly greater than
    assert(ctrl.filterByGenre("Sci-Fi").size() == 2);
    assert(ctrl.filterByGenre("Action").size() == 1);

    // Undo the last two adds.
    ctrl.undo();
    assert(ctrl.getAllMovies().size() == 2);
    ctrl.undo();
    assert(ctrl.getAllMovies().size() == 1);
    assert(ctrl.canRedo());

    // Redo restores them in the same order they were undone.
    ctrl.redo();
    assert(ctrl.getAllMovies().size() == 2);
    ctrl.redo();
    assert(ctrl.getAllMovies().size() == 3);
    assert(!ctrl.canRedo());

    // A fresh action clears the redo stack.
    ctrl.removeMovie("Gladiator");
    assert(ctrl.getAllMovies().size() == 2);
    assert(!ctrl.canRedo());

    ctrl.undo(); // undo the removal -> Gladiator is back
    assert(ctrl.getAllMovies().size() == 3);

    ctrl.redo(); // redo the removal -> Gladiator is gone again (RemoveAction::executeRedo)
    assert(ctrl.getAllMovies().size() == 2);
    ctrl.undo(); // undo it again so the rest of the test sees Gladiator present
    assert(ctrl.getAllMovies().size() == 3);

    std::string oldTitle = "The Matrix";
    ctrl.updateMovie(oldTitle, "The Matrix Reloaded", "Sci-Fi", 2003, 110, "link1_new");
    std::string newTitle = "The Matrix Reloaded";
    assert(ctrl.getMovieByTitle(newTitle).getYear() == 2003);

    ctrl.undo(); // revert the update
    assert(ctrl.getMovieByTitle(oldTitle).getYear() == 1999);

    ctrl.redo(); // reapply the update
    assert(ctrl.getMovieByTitle(newTitle).getYear() == 2003);

    // Watchlist behaviour (separate from the undo/redo stacks).
    Movie watch("Dune", "Sci-Fi", 2021, 500, "link4");
    ctrl.addMovieToWatchList(watch);
    assert(ctrl.getAllMoviesFromWatchList().size() == 1);
    assert(ctrl.getAllMoviesFromWatchList()[0].getTitle() == "Dune");

    std::string watchTitle = "Dune";
    ctrl.removeMovieFromWatchList(watchTitle);
    assert(ctrl.getAllMoviesFromWatchList().empty());

    std::remove(repoFile.c_str());
    std::remove(watchFile.c_str());
}

bool runAllTests() {
    struct NamedTest { const char* name; void(*fn)(); };
    NamedTest tests[] = {
        {"Movie", testMovie},
        {"Validator", testValidator},
        {"DynamicVector", testDynamicVector},
        {"Repository", testRepository},
        {"FileWatchList", testFileWatchList},
        {"Controller", testController},
    };

    bool allPassed = true;
    for (const auto& t : tests) {
        try {
            t.fn();
            std::cout << "[PASS] " << t.name << "\n";
        }
        catch (const std::exception& ex) {
            std::cerr << "[FAIL] " << t.name << ": " << ex.what() << "\n";
            allPassed = false;
        }
        catch (...) {
            std::cerr << "[FAIL] " << t.name << ": unknown exception\n";
            allPassed = false;
        }
    }

    std::cout << (allPassed ? "All tests passed.\n" : "Some tests FAILED.\n");
    return allPassed;
}
