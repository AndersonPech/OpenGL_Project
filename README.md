# OpenGL CMake setup for Windows, Mac and Linux

## 1. Prerequisite downloads:

All platforms require Git and CMake installed which you can install with the instructions below. For Windows you will need to manually install a CPP toolchain (see 'Windows' section for more details).

### Mac
- Install "brew" from https://brew.sh/ if you haven't already.
- To install Git: `brew install git`
- To install CMake: `brew install cmake`

### Linux
- Run `sudo apt update` to get latest package information.
- To download git run `sudo apt install git`
- To download cmake run `sudo apt install cmake`
- You will also need to run this command `sudo apt install -y libx11-dev libxrandr-dev libxinerama-dev libxcursor-dev libxi-dev`

### Windows
- Download and run the Git installer from https://gitforwindows.org/ if you don't have git installed.
- Download and run the CMake installer from https://cmake.org/download/. Make sure you check the add-cmake-to-path option during the installation process.
- You will need to download MSYS2 for a CPP toolchain.
   - Download and run the MSYS2 installer from https://www.msys2.org/
   - For the installation folder choose the default "C:\msys64".
   - Click finish to open up an MSYS2 terminal.
   - In this terminal run:
      1. `pacman -Syu` (will exit the terminal on completion)
      2. Re-open "MSYS2 MSYS" terminal from the windows start menu.
      2. `pacman -Su`
      3. `pacman -S --needed base-devel mingw-w64-x86_64-toolchain`
      4. You will also need a CMake generator (ninja), so run 
         - `pacman -S mingw-w64-x86_64-ninja`
       
- Add `C:\msys64\mingw64\bin` to your PATH. 
   - From windows start menu, search for "Edit system environment variables"
   - In the "Advanced" tab click "Environment variables"
   - In "system variables" double-click "Path".
   - Add the new path `C:\msys64\mingw64\bin`
   - You will have to reload any IDE or terminal for the changes to take effect.

### All
- Clone this repo with `git clone <repo>`
- Open up the repo directory with VSCode or CLion.

## 2. Project structure (info)

We are using a CMake build system. In case you need to add your own dependencies in the future you will need to understand the basics of CMake (https://cmake.org/cmake/help/latest/guide/tutorial/index.html).

Most CMake-based projects follow a similar structure:

<project_root>
   - `bin` (folder containg runtime executables and resources)
   - `build` (folder containing .o files and other build artifacts)
   - `lib` (contains 3rd party dependencies)
   - `include` (this project's include for `#include`s)
   - `res` (source resource folders [copied into `bin`])
   - `src` (project source code)
   - `test` (for any tests)
   - CMakeLists.txt (project build root)

Generally, each main subdirectory (except `res`) will have their own CMakeLists.txt, allowing for per-directory configuration.


## 3. Development IDEs

The only IDEs that are officially support are [VS Code](https://code.visualstudio.com/) and [CLion](https://www.jetbrains.com/clion/). Note: to use CLion you will have to sign up for a free student Jetbrains account, then you will have access to the IDE until you graduate (need to manually renew every year).

At some point Vscode/CLion will prompt you to choose a toolkit which is a set of tools CMake will use to compile the project. So when choosing a tookit/toolchain:
- On Windows choose the GCC 10.3.0 toolkit provided by MinGW.
- On Mac and Linux choose any toolkit that supports C++ (i.e. contains "CXX = ..." in the list of compilers).

### Running on VSCode:
- In a terminal or through the GUI, open this directory (for terminal users run `code .`).
- Make sure you have the "CMake Tools" extension installed.
- Vscode will prompt you to configure the cmake project. Click Yes. You can also do that manually by typing Crtl-Shift-P or Command-Shift-P and selecting "CMake: Configure". The first time it configures there should be an error about no dependencies; so run `sh fetch_deps.sh` script.
   - If you are on windows do `sh fetch_deps.sh -G Ninja` in a git-bash terminal.
- Delete your CMake cache and re-configure.
- Click the play button on the bottom toolbar next which should be next to one of the target names. The first time you click this you may be prompted to select a target.
- You can change targets by clicking "[\<target>]" on the bottom toolbar.
- Clicking the play button will run the target without debugging which would create a "CMake/Launch" terminal in the directory of the target's executable.
- (CMake Tools extension BUG) Running another target will attempt to run the executable in the same "CMake/Launch" terminal which generally undesirable as the executables depend on relative paths to find its resources. You will need to kill this terminal first and re-run the target so a new "CMake/Launch" terminal is created in the right directory.

### Running on CLion:
- Open up this repo in CLion and let it configure.
-  The first time it configures there should be an error about no dependencies; so run `sh fetch_deps.sh` script.
   - If you are on windows do `sh fetch_deps.sh -G Ninja` in a git-bash terminal.
- Delete your CMake cache and re-configure.
- In the top right, choose one of the targets.
- Click the play button to build and run, or just the hammer to build.


To fetch the latest dependencies simply re-run the `fetch_deps.sh` script as outlined above.


Reconfigure and run. It should work now if it didn't before :)
