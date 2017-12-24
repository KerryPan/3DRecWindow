Authors : 
[main author] 		Yasutaka Furukawa furukawa[AT]cs.washington.edu
[main author] 		Jean Ponce Jean.Ponce[AT]ens.fr
[Windows Porting] 	Pierre moulon pmoulon[AT]gmail.com

Special thanks to ASTRE Henri for the PTHREAD 64 bits lib and dll : http://www.visual-experiments.com/

Date : 24 Sept 2010

--------------------
- Web ressources : - 
--------------------
[CMVS] http://grail.cs.washington.edu/software/cmvs/
[windows porting] http://francemapping.free.fr/Portfolio/Prog3D/CMVS.html


--------------------
-- Compilation  --
--------------------

Windows => Use precompiled binary, or compile it with VS2008/2010 (Express or pro, Pro will allow you to enable Opemp in CMVS)
        => Use CMake GUI in order to generate the Visual Studio project file (in ./program you will find the main CMakeLists.txt).
        => To compile windows 64 binary => edit the ./program/CMakeLists.txt and set WIN64_PTHREAD to true

Linux => use makefile in program/main.
Setup in makefile the library path to clapack directory if necessary (in order find clapack/clapack.h, clapack/f2c.h, and graclus at compile time)

--------------------
----  Notes :   ----
--------------------
To test it : ./WinX-VS2010/Readme.txt "Usage of the binaries"

What have been done on native Yasutaka Furukawa source code :

- Create the CMake build system for CMVS/PMVS2 and required library.

- Optimize a little bit JPEG image loading.

- Update CMVS source code in order to compile.

