
Debian
====================
This directory contains files used to package gamepassd/gamepass-qt
for Debian-based Linux systems. If you compile gamepassd/gamepass-qt yourself, there are some useful files here.

## gamepass: URI support ##


gamepass-qt.desktop  (Gnome / Open Desktop)
To install:

	sudo desktop-file-install gamepass-qt.desktop
	sudo update-desktop-database

If you build yourself, you will either need to modify the paths in
the .desktop file or copy or symlink your gamepass-qt binary to `/usr/bin`
and the `../../share/pixmaps/gamepass128.png` to `/usr/share/pixmaps`

gamepass-qt.protocol (KDE)

