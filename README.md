# Net Rain
### A C++/CLI network visualizer (in the style of Matrix code rain) for Windows
___
## About
This is a packet sniffer for Windows machines that uses the retrieved packet details as input for dynamically creating the raining green code seen in the *Matrix* franchise.

This started off as an attempt to create a C++/CLI executable with a larger codebase than a plain code exercise, which is also as simple as possible, with as few dependencies as possible.

## Building and Dependencies
The code can be built using **Visual Studio 2022** and the solution contains 4 different projects:
- **NetRain_UI/NetRain.vcxproj**: This is the base project that contains the entry point of the application. It was first created using the [C++ Windows Forms Project with GUI](https://marketplace.visualstudio.com/items?itemName=RichardKaiser.CppWinfGUI) extension. It is the only C++/CLI project in the solution.
  - [**memononen/nanosvg**](https://github.com/memononen/nanosvg) - Used by this project, already included in the solution's `include` folder, for easier building. The folder is not taken into account for code and language stats.
- **NetRain_Resources/NetRain_Resources.vcxproj**: Plain C++ static library that contains the glyphs displayed in the code rain as resources and also handles resource reading.
  - [**rezmason/matrix**](https://github.com/Rezmason/matrix) - The SVG files that have been used as resources in this project are slightly modified versions of [texture_simplified.svg](https://github.com/Rezmason/matrix/blob/master/svg%20sources/texture_simplified.svg). Some glyphs needed to be modified so that they could be rendered correctly using GDI+.
- **NetRain_CodeWeather/NetRain_CodeWeather.vcxproj** - C++ static library that defines the basic object structure that will hold information related to the code rain effect. Also defines a `CodeCloud` class that generates (and randomizes) the related objects. It uses **stdlib.h** for random number generation.
- **NetRain_Networking/NetRain_Networking.vcxproj** - C++ static library that contains a single instance packet sniffer. It retrieves information for transmitted packets of various protocols, using **WinSock2.h**. The retrieved information will afterwards be used for parameterizing the code rain effect.
