ifndef TOPDIR
export TOPDIR	:= $(shell cat ENVVARS\TOP.DIR)
export TOPSRC	= $(TOPDIR)\SOURCE
export TARGETDIR	= $(TOPDIR)\BIN
export OBJSDIR	= $(TOPDIR)\OBJ
endif

TARGET	:= DUNGEON
BUILD_TYPE	:= Debug
SOURCEDIR = Source

debug:	BUILD_TYPE	= debug
debug:	$(TARGET)

# Ideally, this would be an absolute, full path, but the CURDIR uses the
# correct casing and forward slashes, while OBJSDIR uses all upper-case
# and back slashes
ifneq ($(notdir $(OBJSDIR)), $(notdir $(CURDIR)))

CFILES			:= $(foreach dir,$(SOURCEDIR),$(notdir $(wildcard $(dir)/*.c)))
export VPATH	:= $(foreach dir, $(SOURCEDIR),$(CURDIR)/$(dir))
export OBJS		:= $(CFILES:.c=.o)

$(TARGET):
	echo $(OBJSDIR)
	echo $(CURDIR)
	$(MAKE) -C $(OBJSDIR) -f $(TOPSRC)\DOS\Makefile $(BUILD_TYPE)

else
$(TARGET): $(OBJS)
	gcc -o $(TARGETDIR)\$(TARGET) $(OBJS)

%.o: %.c
	gcc -c -MMD -MP -MF $*.d $< -o $@

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
