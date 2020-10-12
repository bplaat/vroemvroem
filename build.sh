# I still don't understand Make :( so I use this garbage...

name="vroemvroem"
version="0.1.0"
platform="x86_64"

if [ "$1" == "clean" ]; then
    rm -r target

elif [ "$1" == "release" ]; then
    mkdir -p target/release

    for file in src/*.c; do
        gcc -Os $release_flags -Iinclude -c $file -o target/release/$(basename $file .c).o
    done

    for file in src/*.cpp; do
        g++ -Os -Iinclude -c $file -o target/release/$(basename $file .cpp).o
    done

    if [ "$(uname -s)" == "Linux" ]; then
        rm -f target/release/$name-v$version-$platform

        g++ -s $(find target/release -name *.o) -lSDL2 -o target/release/$name-v$version-$platform
    else
        rm -f target/release/$name-v$version-$platform.exe

        g++ -s $(find target/release -name *.o) -static -lsdl2 -lgdi32 -lversion -lsetupapi -lwinmm -limm32 -lole32 -loleaut32 -Wl,--subsystem,windows -o target/release/$name-v$version-$platform.exe
    fi

    rm -r target/release/*.o

    rm -f -r target/release/assets
    cp -r assets target/release

else
    clear

    mkdir -p target/debug

    for file in src/*.c; do
        object="target/debug/$(basename $file .c).o"
        if [[ $file -nt $object ]]; then
            rm -f $object
            gcc -Iinclude -c $file -o $object
        fi
    done

    for file in src/*.cpp; do
        object="target/debug/$(basename $file .cpp).o"
        if [[ $file -nt $object ]]; then
            rm -f $object
            g++ -g -DDEBUG -Wall -Wextra -Wpedantic --std=c++17 -Iinclude -c $file -o $object
        fi
    done

    if [ "$(uname -s)" == "Linux" ]; then
        rm -f target/debug/$name-v$version-$platform-debug

        g++ $(find target/debug -name *.o) -lSDL2 -o target/debug/$name-v$version-$platform-debug

        if [ "$1" == "debug" ]; then
            gdb ./target/debug/$name-v$version-$platform-debug
        elif [ "$1" == "mem" ]; then
            valgrind --suppressions=./linux_sdl_gl.sup --leak-check=full --show-leak-kinds=all --log-file=target/debug/valgrind.log ./target/debug/$name-v$version-$platform-debug
        else
            ./target/debug/$name-v$version-$platform-debug
        fi
    else
        rm -f target/debug/$name-v$version-$platform-debug.exe

        g++ $(find target/debug -name *.o) -lsdl2 -o target/debug/$name-v$version-$platform-debug.exe

        if [ "$1" == "debug" ]; then
            gdb ./target/debug/$name-v$version-$platform-debug.exe
        elif [ "$1" == "mem" ]; then
            drmemory -- ./target/debug/$name-v$version-$platform-debug.exe
        else
            ./target/debug/$name-v$version-$platform-debug.exe
        fi
    fi
fi
