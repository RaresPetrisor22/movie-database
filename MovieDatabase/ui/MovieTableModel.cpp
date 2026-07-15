#include "MovieTableModel.h"
#include <QString>

MovieTableModel::MovieTableModel(QObject* parent)
    : QAbstractTableModel(parent) {}

MovieTableModel::MovieTableModel(const std::vector<Movie>& m, QObject* parent)
    : QAbstractTableModel(parent), movies(m) {}

void MovieTableModel::setMovies(const std::vector<Movie>& m) {
    beginResetModel();
    movies = m;
    endResetModel();
}

int MovieTableModel::rowCount(const QModelIndex& parent) const {
    if (parent.isValid()) return 0;
    return static_cast<int>(movies.size());
}

int MovieTableModel::columnCount(const QModelIndex& parent) const {
    if (parent.isValid()) return 0;
    return 5;
}

QVariant MovieTableModel::data(const QModelIndex& index, int role) const {
    if (!index.isValid()) return QVariant();
    if (index.row() < 0 || index.row() >= static_cast<int>(movies.size()))
        return QVariant();

    const Movie& m = movies[index.row()];

    if (role == Qt::DisplayRole || role == Qt::EditRole) {
        switch (index.column()) {
        case 0: return QString::fromStdString(m.getTitle());
        case 1: return QString::fromStdString(m.getGenre());
        case 2: return m.getYear();
        case 3: return m.getLikes();
        case 4: return QString::fromStdString(m.getTrailer());
        default: return QVariant();
        }
    }
    return QVariant();
}

QVariant MovieTableModel::headerData(int section, Qt::Orientation orientation, int role) const {
    if (role != Qt::DisplayRole) return QVariant();
    if (orientation == Qt::Horizontal) {
        switch (section) {
        case 0: return QString("Title");
        case 1: return QString("Genre");
        case 2: return QString("Year");
        case 3: return QString("Likes");
        case 4: return QString("Trailer");
        default: return QVariant();
        }
    }
    return section + 1;
}
