.PHONY: all
all: build flash

BINARY = mReader

.PHONY: build
build:
	mkdir -p build \
	&& cd ./build \
	&& cmake ..; \
	make $(BINARY)

.PHONY: flash
flash:
	cd ./build; \
	sudo picotool load mReader.uf2 -v -x

.PHONY: clean
clean:
	rm -rf build

buildAndFlash: build flash
