.PHONY: default all run
default: all

ROOTDIR=X:/Code/AOC
CFLAGS:=-g -Og -Wall -Wextra -Isrc -D_CRT_SECURE_NO_WARNINGS

GLOBNAMES=$(sort $(foreach GLOBPATH,$(wildcard $1),$(lastword $(subst /, ,$(basename $(GLOBPATH))))))
YEARS=$(call GLOBNAMES,src/????)
$(foreach YEAR,$(YEARS),$(eval DAYS_$(YEAR)=$(call GLOBNAMES,src/$(YEAR)/??.c)))

build/aoc.dll: src/aoc.h src/lib/*.h
	@clang $(CFLAGS) -shared -DAOC_IMPLEMENTATION -xc $< -o $@ >/dev/null

define build_dll
ALL_DLLS += build/$1/$2.dll
build/$1/$2.dll: src/$1/$2.c src/arr.h src/set.h build/aoc.dll
	@clang $(CFLAGS) -shared -DYEAR=$1 -DDAY=$(patsubst 0%,%,$2) src/$1/$2.c -Lbuild -laoc -o build/$1/$2.dll >/dev/null
endef

$(foreach YEAR,$(YEARS),$(foreach DAY,$(DAYS_$(YEAR)),$(eval $(call build_dll,$(YEAR),$(DAY)))))

build/main.exe: src/main.c build/aoc.dll
	@clang $(CFLAGS) $< -Lbuild -laoc -o $@

build/test.exe: src/test.c src/arr.h src/set.h build/aoc.dll
	@clang $(CFLAGS) $< -Lbuild -laoc -o $@

all: $(ALL_DLLS) build/main.exe build/test.exe

LAST_YEAR = $(lastword $(YEARS))
LAST_DAY  = $(lastword $(DAYS_$(LAST_YEAR)))
run: build/main.exe build/$(LAST_YEAR)/$(LAST_DAY).dll
	@pushd build >/dev/null && ./main.exe $(LAST_YEAR)/$(LAST_DAY) && popd >/dev/null
