# KDE-Desktop-Effect-Template
A template designed for creating KWin Desktop Effects compatible with KDE Plasma 6.

TODO: add config module

TODO: Add instructions on customization to README

# How to build from source code:

You need to install development packages for your distribution first:

<details>
<summary>OpenSUSE Tumbleweed</summary>

```
sudo zypper in git cmake gcc-c++ kf6-kconfigwidgets-devel kf6-kcmutils-devel kwin6-devel kf6-kwindowsystem-devel qt6-quick-devel qt6-core-private-devel kf6-kglobalaccel-devel
```

</details>

After installing the required development packages, you can clone the source code and compile it as follows:

```bash
git clone https://github.com/aaronkirschen/KDE-Desktop-Effect-Template
cd KDE-Desktop-Effect-Template
mkdir build
cd build
cmake .. --install-prefix /usr
make
sudo make install
```

# Credits

This project utilizes code from the following sources:

- [KDE-Rounded-Corners](https://github.com/matinlotfali/KDE-Rounded-Corners) by [matinlotfali](https://github.com/matinlotfali) - CMake code, README
- [Invert Effect](https://github.com/KDE/kwin/tree/master/src/plugins/invert) by [KDE](https://kde.org/)