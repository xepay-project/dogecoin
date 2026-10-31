
Debian
====================
This directory contains files used to package litonpayd/litonpay-qt
for Debian-based Linux systems. If you compile litonpayd/litonpay-qt yourself, there are some useful files here.

## litonpay: URI support ##


litonpay-qt.desktop  (Gnome / Open Desktop)
To install:

	sudo desktop-file-install litonpay-qt.desktop
	sudo update-desktop-database

If you build yourself, you will either need to modify the paths in
the .desktop file or copy or symlink your litonpay-qt binary to `/usr/bin`
and the `../../share/pixmaps/litonpay128.png` to `/usr/share/pixmaps`

litonpay-qt.protocol (KDE)

