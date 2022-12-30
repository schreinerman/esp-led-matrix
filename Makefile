build_path:=build
release_path:=release

cat := $(if $(filter $(OS),Windows_NT),type,cat)
version_info := $(shell $(cat) VERSION)

all: clean webupdate esp8266 esp32

webupdate:
	python utils/create_web_store.py
	
esp8266 esp32:
	arduino-cli core install $@:$@
	arduino-cli compile --build-path "$(build_path)" --profile $@ --build-property "compiler.cpp.extra_flags=-DAPP_VERSION=\"V$(version_info)\""

	mkdir -p "$(release_path)"
	$(eval NAME := $(notdir $(basename $(wildcard ./*.ino))))

	mv "./$(build_path)/"*.ino.bin "./$(release_path)/v$(version_info)_$@_$(NAME).bin"
clean:
	mkdir -p build
	rm -fR build/*
