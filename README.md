# retrotools

This is really out of date... I'll get to sorting this out soon, honest 8). 

Mainly 6502 stuff

## Some videos

[Jan 2020](https://www.youtube.com/watch?v=ZFyCltxY-v0)

[First run of Combat](https://www.youtube.com/watch?v=vHxzVI3zZks)

[Playfields](https://www.youtube.com/watch?v=vk6umVQUwZQ)

[First ROM](https://www.youtube.com/watch?v=yhQVkIKHjTM)

## To Build

To build Retrotools you need a copy of my 'shared_cpp' repo in the source tree. I have it set up so that the 'shared_cpp' project is alongside 'retrotools' project, then symbolicaally link the folder in to retrotools. You could also copy it in... it is up to you. Whichever way you choose you need th make sure the 'shaared_cpp' folder in inside the 'retrotools' folder.

Once shared_cpp is set up, you also need to install cmake

### Linux

This is based off Ubunti 20.04 basic install.

1) Make sure you have cmake installed.
2) In a console execute the following commands:

cmake .

make

You should now have the 'vistella' executable in the folder

## Command Line Parameters

TODO
