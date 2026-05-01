Name:           steamdeckmediaplayer
Version:        1.0.0
Release:        0
Summary:        Controller-friendly local media player for Steam Deck and Linux handhelds
License:        MIT
URL:            https://github.com/zeclorn/steamdeckmediaplayer
Source0:        %{name}-%{version}.tar.gz

BuildRequires:  cmake
BuildRequires:  gcc-c++
BuildRequires:  pkgconfig
BuildRequires:  pkgconfig(Qt6Core)
BuildRequires:  pkgconfig(Qt6Gui)
BuildRequires:  pkgconfig(Qt6Qml)
BuildRequires:  pkgconfig(Qt6Quick)
BuildRequires:  pkgconfig(mpv)
BuildRequires:  pkgconfig(sdl2)

%description
Steam Deck Media Player is a local-first media browser and player aimed at
Steam Deck Game Mode and KDE desktop use. It uses Qt 6, QML, and libmpv for a
controller-friendly local playback experience.

%prep
%autosetup -p1

%build
%cmake -DCMAKE_BUILD_TYPE=Release
%cmake_build

%install
%cmake_install

%files
%license LICENSE
%doc README.md
%{_bindir}/steamdeckmediaplayer
%{_datadir}/applications/steamdeckmediaplayer.desktop
%{_datadir}/icons/hicolor/scalable/apps/steamdeckmediaplayer.svg

%changelog
