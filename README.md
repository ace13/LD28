Run 'n Gun
==========

For Ludum Dare #28 - You only get one.


Building
--------

This game requires CMake, Boost, and SFML (SFML 2.0 should be enough, though I myself use the latest Git)
It also uses many C++11 features so a reasonably new compiler might be good too, VS 2010 and GCC 4.3 for example.

Because of a small CMake file issue you need to manually set Kunlaboro_BOOST, otherwise it won't build. I might fix this before the end of Ludum Dare but don't expect it.

Running
-------

All the files in the resources folder are needed for proper gameplay, put them next to the executable you build.
