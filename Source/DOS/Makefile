ifndef TOPDIR
export TOPDIR	:= $(shell cat ENVVARS\TOP.DIR)
export TOPSRC	= $(TOPDIR)\SOURCE
export TARGETDIR	= $(TOPDIR)\BIN
export OBJSDIR	= $(TOPDIR)\OBJ
endif

SOURCEDIR = SOURCE
TARGET	:= DUNGEON

BUILD_TYPE	:= Debug

debug:	BUILD_TYPE	= debug
debug:	$(TARGET)

# Ideally, this would be an absolute, full path, but the CURDIR uses the
# correct casing and forward slashes, while OBJSDIR uses all upper-case
# and back slashes
ifneq ($(notdir $(OBJSDIR)), $(notdir $(CURDIR)))

CFILES			:= $(foreach dir,$(SOURCEDIR),$(notdir $(wildcard $(dir)/*.C)))
SFILES			:= $(foreach dir,$(SOURCEDIR),$(notdir $(wildcard $(dir)/*.S)))
export VPATH	:= $(foreach dir, $(SOURCEDIR),$(CURDIR)/$(dir))
export OBJS		:= $(CFILES:.C=.o) $(SFILES:.S=_S.o)

$(TARGET):
	echo $(OBJSDIR)
	echo $(CURDIR)
	$(MAKE) -C $(OBJSDIR) -f $(TOPSRC)\DOS\Makefile $(BUILD_TYPE)

else
$(TARGET): $(OBJS)
	gcc -o $(TARGETDIR)\$(TARGET) $(OBJS) -lpdcurses

%.o: %.C
	gcc -c -x c -MMD -MP -MF $*.d -I$(TOPSRC)/DOS/HEADERS $(basename $<).c -o $@

%_S.o: %.S
	gcc -c -MMD -MP -MF $*_S.d -I$(TOPSRC)/DOS/HEADERS $(basename $<).s -o $@

-include $(OBJSDIR)\*.d

endif

.PHONY: clean
clean:
	@echo Removing files from $(TARGETDIR)
	@deltree /Y $(TARGETDIR)\*
	@echo Done
	@echo Removing files from $(OBJSDIR)
	@deltree /Y $(OBJSDIR)\*
	@echo Done
	@BOOTSTRAP.BAT
completeclean:
	@echo Removing files from $(TARGETDIR)
	@deltree /Y $(TARGETDIR)\*
	@echo Done
	@echo Removing files from $(OBJSDIR)
	@deltree /Y $(OBJSDIR)\*
	@echo Done
