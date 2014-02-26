# About the Project
This library (mainly header files) resulted from needs I commonly had (and will have) with my C++ projects. The library will contain a growing set of functions for often needed tasks like file operations, streams to add transparent compression/decompression layers, etc.

Currently, the library is a dependency of my project **KSynth** (https://github.com/k-a-z-u/KSynth), provides file-system options and streaming IO.

Unfortunately im in quite a dilemma. Using only header files, it is very convenient to include the library within other projects. While using only header files is perfectly fine for template classes or small things without many includes, large components with many includes would suffer.
I'm not a fan of unnecessary complex building routines e.g. for creating a dynamic library, but adding the needed .cpp files to your projects build-script isn't nice either. Maybe someone has a nice hint here? ;)

# Test-Cases
Currently the library contains a bunch of test-cases to ensure proper operation using **gtest**. The provided CMakeLists.txt can build those test-cases for you to ensure the library works.

The best way to configure is as follows:

```
# create build folder beside the KLib folder
mkdir KLib_build

# change to build folder
cd KLib_build

# run cmake for KLib
cmake ../KLib

# build
make
```

an alternative to **cmake** is to use **ccmake** which shows all available build options using a command-line GUI to e.g. enabled **SSL** (still under development), **zlib**, etc.


# External dependencies
External dependencies are only necessary for optional modules. The library itself does currently not require any dependencies.
* gtest
 * to run the test-cases
* openSSL
 * for sockets with SSL support
* zlib
 * for gzip compressed input/output streams

Some other libraries as **tinyxml2** and **7z** are currently included inside the project, to ensure an easy build even when using windows or other platforms. Maybe there will be some option in the furture, to use the system's libraries instead.


# License
Apache License Version 2.0
http://apache.org/licenses/LICENSE-2.0.txt
