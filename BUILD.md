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
```
sudo apt-get install debhelper
debuild -uc -us
```

deb-пакет будет в каталоге уровнем выше с названием `ctool1cd_<версия>_<платформа>.deb`

## Windows

Установка системы генерации проектов:
https://cmake.org/download/

### IDE Visual Studio 2017

Установка зависимостей:

Скачать и установить скомпилированные библиотеки `boost x86`
https://sourceforge.net/projects/boost/files/boost-binaries/1.63.0/boost_1_63_0-msvc-14.0-32.exe/download

В качестве каталога назначения указать, например `C:\Libraries\boost_1_63_0`

Скачать библиотеку `zlib` и скомпилировать
https://github.com/madler/zlib/archive/v1.2.8.zip

Распаковать архив, например в каталог `C:\Libs\zlib-1.2.8`

```cmd
cd zlib-1.2.8

md build

cd build

cmake -DCMAKE_SHARED_LINKER_FLAGS="/NODEFAULTLIB:MSVCRTD" -DCMAKE_C_FLAGS_DEBUG="/MT /Od /Ob0" ..

cmake --build . --config Debug --target zlibstatic

copy zconf.h ..
```

Генерация проекта:

Visual Studio 2017 поддерживает работу с cmake без предварительной генерации проекта

Открыть каталог с корневым `CMakeLists.txt` через меню `File -> Open -> CMake...`

Для настройки зависимостей и типов проектов `Debug, Release`, необходимо 
через меню `Cmake -> Change CMake Settings -> CMakeLists.txt` создать файл `CMakeSettings.json`

```json
{
  // See https://go.microsoft.com//fwlink//?linkid=834763 for more information about this file.
  "configurations": [
    {
      "name": "x86-Debug",
      "generator": "Ninja",
      "configurationType": "Debug",
      "inheritEnvironments": [ "msvc_x86" ],
      "buildRoot": "${env.USERPROFILE}\\CMakeBuilds\\${workspaceHash}\\build\\${name}",
      "installRoot": "${env.USERPROFILE}\\CMakeBuilds\\${workspaceHash}\\install\\${name}",
      "cmakeCommandArgs": "",
      "buildCommandArgs": "-v",
      "ctestCommandArgs": ""
    }
  ]
}
```

Заменить переменные:

`"generator": "Visual Studio 14 2015",` - для какой IDE создавать проект

`"buildRoot": "${projectDir}\\build\\${name}",` - каталог, где будут хранится сгенерированные проекты `${projectDir}` - путь до корневого `CMakeLists.txt`

`"installRoot": "${projectDir}\\install\\${name}",`

`"buildCommandArgs": "-v:minimal",`

Добавить переменные cmake:

```json
"variables": [
        {
          "name": "BOOST_ROOT",
          "value": "C:\\Libs\\boost_1_63_0"
        },
        {
          "name": "BOOST_LIBRARYDIR",
          "value": "C:\\Libs\\boost_1_63_0\\lib32-msvc-14.0"
        },
        {
          "name": "BOOST_INCLUDEDIR",
          "value": "C:\\Libs\\boost_1_63_0"
        },
        {
          "name": "ZLIB_INCLUDE_DIR",
          "value": "C:\\Libs\\zlib-1.2.8"
        },
        {
          "name": "ZLIB_LIBRARY_DEBUG",
          "value": "C:\\Libs\\zlib-1.2.8\\build\\Debug\\zlibstatic.lib"
        }
]
```

В итоге должнен получится файл вида:

```json
{
  // See https://go.microsoft.com//fwlink//?linkid=834763 for more information about this file.
  "configurations": [
    {
      "name": "x86-Debug",
      "generator": "Visual Studio 14 2015",
      "configurationType": "Debug",
      "inheritEnvironments": [ "msvc_x86" ],
     "buildRoot": "${projectDir}\\build\\${name}",
      "installRoot": "${projectDir}\\install\\${name}",
      "cmakeCommandArgs": "",
      "buildCommandArgs": "-v:minimal",
      "ctestCommandArgs": "",
      "variables": [
        {
          "name": "BOOST_ROOT",
          "value": "C:\\Libs\\boost_1_63_0"
        },
        {
          "name": "BOOST_LIBRARYDIR",
          "value": "C:\\Libs\\boost_1_63_0\\lib32-msvc-14.0"
        },
        {
          "name": "BOOST_INCLUDEDIR",
          "value": "C:\\Libs\\boost_1_63_0"
        },
        {
          "name": "ZLIB_INCLUDE_DIR",
          "value": "C:\\Libs\\zlib-1.2.8"
        },
        {
          "name": "ZLIB_LIBRARY_DEBUG",
          "value": "C:\\Libs\\zlib-1.2.8\\build\\Debug\\zlibstatic.lib"
        }
      ]
    }
  ]
}
```

В каталоге `buildRoot` будет создан Solution `tool1cd.sln`, который можно использовать для разработки.