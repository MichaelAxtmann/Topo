Topo is a multi-platform system topology abstraciton library for x86-based systems containing one or more NUMA nodes.
It is intended as a wrapper around the hwloc library to provide helper functions for other libraries that might use it.
Furthermore, it exposes a single read-only instance of the system-topology so that libraries that query the topology do not need separate instances.

Topo is implemented in C++.


# Requirements

To build and link with Topo, the following are required.

- 64-bit x86-compatible processor with support for AVX instructions
  
  Topo has been tested with Intel processors of the Sandy Bridge generation and newer.
  Non-Intel processors may also be compatible if they support the required instructions.
  Do not attempt to use it on older-generation processors, at the risk of encountering "Illegal Instruction" errors.
  
- Windows 10 Pro or Ubuntu 14.04
  
  64-bit versions of these operating systems are required.
  Other Linux distributions are likely compatible, but they have not been tested.

- [**hwloc**](https://www.open-mpi.org/projects/hwloc/) and its dependencies
  
  Topo makes use of this library to obtain system topology information in a platform-independent manner.
  It has been tested with version 1.11.4.
  Some distributions of Linux may make this library available as a package.
  Otherwise, and in all cases on Windows, it must be downloaded and installed manually.


# Building

On all platforms, Topo compiles to a static library.

The Windows build system is based on Visual Studio 2015 Community Edition. Compilation is known to work from the graphical interface, but command-line build is also likely possible.

To build on Linux, just type `make` from within the repository directory.


# Linking and Using

Projects that make use of Topo should include the top-level topo.h header file and nothing else.

Assuming a Linux-based C-language project that uses Topo and consists of a single source file called "main.c", the following command would build and link with Topo.

    g++ main.c -ltopo -lhwloc -lnuma -lpciaccess -lxml2


# Getting Started

Documentation is available and can be built using Doxygen.
It covers both the external API in topo.h and Topo's internals, the latter being of interest only to those who wish to modify the implementation of Topo.

On Linux, type `make docs` to compile the documentation. On Windows, run the Doxygen tool using the repository directory as the working directory (it may be necessary to create the output directory manually first).

The primary purpose of Topo is to provide helper functions for supporting other libraries that use hwloc.
Only a few external function calls are made available; the hwloc documentation is the best source of information on how to interact with the objects Topo exposes.
Note, however, that the objects Topo exposes should not be modified in place. They may, however, be duplicated and the duplicates modified however needed.


# Copyright

Topo is licensed under BSD 3-clause (see "LICENSE" in the top-level source code directory).

Copyright (c) 2016-2017 Stanford University, Department of Electrical Engineering.
Authored by Samuel Grossman.
