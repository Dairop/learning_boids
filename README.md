# learning_boids

Boids, represented as fish, form different species that evolve and lean how to behave thanks to natural selection.
The resulting neural network are analyzed after some time learning and used to generate various graphs.

## Prerequisite
- C++ 20
- Have CMake installed

## Compilation
### Only tested on Windows with Visual Studio for the moment 
Detailed explanation on the [cmake ](https://preshing.com/20170511/how-to-build-a-cmake-based-project/ "cmake website")


## Commands
- Clear the data folder before running the program
> > You can not do it but it is easier to see which files are created by your current simulation
  
- Space to start the simulation
  
- All the graphs are created in the "data" folder
> > 3 of them are updated frequently (population, food cycles & interactions)
> > 48 others are updated 250x less frequently (because they take a lot of time to make)
