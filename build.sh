# I still don't understand Make :( so I use this garbage...

name="vroemvroem"
version="0.1.0"
platform="x86_64"

rm -f -r .vscode

if [ "$1" == "clean" ]; then
    rm -r target

elif [ "$1" == "release" ]; then
    mkdir -p target/release

    for file in $(find src -name "*.c"); do
        folder=$(dirname ${file:4:-2})
        if [[ "$folder" != "." ]]; then
            mkdir -p "target/release/$folder"
        fi

        if gcc -Os -Iinclude -c $file -o "target/release/${file:4:-2}.o"; then
            echo "Passed: ${file:4}"
        else
            echo "Failed: ${file:4}"
            exit
        fi
    done

    for file in $(find src -name "*.cpp"); do
        folder=$(dirname ${file:4:-4})
        if [[ "$folder" != "." ]]; then
            mkdir -p "target/release/$folder"
        fi

        if g++ -Os -Iinclude -c $file -o "target/release/${file:4:-4}.o"; then
            echo "Passed: ${file:4}"
        else
            echo "Failed: ${file:4}"
            exit
        fi
    done

    if [ "$(uname -s)" == "Linux" ]; then
        rm -f "target/release/$name-v$version-$platform"

        g++ -s $(find target/release -name *.o) -lSDL2 -o "target/release/$name-v$version-$platform"

        find target/release -not -name "$name-v$version-$platform" -delete
    else
        rm -f "target/release/$name-v$version-$platform.exe"

        g++ -s $(find target/release -name *.o) -static -lsdl2 -lgdi32 -lversion -lsetupapi -lwinmm -limm32 \
            -lole32 -loleaut32 -Wl,--subsystem,windows -o "target/release/$name-v$version-$platform.exe"

        find target/release -not -name "$name-v$version-$platform.exe" -delete
    fi

    rm -f -r target/release/assets
    cp -r assets target/release

else
    clear

    mkdir -p target/debug

    for file in $(find src -name "*.c"); do
        object="target/debug/${file:4:-2}.o"

        folder=$(dirname ${file:4:-2})
        if [[ "$folder" != "." ]]; then
            mkdir -p "target/debug/$folder"
        fi

        if [[ $file -nt $object ]]; then
            rm -f $object
            if gcc -Iinclude -c $file -o $object; then
                echo "Passed: ${file:4}"
            else
                echo "Failed: ${file:4}"
                exit
            fi
        fi
    done

    for file in $(find src -name "*.cpp"); do
        object="target/debug/${file:4:-4}.o"

        folder=$(dirname ${file:4:-4})
        if [[ "$folder" != "." ]]; then
            mkdir -p "target/debug/$folder"
        fi

        if [[ $file -nt $object ]]; then
            rm -f $object
            if g++ -g -DDEBUG -Wall -Wextra -Wpedantic --std=c++17 -Iinclude -c $file -o $object; then
                echo "Passed: ${file:4}"
            else
                echo "Failed: ${file:4}"
                exit
            fi
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
