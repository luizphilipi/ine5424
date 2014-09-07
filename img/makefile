# EPOS Image Makefile

include	../makedefs

IMAGE := $(addsuffix .img,$(APPLICATION))
OUTPUT := $(addsuffix .out,$(APPLICATION))

all:		$(IMAGE)

run:		$(IMAGE)
		$(EMULATOR) $(IMAGE) | $(TEE) $(OUTPUT)

debug:		$(IMAGE)
		$(EMULATOR) $(IMAGE) | $(TEE) $(OUTPUT) &
		$(DEBUGGER) $(APP)/$(APPLICATION)

clean:
		$(CLEAN) *_boot *_setup *_init *_system $(APPLICATION)
