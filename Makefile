#
#  Copyright (c) 2021 Siddharth Chandrasekaran <sidcha.dev@gmail.com>
#
#  SPDX-License-Identifier: Apache-2.0
#

O       ?= .
SRC     := $(wildcard src/*.c)
OBJ     := $(SRC:src/%.c=$(O)/src/%.o)
CCFLAGS ?= -Wall -Wextra -O3

ifeq ($(V),)
Q    := @
else
Q    :=
endif

.PHONY: all
all: $(O)/libutils.a
	@echo > /dev/null

$(O)/libutils.a: $(OBJ)
	@echo "  AR $(@F)"
	$(Q)$(AR) -qc $@ $^

$(O)/src/%.o: src/%.c
	@echo "  CC $<"
	@mkdir -p $(@D)
	$(Q)$(CC) -c $< $(CCFLAGS) -Iinclude/ -o $@

.PHONY: clean
clean:
	$(Q)rm -f $(OBJ) $(O)/libutils.a
