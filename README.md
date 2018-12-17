# LLVM examples

This repository contains a very simple example project that shows how to write an LLVM optimization pass and how to build a dynamic library as well as a stand-alone application from it. This example was written as part of the LLVM lecture in "Ausgewählte Kapitel aus dem Übersetzerbau" at the Friedrich-Alexander-University Erlangen-Nürnberg.

## Building the dynamic library and the application

To build the dynamic library as well as the stand-alone application simply type 

```
make
```

Without further changes to the Makefile it is required that LLVM is installed and can be found in the search path. This project was built and tested against LLVM version 7.0.

## Running the example pass

The dynamic library can be used with the LLVM `opt` tool, e.g.

```
opt -S -load=./bin/passes.o -strength-reduction <INPUT PROGRAM>
```

The stand-alone application can be run as follows:

```
./bin/app <INPUT PROGRAM>
```

## License

This project is licensed under the terms of the MIT license, see `LICENSE.mit` for further information.
