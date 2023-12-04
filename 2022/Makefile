.PHONY: default all run
default: all

CFLAGS:=-g -Og -Wall -Wextra -Iinclude -Ilib -D_CRT_SECURE_NO_WARNINGS

# Gets all filenames from a glob expression without their file extensions
GLOBNAMES=$(sort $(foreach GLOBPATH,$(wildcard $1),$(lastword $(subst /, ,$(basename $(GLOBPATH))))))

# Get day information
$(eval DAYS=$(call GLOBNAMES,src/??.c))
LAST_DAY = $(lastword $(DAYS))

define build_dll
ALL_DLLS += build/$1.dll
build/$1.dll: src/$1.c include/*.h build/aoc.dll
	@clang $(CFLAGS) -shared -DYEAR=2022 -DDAY=$(patsubst 0%,%,$1) src/$1.c -Lbuild -laoc -o build/$1.dll >/dev/null
endef
$(foreach DAY,$(DAYS),$(eval $(call build_dll,$(DAY))))

build/aoc.dll: include/aoc.h lib/*.h
	@clang $(CFLAGS) -shared -DAOC_IMPLEMENTATION -xc $< -o $@ >/dev/null

build/main.exe: src/main.c build/aoc.dll
	@clang $(CFLAGS) $< -Lbuild -laoc -o $@

build/test.exe: src/test.c include/*.h build/aoc.dll
	@clang $(CFLAGS) $< -Lbuild -laoc -o $@

all: $(ALL_DLLS) build/main.exe build/test.exe

run: build/main.exe build/$(LAST_DAY).dll
	@pushd build >/dev/null && ./main.exe $(LAST_DAY) && popd >/dev/null
