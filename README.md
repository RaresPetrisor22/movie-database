# 🎬 Movie Database

A desktop **movie database application** written in modern C++ with a **Qt 6** graphical user interface.
Users can browse movies and build a personal watchlist, while administrators can perform full
**CRUD** (Create, Read, Update, Delete) operations — with **undo/redo** support powered by the
Command design pattern.

> 📚 **University project** — developed for the Object-Oriented Programming course as an exercise in
> layered software architecture, design patterns, and test-driven development.

---

## ✨ Features

- **Two operating modes**
  - 👤 **User mode** — browse the movie catalogue, filter movies, and manage a personal watchlist.
  - 🛠️ **Admin mode** — add, edit, and delete movies (full CRUD).
- **Undo / Redo** — every add, remove, and update operation is reversible, implemented with the
  **Command design pattern** (`Action`, `AddAction`, `RemoveAction`, `UpdateAction`).
- **Filtering** — filter movies by genre or by duration.
- **Exportable watchlist** — the watchlist can be persisted and opened as **CSV** or **HTML**,
  using an abstract `FileWatchList` base with interchangeable implementations.
- **Input validation** — a dedicated `Validator` enforces domain rules (non-empty fields, valid
  release year, non-negative likes) and reports errors through custom exceptions.
- **File-backed persistence** — movies are stored in a plain-text file and loaded on startup.
- **Unit tests** — the domain, repository, and controller layers are covered by unit tests
  (**> 98 % coverage**).

---

## 🏛️ Architecture

The application follows a classic **layered architecture**, keeping the user interface, business
logic, and data access cleanly separated:

```
┌─────────────────────────────────────────────────────┐
│  Presentation Layer  (Qt GUI)                        │
│  MainMenu · AdministratorWindow · UserWindow ·       │
│  WatchListWindow · MovieTableModel                   │
└───────────────────────────┬─────────────────────────┘
                            │
┌───────────────────────────▼─────────────────────────┐
│  Business Layer  (Controller)                        │
│  Controller · Action (Command pattern: undo/redo)    │
└───────────────────────────┬─────────────────────────┘
                            │
┌───────────────────────────▼─────────────────────────┐
│  Persistence Layer  (Repository)                     │
│  Repository · FileWatchList (CSV / HTML)             │
└───────────────────────────┬─────────────────────────┘
                            │
┌───────────────────────────▼─────────────────────────┐
│  Domain Layer                                        │
│  Movie · Validator · Exceptions                      │
└─────────────────────────────────────────────────────┘
```

A full UML class diagram is available in [`docs/diagram.md`](docs/diagram.md).

### Design patterns used
- **Command pattern** — encapsulates each repository operation as a reversible `Action`, enabling
  undo/redo via two stacks in the `Controller`.
- **Template Method / Strategy** — `FileWatchList` defines the watchlist workflow while
  `CSVWatchList` and `HTMLWatchList` provide the export format.
- **Model/View** — `MovieTableModel` bridges the domain data to Qt's table views.

---

## 🛠️ Tech Stack

| | |
|------------------|-------------------------------------------|
| **Language**     | C++17                                      |
| **GUI**          | Qt 6.5 (Widgets)                           |
| **Build**        | Visual Studio 2022 / MSBuild (MSVC 64-bit) |
| **Testing**      | Custom unit-test suite (`assert`-based)    |

---

## 🚀 Getting Started

### Prerequisites
- **Visual Studio 2022** with the *Desktop development with C++* workload
- **Qt 6.5** (MSVC 2019 64-bit) + the **Qt Visual Studio Tools** extension

### Build & Run
1. Clone the repository:
   ```bash
   git clone https://github.com/<your-username>/movie-database.git
   ```
2. Open `MovieDatabase.slnx` in Visual Studio.
3. In *Qt VS Tools → Qt Versions*, make sure a **Qt 6.5 MSVC 64-bit** kit is registered.
4. Select the **x64** configuration and press **F5** to build and run.

The application starts with the **main menu**, where you choose **User** or **Admin** mode.

---

## 📂 Repository Structure

```
movie-database/
├── MovieDatabase/               # Visual Studio C++ project
│   ├── domain/                  # Movie entity, Validator, exceptions
│   ├── persistence/             # Repository, FileWatchList (CSV/HTML), DynamicVector
│   ├── business/                # Controller + Command pattern (Action)
│   ├── ui/                      # Qt windows, models, .ui forms, resources
│   ├── tests/                   # Unit tests
│   ├── data/                    # Data files (movies.txt, generated watchlists)
│   ├── main.cpp                 # Application entry point
│   └── MovieDatabase.vcxproj    # Project file
├── docs/                        # UML diagram and documentation
├── MovieDatabase.slnx           # Visual Studio solution
├── .gitignore
└── README.md
```

---

## 🧪 Testing

The domain, persistence, and business layers are validated by an assert-based unit-test suite
(`tests/tests.cpp`), achieving **100 % line coverage** on those layers (measured with
[OpenCppCoverage](https://github.com/OpenCppCoverage/OpenCppCoverage)). The only lines excluded
from coverage are the two `ShellExecuteA` OS-shell calls used to open the exported watchlist file
in Explorer — launching a real shell window isn't something an automated test should trigger.

Run the suite without launching the GUI:
```bash
MovieDatabase.exe --run-tests
```

To reproduce the coverage report:
```bash
OpenCppCoverage.exe ^
  --sources MovieDatabase\domain --sources MovieDatabase\persistence --sources MovieDatabase\business ^
  --excluded_sources MovieDatabase\ui ^
  --excluded_line_regex ".*ShellExecuteA.*|.*watchList->displayList\(\).*" ^
  --export_type html:coverage_report ^
  -- MovieDatabase.exe --run-tests
```

---

## 📜 License

This project was created for educational purposes as part of a university course.
