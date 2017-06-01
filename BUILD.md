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