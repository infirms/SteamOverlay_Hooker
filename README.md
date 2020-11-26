# SteamOverlay_Hooker
Repository conatain simple example how you can use Steam Gameoverlay to hook functions.

# Functions

  - Trampoline hook / unhook function.
  - Example with hooking WINAPI FlashWindow function.


# Syntax
```cpp
hook_function( (void*) function, (void*) detour_target, (void*) orig_fn, (int)unknown_var);
```
```cpp
unhook_function( (void*) function, (bool) should_log);
```
* (bool) should log = collect logs about unhook to the the GameOverlayRenderer.log

# Usage

  - Compile project with Release / x86 .
  - Inject to any x86 process that has GameOverlayRenderer.dll in the memory.

# Note*

In case you are using this don't forget about credits :D.

Feel free to do any pull requests.

Code that this repository contains is done for demonstration purposes and it's like a PoC, so don't take it seriously. Thanks!
