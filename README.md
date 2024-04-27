Source for an internal DLL cheat for Jedi Academy. Project file is for Visual Studio 2022.

Works with latest OJK-based client and the latest MB2 client. Partially works or crashes if you join some JA+ or other modded servers. Only for OpenGL renderer.

Inject the DLL while at the main menu. As of the latest MB2 build, changing servers or maps frequently can cause the game to crash with a ghoul2 memory error. Not sure what the cause is yet! :(

To compile, you will need to download and put the following dependencies inside the "/external/" folder:

### ImGUI (at least v1.89.6) in "/external/imgui/"

https://github.com/ocornut/imgui

### minhook in "/external/winhook/" 
*(I only noticed I gave the folder the wrong name as I was typing this readme...)*

https://github.com/TsudaKageyu/minhook

### stb_image (At least version 2.28) in "/external/stb/"

https://github.com/nothings/stb/blob/master/stb_image.h

### nlohmann's JSON for Modern C++ in "/external/nlohmann/" (just the json.hpp)

https://github.com/nlohmann/json

Features:
  - In-Game Menu
    - Default keybind is Insert but can be changed.
    - Able to save and load the cheat settings to a JSON file.
    
  - Wallhack
    - Changeable and toggleable custom shader for enemy player models
    - Anti Mind-trick
  - ESP
    - Box and player names
  - Aimbot
    - Guesstimated prediction based on current weapon projectile speed
    - Adjustable targetting angle
    - Toggle and hold keybinds can be changed.
  - Radar
    - Can change background color
    - Can move and change the size
  - Triggerbot
    - (Not implemented but I have the menu tab for it. Maybe later I will implement it)
  

