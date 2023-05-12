.PHONY: all build test clean

all: build test install

build:
	cmake -Saes-cmac -Bcmake-build -DCMAKE_INSTALL_PREFIX=cmake-install
	cmake --build cmake-build 

test:
	./cmake-build/AesCmac_test

install:
	cmake --install cmake-build
	python cmake-install/PyAesCmac_test.py

clean:
	rm -rf cmake-build/ cmake-install/
