

ifeq ($(strip $(DEVKITARM)),)
$(error "Please set DEVKITARM in your environment. export DEVKITARM=<path to>devkitARM")
endif

include $(DEVKITARM)/ds_rules

GRAPHICS	:=	acres faces

OUT			:=	../nitrofiles/graphics

INPUTS		:= $(foreach dir,$(GRAPHICS),$(addprefix $(dir)/,$(notdir $(wildcard $(dir)/*.bmp))))

TARGETS		:= $(INPUTS:.bmp=.gfx)

.PHONY	:	bmp2ds clean all directories pokemon

all	:	bmp2ds $(TARGETS)
	@echo graphics ...

bmp2ds	:
	@g++ bmp2ds.cpp -o bmp2ds

%.gfx	:	%.bmp %.txt
	@mkdir -p $(OUT)/$(@D)
	@./bmp2ds $< $(OUT)/$@ -o $(shell cat $(word 2,$^))

%.gfx	:	%.bmp
	@mkdir -p $(OUT)/$(@D)
	@./bmp2ds $< $(OUT)/$@

clean	:
	@echo clean ...
	@rm -f bmp2ds
	@rm -rf $(addprefix $(OUT)/,$(TARGETS))
