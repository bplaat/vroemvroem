# I still don't understand Make :( so I use this garbage...
# This build script is written for Windows and the mingw suite
# Currently the builds are not portable because they link to glfw and gcc c++ libs I think :(

name="vroemvroem"
version="0.1.0"

if [ "$1" == "clean" ]; then
    rm -r target

elif [ "$1" == "release" ]; then
    mkdir -p target/release

    for file in src/*.c; do
        gcc -Os $release_flags -Iinclude -c $file -o target/release/$(basename $file .c).o
    done

    # Can't optimize black texture loading bug :(
    for file in src/*.cpp; do
        g++ -Iinclude -c $file -o target/release/$(basename $file .cpp).o
    done

    rm -f target/release/$name-v$version-x86_64.exe

    g++ -s $(find target/release -name *.o) -lglfw3 -Wl,--subsystem,windows -o target/release/$name-v$version-x86_64.exe

    rm -r target/release/*.o

    rm -f -r target/release/assets
    cp -r assets target/release

else
    mkdir -p target/debug

    for file in src/*.c; do
        object="target/debug/$(basename $file .c).o"
        if [[ $file -nt $object ]]; then
            gcc -Iinclude -c $file -o $object
        fi
    done

    for file in src/*.cpp; do
        object="target/debug/$(basename $file .cpp).o"
        if [[ $file -nt $object ]]; then
            g++ -DDEBUG -Wall -Wextra -Wpedantic --std=c++11 -Iinclude -c $file -o $object
        fi
    done

    rm -f target/debug/$name-v$version-x86_64-debug.exe

    g++ $(find target/debug -name *.o) -lglfw3 -o target/debug/$name-v$version-x86_64-debug.exe

    ./target/debug/$name-v$version-x86_64-debug.exe
fi
