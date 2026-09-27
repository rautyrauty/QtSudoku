%define _unpackaged_files_terminate_build 1

Name: qtsudoku
Version: 0.0.1
Release: alt1

Summary: Sudoku puzzle game written in Qt Quick
License: MIT and OFL-1.1
Group: Games/Puzzles
Url: https://github.com/rautyrauty/QtSudoku
Vcs: https://github.com/rautyrauty/QtSudoku.git

Source: %name-%version.tar

BuildRequires: cmake
BuildRequires: gcc-c++
BuildRequires: qt6-base-devel
BuildRequires: qt6-declarative-devel

%description
QtSudoku is a Sudoku puzzle game with a Qt Quick interface. It generates
puzzles with a configurable number of open cells, checks the solution,
hints at the first wrong cell and can solve the board.

%prep
%setup

%build
%cmake
%cmake_build

%install
%cmake_install

%files
%doc README.md LICENSE
%_bindir/qtsudoku
%_iconsdir/hicolor/*/apps/qtsudoku.png
%_desktopdir/qtsudoku.desktop

%changelog
* Sun Sep 27 2026 Ajrat Makhmutov <rauty@altlinux.org> 0.0.1-alt1
- Initial build for ALT Linux.
