# ItemsDat_Decoder
Decoder for items.dat file which can be found in Growtopia game.

## Description
The "ItemsDat_Decoder" project is a tool that can be used to decode the items.dat file which can be found in the Growtopia game. This file contains information about the items that exist in the Growtopia game.

## TODO List
- Write all data in the item vector to a file to see the decoding results
- Find out the meaning of some unknown data

## Building
This project has been tested for building using clang version 15.0.7 and works perfectly (compilers other than this have not been tested).

### Building Instructions
Enter the following command in the console to build this project:

```
g++ ItemsDat_Decoder -o decoder -std=c++17
```

And run the program using the following command:
```
./decoder ${PATH_TO_ITEMS_DAT_FILE}
```

this project is currently supporting items.dat version 15. (items.dat file with higher version may not supported)
