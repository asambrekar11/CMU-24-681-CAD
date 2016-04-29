**********************************************************************************
SOURCE CODE DETAILS
**********************************************************************************
* 	Programming Language: C++
* 	Operating System: Mac OSX 10.11.1/Windows 10/Windows 8/ Windows 7
* 	Compiler/Interpreter: Xcode 7.2 (GCC Clang)/Visual Studio 

**********************************************************************************
FILE STRUCTURE
**********************************************************************************
If you want to build your own project to run this application, you will have to use
CMake to build the project. The file structure for CMake is as follows:
* 	The folder src/public/src has all the library files of the YsFramework
* 	src/Main/ has the following source files we wrote for the project:
	* clustering.cpp & clustering.h
	* anchor.cpp & anchor.h
	* clustering.cpp & clustering.h
	* mihashtable.cpp & mihashtable.h
	* vertex_to_polygon.cpp & vertex_to_polygon.h
	* lattice.h
	* main.cpp
If you want to un the program, we have included a folder of executables for the
program. Please refer running instructions given below

**********************************************************************************
CMAKE INTRODUCTION
**********************************************************************************
CMake is a cross-platform build tool used to generate project files on different
developer APIs

To generate a project for this code on CMake, you will have to create a Build
directory. You may name it as per your liking. For the purposes of instruction
we assume it to be named as "Build". This directory is where the Project files
for your API will be generated.

**********************************************************************************
COMPILING INSTRUCTIONS
**********************************************************************************
* 	Open CMake GUI.
*	If you are using an existing Build directory for some other project or wish to
	change the API for building the project (especially for MAC OSX users, you can
	change from XCode to make) then go to File->Delete Cache.
* 	In the source dialog box give the path of src folder in the source directory.
* 	In the build dialog box give the path of the "Build" folder (you may choose to
	create this folder anywhere on the system).
* 	Click on Configure. From the dialog box select ‘Use default native compilers’.
	You can select Xcode/Visual Studio/Unix Makefiles from the drop-down menu.
* 	Hit Generate. An Xcode/Visual Studio project will be generated in "Build".

**********************************************************************************
RUNNING EXECUTABLES
**********************************************************************************

Please follow the instructions given below to run the program.

How to run the code for Mac OSX:

1.	Through Xcode:
	*	Place the test .stl file in the "Build" directory.
	*	The build directory contains the Xcode project. Open this project.
	*	Select mi_project scheme to build and run. In edit schemes, choose the
		"Build" directory as the working directory.
	*	Run the main.cpp file..
	*	Enter the name of STL file.
	*	Once the file is read, enter number of clusters.
	*	The modified STL file can be found in the "Build" directory.
	Note - You must keep the STL file in the working directory set above.
 
2.	Through .app file:
	*	Open Terminal and Navigate to the folder containing all the source files.
	*	Go to the executables/MacOS folder.
	*	Type "mi_project.app/Contents/MacOS/mi_project" in the terminal and press
		Enter.
	*	Enter the name of STL file.
	*	Once the file is read, enter number of clusters.
	*	The modified STL file can be found in the current directory.
	Note - You must keep the STL file in the current directory set above.

How to run the code for Windows:
1.	Through Visual Studio Project:
	*	The "Build" directory contains the Visual Studio Project. Open this
		project.
	*	In the Solution Explorer, right-click on mi_project and select Build from
		the drop-down menu.
	*	After the build is complete, right-click again and select Open new
		instance from the Debug sub-menu.
	*	This generates an EXE file in the Build/Main/Debug/ folder if you build
		it in Debug mode and Build/Main/Release/ if you build in Release Mode
	* 	Keep the STL files in the folder in above step and use instructions given
		below for running the EXE.
 
2.	Through EXE file:
	*	Open Command Prompt(cmd) and navigate to the folder in which your exe is
		placed. If you are running a Visual Studio generated EXE then that
		folder is Build/Main/Debug/ or Build/Main/Release depending on the mode
		of build. If you want to run our EXE, it is placed in the
		executables/Windows/ folder of our source folder.
	*	Ensure all your STL files are kept in this folder.
	*	Now, type "mi_project.exe" in your Command Prompt.
	*	Enter the name of STL file.
	*	Once the file is read, enter number of clusters.
	*	The modified STL file can be found in the current directory.

If you have any problems running our file, feel free to reach out to us.

**********************************************************************************
THANK YOU. HAVE A GREAT SUMMER
**********************************************************************************