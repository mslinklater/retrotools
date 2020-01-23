# Frequently Asked Questions for retrotools

## Building

### Linux

### Windows

Not supported yet

### MacOS

Not supported yet

## Development

Q/ Why did you code it in C++ rather than <blah> ?

A/ C++ is the language I'm most familiar with. I didn't want to mix up the challenge of wiriting the application with the challenge of learning a new language. I might port it at some stage... you can port it if you like. For now it's in C++ because that's the easiest route to it working.

Q/ Why use cmake ?

A/ Cross platform, wide industry support, I wanted to learn it.

Q/ Why use Dear ImGui ?

A/ Because it is cross platform, lightweight and performant. Ideal for this type of application. It's also got a vibrant community behind it and lots of support.

Q/ What is the code architecture ?

A/ The following patterns are followed:

MVC - Model, view, controller. All UI logic is contained in the Window classes. The model classes are as light as possible and just contain the raw state of the emulated objects. All the glue code is contained in controller classes.

Features
--------

Q/ What machines does it support ?

A/ Currently the Atari 2600 VCS