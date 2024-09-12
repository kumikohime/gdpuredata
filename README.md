# GdPureData
Puredata Plugin for Godot 4.x

Integrates [libpd](https://github.com/libpd/libpd) into the Godot Engine via gdextension

## Documentation

Refer to the official libpd wiki: https://github.com/libpd/libpd/wiki
Almost all methods are exposed in the plugin with sane function names.

## Dependencies

Requirements to build are the same as for compiling godot
Check the relevant article that applies to you
https://docs.godotengine.org/en/stable/contributing/development/compiling/index.html

## Building

1. Change directory to ``gdpuredata/gdextension/gdpuredata/libpd`` and run ``make STATIC=true``.
This will build a static library of the libpd module for the gdextension. 

2. Go back up a folder to ``gdpuredata/gdextension/gdpuredata`` and run ``scons``.
This will create a binary in ``gdpuredata/gdextension/demo/bin``. You will need to move this binary to ``/gdpuredata/addons/`` for the correct platform.
There are set paths in the ``/addons/gdpuredata.gdextension`` file. Adapt to your setup.

## Usage

The plugin is exposed as a Godot node called ``GDPureDataHandle``
Check the demo scene for an example

## Misc

The default buffer length is set at 0.5s. This may cause a latency too high for some.
The latency can be fixed by adjusting the buffer length here.

![image](https://github.com/kumikohime/gdpuredata/assets/76047673/014303bd-d540-4856-8eac-69bde4587817)

The libpd project is under a BSD license. Check the relevant license in the repository.

All other code is released under MIT.

