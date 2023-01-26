# CS118 Project 1

A basic HTTP server implemented in C++.

`server.cpp` contains `main()`, in which a socket is created and an event loop
is run. `request.cpp` and `request.hpp` declare and define a struct that parses
HTTP requests and stores its information. `response.cpp` and `response.hpp`
declare and define a set of functions that build a relevant HTTP response to a
given `Request` struct. Lastly, `config.hpp` defines a set of macros that may be
adjusted as necessary.

## Running the server

Compile or recompile the server simply by running `make` or `make all`.

Once the server is compiled, start it by running `./server`. It listens on port
15635 by default, as defined in `config.hpp`.

## References

TA Discussion Slides:
[1A](https://bruinlearn.ucla.edu/courses/153929/files/12115871?module_item_id=5827859),
[1B](https://bruinlearn.ucla.edu/courses/153929/files/12088772?module_item_id=5824063),
[1C](https://bruinlearn.ucla.edu/courses/153929/files/12090242?module_item_id=5824466)
