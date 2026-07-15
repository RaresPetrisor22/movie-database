#pragma once

#include <QAbstractTableModel>
#include <QVariant>
#include <vector>
#include "domain.h"

class MovieTableModel : public QAbstractTableModel {
    Q_OBJECT

private:
    std::vector<Movie> movies;

public:
    MovieTableModel(QObject* parent = nullptr);
    MovieTableModel(const std::vector<Movie>& m, QObject* parent = nullptr);

    void setMovies(const std::vector<Movie>& m);

    int rowCount(const QModelIndex& parent = QModelIndex()) const override;
    int columnCount(const QModelIndex& parent = QModelIndex()) const override;
    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;
};
