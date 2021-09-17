<p align="center">
  <img src="https://cdn.discordapp.com/attachments/633472429917995038/722143730500501534/Hydroxide_Logo.png"/>
</p>

# Hydroxide++
Penetration testing tool for games developed on the Roblox game engine. (but this time, written in C++)

*view the original version of Hydroxide (written in Lua) here: https://github.com/Upbolt/Hydroxide*

## Why?
The main reason behind the creation of this C++ port is speed. A couple hundred Roblox frames drops the frame count pretty significantly, so this is an experiment to see if ImGui will work any better.

Another bonus is that developers will not be limited to the functions of the scripting utility that they use.

## What's in store?
I plan to implement everything from the original Lua version of Hydroxide into this version. If there are things that can't be added (which, hopefully, should not be very much if at all), it will be listed here.

Planned features:
- Upvalue Scanner
- Constants Scanner
- LocalScript/ModuleScript Scanner (plans on a decompiler as well!)
- Remote & Closure Spy
- HttpSpy (using actual packet logging, not just hooking Lua functions)

## Building from source
Building from source is pretty straight forward, just clone the repository in Visual Studio. 

Once cloned, there are a few dependencies you will need to install.

Microsoft's [vcpkg](https://github.com/microsoft/vcpkg) is recommended, just to make the installation process easier on you. 

`vcpkg install minhook:x86-windows-static`

`vcpkg install nlohmann-json:x86-windows-static`

## Credits
Default Instance Explorer Icons: https://github.com/Elttob/Vanilla
