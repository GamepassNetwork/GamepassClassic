VERSION=0.1.0.0
rm -rf ./release-linux
mkdir release-linux

cp ./src/gamepassd ./release-linux/
cp ./src/gamepass-cli ./release-linux/
cp ./src/qt/gamepass-qt ./release-linux/
cp ./GAMEPASS_small.png ./release-linux/

cd ./release-linux/
strip gamepassd
strip gamepass-cli
strip gamepass-qt

#==========================================================
# prepare for packaging deb file.

mkdir GAMEPASS-$VERSION
cd GAMEPASS-$VERSION
mkdir -p DEBIAN
echo 'Package: GAMEPASS
Version: '$VERSION'
Section: base 
Priority: optional 
Architecture: all 
Depends:
Maintainer: Gamepass
Description: Gamepass coin wallet and service.
' > ./DEBIAN/control
mkdir -p ./usr/local/bin/
cp ../gamepassd ./usr/local/bin/
cp ../gamepass-cli ./usr/local/bin/
cp ../gamepass-qt ./usr/local/bin/

# prepare for desktop shortcut
mkdir -p ./usr/share/icons/
cp ../GAMEPASS_small.png ./usr/share/icons/
mkdir -p ./usr/share/applications/
echo '
#!/usr/bin/env xdg-open

[Desktop Entry]
Version=1.0
Type=Application
Terminal=false
Exec=/usr/local/bin/gamepass-qt
Name=GAMEPASS
Comment= gamepass coin wallet
Icon=/usr/share/icons/GAMEPASS_small.png
' > ./usr/share/applications/GAMEPASS.desktop

cd ../
# build deb file.
dpkg-deb --build GAMEPASS-$VERSION

#==========================================================
# build rpm package
rm -rf ~/rpmbuild/
mkdir -p ~/rpmbuild/{RPMS,SRPMS,BUILD,SOURCES,SPECS,tmp}

cat <<EOF >~/.rpmmacros
%_topdir   %(echo $HOME)/rpmbuild
%_tmppath  %{_topdir}/tmp
EOF

#prepare for build rpm package.
rm -rf GAMEPASS-$VERSION
mkdir GAMEPASS-$VERSION
cd GAMEPASS-$VERSION

mkdir -p ./usr/bin/
cp ../gamepassd ./usr/bin/
cp ../gamepass-cli ./usr/bin/
cp ../gamepass-qt ./usr/bin/

# prepare for desktop shortcut
mkdir -p ./usr/share/icons/
cp ../GAMEPASS_small.png ./usr/share/icons/
mkdir -p ./usr/share/applications/
echo '
[Desktop Entry]
Version=1.0
Type=Application
Terminal=false
Exec=/usr/bin/gamepass-qt
Name=GAMEPASS
Comment= gamepass coin wallet
Icon=/usr/share/icons/GAMEPASS_small.png
' > ./usr/share/applications/GAMEPASS.desktop
cd ../

# make tar ball to source folder.
tar -zcvf GAMEPASS-$VERSION.tar.gz ./GAMEPASS-$VERSION
cp GAMEPASS-$VERSION.tar.gz ~/rpmbuild/SOURCES/

# build rpm package.
cd ~/rpmbuild

cat <<EOF > SPECS/GAMEPASS.spec
# Don't try fancy stuff like debuginfo, which is useless on binary-only
# packages. Don't strip binary too
# Be sure buildpolicy set to do nothing

Summary: Gamepass wallet rpm package
Name: GAMEPASS
Version: $VERSION
Release: 1
License: MIT
SOURCE0 : %{name}-%{version}.tar.gz
URL: https://www.gamepass.network/

BuildRoot: %{_tmppath}/%{name}-%{version}-%{release}-root

%description
%{summary}

%prep
%setup -q

%build
# Empty section.

%install
rm -rf %{buildroot}
mkdir -p  %{buildroot}

# in builddir
cp -a * %{buildroot}


%clean
rm -rf %{buildroot}


%files
/usr/share/applications/GAMEPASS.desktop
/usr/share/icons/GAMEPASS_small.png
%defattr(-,root,root,-)
%{_bindir}/*

%changelog
* Tue Aug 24 2022  Gamepass Project Team.
- First Build

EOF

rpmbuild -ba SPECS/GAMEPASS.spec



