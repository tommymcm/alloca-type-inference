BUILD_DIR ?= build
INSTALL_DIR ?= install

all: install

build:
	mkdir -p $(BUILD_DIR)
	cmake -DCMAKE_INSTALL_PREFIX=$(INSTALL_DIR) -S . -B $(BUILD_DIR)
	make -C $(BUILD_DIR) all -j32 --no-print-directory

install: build
	mkdir -p $(INSTALL_DIR)
	make -C $(BUILD_DIR) install -j32 --no-print-directory

uninstall:
	-cat $(BUILD_DIR)/install_manifest.txt | xargs rm -f
	rm -f enable
	rm -rf $(BUILD_DIR)

clean:
	make -C $(BUILD_DIR) clean -j32
	rm -rf $(BUILD_DIR)

.PHONY: all build install uninstall clean
