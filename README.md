# Velocity ver 0.1
A super lightweight and fast Typing speed Evaluator for Windows Command Prompt/ Powershell in C/C++. To run velocity either build from source using `cmake`/`ninja` or clone the repo and add the path of the project (where `velocity.exe` resides) to path environment variable to run anywhere using the command `velocity` (prefer the latter).

## Improvements ?
Add a Performance graph.

## `winconsole.h`
A C library written utilising the windows console buffer library to implement useful functions for clearing screen, getting non-blocking single keyboard entry etc. from the conio.h library and along with that other cool console colour setting functions. This is done considering the fact that `conio.h` is largely incompatible with many systems and compilers (almost all compilers that aren't Turbo/Borland). 
