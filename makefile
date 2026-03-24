BUILDDIR := build

all: engine

engine: | $(BUILDDIR)
	g++ src/engine.cpp -o build/engine -lraylib

clean:
	rm -rf $(BUILDDIR)

$(BUILDDIR):
	mkdir $(BUILDDIR)
