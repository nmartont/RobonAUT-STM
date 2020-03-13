# RobonAUT robotics competition STM embedded software

This repository contains the code for the MainController and the LineController of the system.
Both controllers are of type STM32F446.

## Welcome

Here you can find the source code of a university robotics project me and my friend partook in a couple years ago.
The point of the competition was to build small autonomous robots based on an RC car, which can complete
fast laps around a track following a line, and also finish an obstacle course.

I wrote most of the C code for the project, and my friend did most of the electrical work.
There are two controllers in the system:
* MainController is responsible for the main robot logic and external communication
* LineController is responsible for the handling of the line sensor (32 photo-diodes)

We got the not-so-bad 4th place in this competition, with a fairly impressive obstacle course result.
You can see a short video of the competition [here](https://www.youtube.com/watch?v=prxHJ3FHOCM).

Please feel free to look through the source files to get an idea of my work.
But please keep in mind that the quality of my productive code is much higher, and also has unit testing for every function.
I skipped some parts of the documentation and testing in this project, because of time limitations.
