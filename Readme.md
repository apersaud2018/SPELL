# Singing Phoneme Ergonomic LabeLer (SPELL)
Yes the acronym is a stretch but it's better than Singing Data Labeling Tool (SDLT)
The goal of this tool is to make labeling phonemes, and various other singing related features, easy and painless.

# Required Tools/Libraries
- QT 6
- Cmake 3.17+
- ...

# Compile
## Windows
### Configure
```bash
mkdir build
cd build
cmake -GNinja -DCMAKE_C_COMPILER=C:/Qt/Tools/mingw810_64/bin/gcc.exe -DCMAKE_CXX_COMPILER=C:/Qt/Tools/mingw810_64/bin/g++.exe "-DCMAKE_PREFIX_PATH:STRING=C:/Qt/6.1.2/mingw81_64" ..
```
Modify paths as needed
### Build
```bash
cmake --build .
```

## Linux
Idk yet, didn't test it
Ask me later
probably just ``mkdir build && cd build && cmake .. && cmake --build .``

## Mac OSX
None of us own a Mac, we'll figure it out later

## Todo
- Finish and spell check the readme
