# home-management-client

## Compilation
Depending on your Qt installation, you should run : 
- Installation from public repos : 
    ``` bash
    cmake -S -B build
    cmake --build build
    ```
- Installation from Qt online installer : 
    ``` bash
    CMAKE_PREFIX_PATH=<path_to_Qt_install> cmake -S . -B build
    cmake --build build
    ```
    In my case : `<path_to_Qt_install>=/opt/Qt/6.7.0/gcc_64/lib/cmake`

In the second case, you can also fill up a `PreLoad.cmake` file with the following line `set(CMAKE_PREFIX_PATH "<path_to_Qt_install>" CACHE INTERNAL "" FORCE)`, to automatically set the cache variable for cmake, and thus using the same commands as the first case.