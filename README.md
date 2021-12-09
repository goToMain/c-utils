# c-utils
[![Build CI][1]][2]

This repository hosts some modular, drop-in, C utilities that you can copy into
your project and and kick-start your work.

## List of utils:

The following methods/utils are available in this repo:

  - **arg_parser** - Command line argument parsing helper
  - **bus_server** - A broadcasting IPC server for connected clients
  - **channel** - A Communication protocol (uart, msgq, tcp, etc.,) abstraction layer
  - **circbuf** - Generic, lock-free, circular/ring buffer implementation
  - **disjoint_set** - Disjoint-set library
  - **event** - Event notifier for multi-threaded applications
  - **fdutils** - Collection of operations that are commonly performed on unix file descriptors
  - **file** - Collection of methods that operate on directories, files and paths
  - **filo** - First-In Last-Out (stack) implementation
  - **hashmap** - Hashmap/dictionary/map library
  - **list** - Singly and doubly linked list data library
  - **logger** - Logging module for C applications with log-levels, colors, and log-to-file features
  - **memory** - Do-or-die helper methods that allow use of mallloc/calloc/strdups without NULL checks
  - **procutils** - Linux process manipulation utilities
  - **queue** - Last-in First-out (queue) implementation
  - **serial** - Library to interact with uart devices
  - **slab** - Poor man's slab allocator for dynamic memory without using heap
  - **sockutils** - Collection of methods that operate on sockets
  - **stack** - Stack implementation using linked lists
  - **strlib** - A string_t type and some common methods that operate on them
  - **strutils** - Commonly used C string utils
  - **utils** - Other ad-hoc methods that don't fit anywhere else
  - **workqueue** - Worker thread library that waits for a job to execute

## How to use this repo?

You can clone this repo and copy individual files from src/ and link it to your
project.

[1]: https://github.com/embedjournal/c-utils/workflows/Build%20CI/badge.svg
[2]: https://github.com/embedjournal/c-utils/actions?query=workflow%3A%22Build+CI%22
[3]: https://embedjournal.com/implementing-circular-buffer-embedded-c/
