#pragma once
#include "domain.h"
#include "repository.h"

class Action {
protected:
    Repository& repo;
public:
    Action(Repository& r) : repo(r) {}
    virtual ~Action() = default;

    virtual void executeUndo() = 0;
    virtual void executeRedo() = 0;
};

class AddAction : public Action {
private:
    Movie addedMovie;
public:
    AddAction(Repository& r, const Movie& m) : Action(r), addedMovie(m) {}

    void executeUndo() override {
        repo.removeMovie(addedMovie.getTitle());
    }

    void executeRedo() override {
        repo.addMovie(addedMovie);
    }
};

class RemoveAction : public Action {
private:
    Movie removedMovie;
public:
    RemoveAction(Repository& r, const Movie& m) : Action(r), removedMovie(m) {}

    void executeUndo() override {
        repo.addMovie(removedMovie);
    }

    void executeRedo() override {
        repo.removeMovie(removedMovie.getTitle());
    }
};

class UpdateAction : public Action {
private:
    Movie oldMovie;
    Movie newMovie;
public:
    UpdateAction(Repository& r, const Movie& oldM, const Movie& newM)
        : Action(r), oldMovie(oldM), newMovie(newM) {}

    void executeUndo() override {
        repo.updateMovie(
            newMovie.getTitle(),
            oldMovie.getTitle(),
            oldMovie.getGenre(),
            oldMovie.getYear(),
            oldMovie.getLikes(),
            oldMovie.getTrailer()
        );
    }

    void executeRedo() override {
        repo.updateMovie(
            oldMovie.getTitle(),
            newMovie.getTitle(),
            newMovie.getGenre(),
            newMovie.getYear(),
            newMovie.getLikes(),
            newMovie.getTrailer()
        );
    }
};
