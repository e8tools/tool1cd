# Сборка Tool1CD

Проект поддерживает CMake 2.8 и выше. Необходим компилятор C++ с поддержкой C++11.

Необходимые библиотеки:
  * Boost.FileSystem (>= 1.53)
  * Boost.Regex (>= 1.53)
  * zlib (>= 1.2.8)

## Ubuntu

```sh
# Установка системы сборки:
sudo apt-get install cmake make g++ libstdc++-dev

# Установка зависимостей:
sudo apt-get install libboost-filesytem-dev libboost-system-dev libboost-regex-dev zlib1g-dev

# Сборка:
mkdir build && cd build
cmake .. && make
make install # или checkinstall
```

Сборка deb-пакета:

```sh
sudo apt-get install debhelper
debuild -uc -us
```

Поддержка разработки графического интерфейса пользователя.

```sh
sudo apt install mesa-common-dev
```

Скачать и установить скомпилированные библиотеки [Qt](https://www.qt.io/download)

deb-пакет будет в каталоге уровнем выше с названием `ctool1cd_<версия>_<платформа>.deb`

## Windows

Установить систему генерации проектов:
https://cmake.org/download/

### Сборка консольного проекта в Visual Studio 2017

Установка зависимостей:

Скачать и установить скомпилированные библиотеки `boost x86`
https://sourceforge.net/projects/boost/files/boost-binaries/1.66.0/boost_1_66_0-msvc-14.1-32.exe/download

В качестве каталога назначения указать, например `C:\Libs\boost_1_66_0`

Скачать библиотеку `zlib` и скомпилировать
https://github.com/madler/zlib/archive/v1.2.8.zip

Распаковать архив, например в каталог `C:\Libs\zlib-1.2.8`

Открыть каталог с *корневым* `CMakeLists.txt` через меню `File -> Open -> CMake...`
Для настройки зависимостей и типов проектов `Debug, Release`, необходимо 
через меню `Cmake -> Change CMake Settings -> CMakeLists.txt` создать файл `CMakeSettings.json`

Замените переменные в секции `x86-Debug`:

`"generator": "Visual Studio 15 2017",` - для какой IDE создавать проект

`"buildRoot": "${projectDir}\\build\\${name}",` - каталог, где будут хранится сгенерированные проекты `${projectDir}` - путь до корневого `CMakeLists.txt`

`"installRoot": "${projectDir}\\install\\${name}",`

`"buildCommandArgs": "-v:minimal",`

Должен получиться такой файл:

```json
 "configurations": [
    {
      "name": "x86-Debug",
      "generator": "Visual Studio 15 2017",
      "configurationType": "Debug",
      "inheritEnvironments": [ "msvc_x86" ],
      "buildRoot": "${projectDir}\\build\\${name}",
      "installRoot": "${projectDir}\\install\\${name}",
      "cmakeCommandArgs": "",
      "buildCommandArgs": "-v:minimal",
      "ctestCommandArgs": ""
    }
  ]
}
```

Через меню `Cmake -> Only build` выбрать цель `zlibstaticd.lib`
Артифакты сборки будут находится подобном каталоге `C:\libs\zlib-1.2.8\build\x86-Debug\Debug`
После сборки необходимо скопировать файл `zconf.h` в каталог `C:\Libs\zlib-1.2.8`

Генерация консольного проекта:

Visual Studio 2017 поддерживает работу с cmake без предварительной генерации проекта

Открыть каталог с *корневым* `CMakeLists.txt` через меню `File -> Open -> CMake...`

Для настройки зависимостей и типов проектов `Debug, Release`, необходимо 
через меню `Cmake -> Change CMake Settings -> CMakeLists.txt` создать файл `CMakeSettings.json`

Замените переменные:

`"generator": "Visual Studio 15 2017",` - для какой IDE создавать проект

`"buildRoot": "${projectDir}\\build\\${name}",` - каталог, где будут хранится сгенерированные проекты `${projectDir}` - путь до корневого `CMakeLists.txt`

`"installRoot": "${projectDir}\\install\\${name}",`

`"buildCommandArgs": "-v:minimal",`

Добавить переменные cmake:

```json
 "variables": [
        {
          "name": "Boost_FILESYSTEM_LIBRARY_DEBUG",
          "value": "C:\\libs\\boost_1_66_0\\lib32-msvc-14.1\\boost_filesystem-vc141-mt-gd-x32-1_66.lib"
        },
        {
          "name": "Boost_SYSTEM_LIBRARY_DEBUG",
          "value": "C:\\libs\\boost_1_66_0\\lib32-msvc-14.1\\boost_system-vc141-mt-gd-x32-1_66.lib"
        },
        {
          "name": "Boost_REGEX_LIBRARY_DEBUG",
          "value": "C:\\libs\\boost_1_66_0\\lib32-msvc-14.1\\boost_regex-vc141-mt-gd-x32-1_66.lib"
        },
        {
          "name": "NOGUI",
          "value": "true"
        },
        {
          "name": "BOOST_ROOT",
          "value": "C:\\libs\\boost_1_66_0"
        },
        {
          "name": "BOOST_LIBRARYDIR",
          "value": "C:\\libs\\boost_1_66_0\\lib32-msvc-14.1"
        },
        {
          "name": "BOOST_INCLUDEDIR",
          "value": "C:\\libs\\boost_1_66_0"
        },
        {
          "name": "ZLIB_LIBRARY_DEBUG",
          "value": "C:\\libs\\zlib-1.2.8\\build\\x86-Debug\\Debug\\zlibstaticd.lib"
        },
        {
          "name": "ZLIB_INCLUDE_DIR",
          "value": "C:\\libs\\zlib-1.2.8"
        }
      ]
```

### Сборка проекта с поддержкой GUI в Visual Studio 2017

Установка зависимостей:
Скачать и установить скомпилированные библиотеки [Qt](https://www.qt.io/download).
Необходимо установить комплект библиотек собранных с помощью `msvc2017_64`

Скачать и установить скомпилированные библиотеки `boost x64`
https://sourceforge.net/projects/boost/files/boost-binaries/1.66.0/boost_1_66_0-msvc-14.1-64.exe/download

В качестве каталога назначения указать, например `C:\Libs\boost_1_66_0`.

Скачать библиотеку `zlib` и скомпилировать
https://github.com/madler/zlib/archive/v1.2.8.zip

Распаковать архив, например в каталог `C:\Libs\zlib-1.2.8`

Открыть каталог с *корневым* `CMakeLists.txt` через меню `File -> Open -> CMake...`
Для настройки зависимостей и типов проектов `Debug, Release`, необходимо 
через меню `Cmake -> Change CMake Settings -> CMakeLists.txt` создать файл `CMakeSettings.json`

Замените переменные в секции `x64-Debug`:

`"generator": "Visual Studio 15 2017 Win64",` - для какой IDE создавать проект

`"buildRoot": "${projectDir}\\build\\${name}",` - каталог, где будут хранится сгенерированные проекты `${projectDir}` - путь до корневого `CMakeLists.txt`

`"installRoot": "${projectDir}\\install\\${name}",`

`"buildCommandArgs": "-v:minimal",`

Должен получиться такой файл:

```json
 "configurations": [
    {
      "name": "x64-Debug",
      "generator": "Visual Studio 15 2017 Win64",
      "configurationType": "Debug",
      "inheritEnvironments": [ "msvc_x64" ],
      "buildRoot": "${projectDir}\\build\\${name}",
      "installRoot": "${projectDir}\\install\\${name}",
      "cmakeCommandArgs": "",
      "buildCommandArgs": "-v:minimal",
      "ctestCommandArgs": ""
    }
  ]
}
```

Через меню `Cmake -> Only build` выбрать цель `zlibstaticd.lib`
Артифакты сборки будут находится подобном каталоге `C:\libs\zlib-1.2.8\build\x64-Debug\Debug`
После сборки необходимо скопировать файл `zconf.h` в каталог `C:\Libs\zlib-1.2.8`

Генерация проектов (консольного, GUI):

Visual Studio 2017 поддерживает работу с cmake без предварительной генерации проекта

Открыть каталог с *корневым* `CMakeLists.txt` через меню `File -> Open -> CMake...`

Для настройки зависимостей и типов проектов `Debug, Release`, необходимо 
через меню `Cmake -> Change CMake Settings -> CMakeLists.txt` создать файл `CMakeSettings.json`

Заменить переменные в секции `x64-Debug`:

`"generator": "Visual Studio 15 2017 Win64",` - для какой IDE создавать проект

`"buildRoot": "${projectDir}\\build\\${name}",` - каталог, где будут хранится сгенерированные проекты `${projectDir}` - путь до корневого `CMakeLists.txt`

`"installRoot": "${projectDir}\\install\\${name}",`

`"buildCommandArgs": "-v:minimal",`

Добавить переменные cmake:

```json
 "variables": [
        {
          "name": "Boost_FILESYSTEM_LIBRARY_DEBUG",
          "value": "C:\\libs\\boost_1_66_0\\lib64-msvc-14.1\\boost_filesystem-vc141-mt-gd-x64-1_66.lib"
        },
        {
          "name": "Boost_SYSTEM_LIBRARY_DEBUG",
          "value": "C:\\libs\\boost_1_66_0\\lib64-msvc-14.1\\boost_system-vc141-mt-gd-x64-1_66.lib"
        },
        {
          "name": "Boost_REGEX_LIBRARY_DEBUG",
          "value": "C:\\libs\\boost_1_66_0\\lib64-msvc-14.1\\boost_regex-vc141-mt-gd-x64-1_66.lib"
        },
        {
          "name": "NOGUI",
          "value": "false"
        },
        {
          "name": "BOOST_ROOT",
          "value": "C:\\libs\\boost_1_66_0"
        },
        {
          "name": "BOOST_LIBRARYDIR",
          "value": "C:\\libs\\boost_1_66_0\\lib64-msvc-14.1"
        },
        {
          "name": "BOOST_INCLUDEDIR",
          "value": "C:\\libs\\boost_1_66_0"
        },
        {
          "name": "ZLIB_LIBRARY_DEBUG",
          "value": "C:\\libs\\zlib-1.2.8\\build\\x64-Debug\\Debug\\zlibstaticd.lib"
        },
        {
          "name": "ZLIB_INCLUDE_DIR",
          "value": "C:\\libs\\zlib-1.2.8"
        },
        {
          "name": "Qt5Widgets_DIR",
          "value": "C:\\Qt\\5.9.5\\msvc2017_64\\lib\\cmake\\Qt5Widgets"
        }
      ]
```
