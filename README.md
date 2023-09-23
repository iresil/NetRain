# Net Rain
### A Windows screensaver & network visualizer in the style of Matrix code rain
___
## About
NetRain is a **Visual C++** project that utilizes .NET 4.7.2 controls via C++/CLI to display the raining green code (a.k.a. code rain) seen in the *Matrix* franchise.
Code rain can be generated in either of the following modes:
- **Randomized**: In this mode, all the related parameters (e.g. the size and fall speed of each raindrop) are randomly generated and the code rain effect is always **green**.
- **Based on Network Traffic**: This mode works as a network visualizer.
Each raindrop's *color* corresponds to a type of traffic (i.e. **green** is for TCP packets, **orange** is for UDP packets and **blue** is for any packet that has `127.0.0.1` as both source and destination).
Each raindrop's *length* corresponds to the number of packets of the specified type captured in the last 6 seconds before the raindrop first entered the screen.

Neither of the two available modes starts with a full code rain effect. Instead, during the first seconds of each execution a faint code "drizzle" appears.
That was done on purpose, since the packet capture process takes a few seconds to begin and until then raindrops are always green.

The code switches between the two available modes automatically, based on whether you simply run it, or run it as administrator.
*You can't run the network visualizer mode without admin rights, since it uses raw sockets to capture the traffic.*

> [!NOTE]
> This started off as an attempt to create a C++/CLI executable with a larger codebase than a plain code exercise, which would also be as simple as possible, with as few dependencies as possible.

## Code Structure
The solution can be opened using **Visual Studio 2022** and it contains 5 different projects:
- **NetRain_UI/NetRain.vcxproj**: This is the base project that contains the entry point of the application.
It was first created using the [C++ Windows Forms Project with GUI](https://marketplace.visualstudio.com/items?itemName=RichardKaiser.CppWinfGUI) extension. It is the only C++/CLI project in the solution.
- **NetRain_Resources/NetRain_Resources.vcxproj**: C++ static library that contains the glyphs displayed in the code rain as resources and also handles resource reading.
- **NetRain_CodeWeather/NetRain_CodeWeather.vcxproj** - C++ static library that defines the basic object structure that will hold information related to the code rain effect.
Also defines a `CodeCloud` class that generates (and randomizes) the related objects. It uses **stdlib.h** for random number generation.
- **NetRain_Networking/NetRain_Networking.vcxproj** - C++ static library that contains a single instance packet sniffer. It retrieves information for transmitted packets of various protocols, using **WinSock2.h**.
The retrieved information will afterwards be used for parameterizing the code rain effect.
- **NetRain_Common/NetRain_Common.vcxproj** - C++ static library that contains consts and enums that will be used across the rest of the projects.

## Acknowledgements
The main idea from which this project originated, was born out of discussion with [**Vartalus**](https://github.com/Vartalus), who also contributed during its creation.

[**@Vartalus**](https://github.com/Vartalus): Thanks for the great cooperation during this project and all the previous ones.

This project was also made possible due to the existence of the following repositories:
- [**rezmason/matrix**](https://github.com/Rezmason/matrix) - The SVG files that have been used as resources in this project are slightly modified versions of
[texture_simplified.svg](https://github.com/Rezmason/matrix/blob/master/svg%20sources/texture_simplified.svg). Some glyphs needed to be modified so that they could be rendered correctly using GDI+.
- [**memononen/nanosvg**](https://github.com/memononen/nanosvg) - Referenced in **NetRain_Resources**, its header has been added to the solution's `include` folder, for easier building.
The folder in question is not taken into account for code and language stats.
