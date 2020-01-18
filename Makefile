SUBDIRS = 3ds nds

all:
	@for dir in $(SUBDIRS); do $(MAKE) -C $$dir; done

clean:
	@for dir in $(SUBDIRS); do $(MAKE) clean -C $$dir; done

3ds:
	@$(MAKE) -C 3ds

dsi:
	@$(MAKE) -C dsi

.PHONY: $(SUBDIRS) clean