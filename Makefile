#
#  Copyright (c) 2021 Siddharth Chandrasekaran <sidcha.dev@gmail.com>
#
#  SPDX-License-Identifier: Apache-2.0
#

O       ?= .
NAME    ?= utils
SRC     := $(wildcard src/*.c)
OBJ     := $(SRC:src/%.c=$(O)/$(NAME)/%.o)
CCFLAGS ?= -Wall -Wextra -O3

ifeq ($(V),)
Q    := @
else
Q    :=
endif

.PHONY: all
all: $(O)/lib$(NAME).a
	@echo > /dev/null

$(O)/lib$(NAME).a: $(OBJ)
	@echo "  AR $(@F)"
	$(Q)$(AR) -qc $@ $^

$(O)/$(NAME)/%.o: src/%.c
	@echo "  CC $<"
	@mkdir -p $(@D)
	$(Q)$(CC) -c $< $(CCFLAGS) -Iinclude/ -o $@

.PHONY: clean
clean:
	$(Q)rm -f $(OBJ) $(O)/lib$(NAME).a
