Source Code Details:
* Programming Language: C++
* Operating System: Mac OSX 10.11.1/Windows 10
* Compiler/Interpreter: Xcode 7.2 (GCC Clang)/Visual Studio 

File Structure:
For CMake compatibility, Source and Build
* Source/YsFramework/public/src has all the library files of the YsFramework
* Source/YsFramework/Main/ has the following files:
   * clustering.cpp & clustering.h
   * anchor.cpp & anchor.h
   * clustering.cpp & clustering.h
   * mihashtable.cpp & mihashtable.h
   * vertex_to_polygon.cpp & vertex_to_polygon.h
   * lattice.h
   * main.cpp

* Build directory is the place where all the code will be complied and built through CMake

*The build directory contains a Xcode/Visual Studio Project which can be used to make execute the code through Xcode



Compiling Instructions for CMake:
* Open CMake GUI
* Go to File->Delete Cache
* In the source dialog box give the path of YsFramework in the source directory
* In the build dialog box give the path of the build folder(you may choose to create this folder anywhere on the system or you can use the folder which we have provided)
* Click on Configure. From the dialog box select ‘Use default native compilers’. Select Xcode/Visual Studio from the drop-down menu
* Hit Generate. An Xcode/Visual Studio project will be generated in the build directory


How to run the code for Mac OSX:
Through Xcode:
  * Place the test .stl file in the build directory 
  * The build directory contains the Xcode project. Open this project and run the main.cpp file. Follow the instructions to generate the modified .stl file 
  * The modified .stl file can be found in the build directory 
 
Through .app file:  
  * The build directory contains the .app file. Right click on the this file and select Show Package Contents. In the resources folder, place the test .stl file 
  * Double click on the .app file to run the code. Follow the instructions to generate the modified .stl file 
  * The modified .stl file can be found in the resources folder  

How to run the code for Windows:
Through Visual Studio Project:
  * Place the test .stl file in the build directory 
  * The build directory contains the Visual Studio Project. Open this project and run the main.cpp file. Follow the instructions to generate the modified .stl file 
 
Through .exe file:  
  * The build directory contains the .exe file. Place the test .stl file in the same folder as the .exe file 
  * Double click on the .exe file to run the code. Follow the instructions to generate the modified .stl file 
  * The modified .stl file can be found in the same folder 