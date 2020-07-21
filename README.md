# c-utils
[![Build CI][1]][2]

This repository hosts some modular, drop-in, C utilities that you can copy into
your project and and kick-start your work.

## List of utils:

The following methods/utils are available in this repo:

* Circular Buffers - A data-type agnostic circular buffer implementation read more [here][3].
* Hexdump - A `hexdump -c` output equivalent method to inspect buffers.
* String Library - A string library for some common use cases where pasing
  char pointer, and length around looks ugly.

## How to use this repo?

You can clone this repo and copy individual files from src/ and link it to your
project.

[1]: https://github.com/embedjournal/c-utils/workflows/Build%20CI/badge.svg
[2]: https://github.com/embedjournal/c-utils/actions?query=workflow%3A%22Build+CI%22
[3]: https://embedjournal.com/implementing-circular-buffer-embedded-c/
