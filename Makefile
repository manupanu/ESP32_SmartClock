FQBN=esp32:esp32:lolin_c3_mini
PORT=/dev/cu.usbmodem21201
SKETCH=ESP32-Clock.ino
BAUD=115200

all: compile

compile:
	arduino-cli compile --fqbn $(FQBN) $(SKETCH)

upload: compile
	arduino-cli upload -p $(PORT) --fqbn $(FQBN) $(SKETCH)

monitor:
	tio -b $(BAUD) $(PORT)

.PHONY: all compile upload monitor
