# Velocity ver 0.1
A lightweight and fast system console Typing speed Evaluator for Windows in C/C++. To run velocity either build from source using cmake/ninja or download the velocity executable and run in command prompt with .\velocity (prefer the latter).

## Improvements
First of all to add a File handler that writes the user performance into history.txt and take inferences from it.
Second to add several other text examples and allow choice between standard and random text. A random sentance generator to produce random combinations of relevant words
Option to add one's own typing text as a .txt.

## winconsole.h
A C library written utilising the windows console buffer library to implement useful functions for clearing screen, getting non-blocking single keyboard entry etc. from the conio.h library and along with that other cool console colour setting functions. This is done considering the fact that conio.h is largely incompatible with many systems and compilers (almost all compilers that aren't Turbo/Borland). 
