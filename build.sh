# I still don't understand Make :( so I use this garbage...
# This build script is written for Windows and the mingw suite
# Currently the builds are not portable because they link to glfw and gcc c++ libs I think :(

name="vroemvroem"
version="0.1.0"
release_flags="-s -Os -Wl,--subsystem,windows"
debug_flags="-DDEBUG -Wall -Wextra -Wpedantic --std=c++11"
sources="$(find src -name *.cpp) src/glad.c"
libs="-lglfw3"

if [ "$1" == "clean" ]; then
    rm -r target

elif [ "$1" == "release" ]; then
    mkdir -p target/release
    rm -f target/release/$name-v$version-x86_64.exe
    g++ $release_flags $sources $libs -o target/release/$name-v$version-x86_64.exe

else
    mkdir -p target/debug
    rm -f target/debug/$name-v$version-x86_64-debug.exe
    g++ $debug_flags $sources $libs -o target/debug/$name-v$version-x86_64-debug.exe
    ./target/debug/$name-v$version-x86_64-debug.exe
fi
