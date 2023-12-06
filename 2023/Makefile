.PHONY: default all clean run
default: all

CFLAGS:=-g -Og -Wall -Wextra -Iinclude -Ilib -D_CRT_SECURE_NO_WARNINGS

# Gets all filenames from a glob expression without their file extensions
GLOBNAMES=$(sort $(foreach GLOBPATH,$(wildcard $1),$(lastword $(subst /, ,$(basename $(GLOBPATH))))))

# Get day information
$(eval INPUTS=$(call GLOBNAMES,src/??.c src/??-1.c src/??-2.c))
LAST_DAY = $(patsubst %-1,%,$(patsubst %-2,%,$(lastword $(INPUTS))))

define build_dll
ALL_DLLS += build/$1.dll
build/$1.dll: src/$1.c include/*.h build/aoc.dll
	$$(info Creating $1.dll...)
	@clang $(CFLAGS) -shared -DAOC_DAY=$(patsubst %-1,%,$(patsubst %-2,%,$1)) \
		src/$1.c -Lbuild -laoc -o build/$1.dll > /dev/null
endef
$(foreach INPUT,$(INPUTS),$(eval $(call build_dll,$(INPUT))))

build/aoc.dll: include/aoc.h lib/*.h
	$(info Creating $@...)
	@clang $(CFLAGS) -shared -DAOC_IMPLEMENTATION -xc $< -o $@ >/dev/null

build/main.exe: src/main.c build/aoc.dll
	$(info Creating $@...)
	@clang $(CFLAGS) $< -Lbuild -laoc -o $@

build/test.exe: src/test.c include/*.h build/aoc.dll
	$(info Creating $@...)
	@clang $(CFLAGS) $< -Lbuild -laoc -o $@

all: $(ALL_DLLS) build/main.exe build/test.exe

clean:
	@rm build/*

run: build/main.exe build/$(LAST_DAY).dll
	@pushd build >/dev/null && ./main.exe $(LAST_DAY) && popd >/dev/null
