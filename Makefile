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
	picotool load mReader.uf2 -v -x
# 	openocd -s $(shell pwd) -f interface/cmsis-dap.cfg -f target/rp2040.cfg -f reset.cfg -c "program $(BINARY).elf verify reset exit"

.PHONY: clean
clean:
	rm -rf build

buildAndFlash: build flash
