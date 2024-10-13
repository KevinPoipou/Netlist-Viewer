# Netlist Viewer

This application, developed in C++ and using Qt4, allows you to read XML descriptions of digital circuits and display them

## Getting Started

### Required package :
- Qt4
- cmake
- c++ compiler
  
### To run the application, follow these steps:
1. Navigate to the build folder
   ```bash
   cd build
2. Run CMake
   ```bash
   cmake ../src
3. Install the application
   ```bash
   make install
5. Execute the application
   ```bash
   ./tme810

## The application
![Preview of the app on a fulladder](./images/app.png)

### Shortcuts

| Shortcut       | Action                                      |
|----------------|---------------------------------------------|
| Arrow keys     | Navigate through the application            |
| Ctrl +         | Zoom in                                     |
| Ctrl -         | Zoom out                                    |
| Ctrl =         | Set zoom to default                         |
| Ctrl S         | Save                                        |
| Ctrl O         | Open a new cell                             |
| Ctrl C         | Open the current instances widget           |
| Ctrl I         | Open the cells library                      |
| Ctrl A         | Open the current instances widget and cells library |
| Ctrl Q         | Quit the app                                |


If you don't want the widgets to end up behind the application, you can force them to always stay on top in the Options > Widget priority
