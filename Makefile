#
#  Copyright (c) 2021 Siddharth Chandrasekaran <siddharth@embedjournal.com>
#
#  SPDX-License-Identifier: Apache-2.0
#

O       ?= .
SRC     := $(wildcard src/*.c)
OBJ     := $(SRC:src/%.c=$(O)/src/%.o)
CCFLAGS ?= -Wall -Wextra -O3

ifeq ($(VERBOSE),)
Q    := @
else
Q    :=
endif

all: $(O)/libutils.a
	@echo > /dev/null

$(O)/libutils.a: $(OBJ)
	@echo "  AR $@"
	$(Q)$(AR) -qc $@ $^

$(O)/src/%.o: src/%.c
	@echo "  CC $<"
	@mkdir -p $(@D)
	$(Q)$(CC) -c $< $(CCFLAGS) -Iinclude/ -o $@

.PHONY: clean
clean:
	$(Q)rm -f $(OBJ) $(O)/libutils.a
