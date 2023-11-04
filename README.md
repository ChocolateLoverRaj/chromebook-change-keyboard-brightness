# Unnecessary Project
Coolstar's keyboard driver is way better than using AutoHotKeys. Don't use AutoHotKeys. To make brightness keys change the brightness on a Chromebook use this guide: https://github.com/chrultrabook/docs/issues/63

# Change Keyboard Brightness
## Basic Usage
```ps1
"Change Keyboard Brightness.exe" +20 # Increase the brightness by 20%
"Change Keyboard Brightness.exe" -20 # Decrease the brightness by 20%
```
Don't worry about going below 0% or above 100%, it is limited between 0%-100%

## Usage with AutoHotKeys
Use [`run-hidden`](https://github.com/stax76/run-hidden#readme) so that no window pops up.
