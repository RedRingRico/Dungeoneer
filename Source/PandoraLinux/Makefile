###############################################################################
#                                 Dungeoneer Makefile [PandoraLinux] Ver. 1.0 #
###############################################################################
##### Make sure that the Angstrom ARM toolchain root was defined ##############
ifeq ($(strip $(PNDSDK)),)
$(error Please set PNDSDK to point to your Pandora toolchain root directory  )
endif

ifeq ($(strip $(ZEDSDK)),)
$(error Please set ZEDSDK to point to your ZED SDK root directory )
endif

ifndef TOPDIR
export TOPDIR		:= $(shell cd ../../ && pwd)
export TOPSRC		= $(TOPDIR)/Source
export TARGETDIR	= $(TOPDIR)/Bin/$(PLATFORM)/$(ARCH)/$(BITSIZE)
export OBJSDIR		= $(TOPDIR)/Obj/$(PLATFORM)/$(ARCH)/$(BITSIZE)/GCC/$(GCCVER)/$(BUILD)
endif
SOURCEDIR	= Source ../Common/Source

TOOL_PREFIX = pandora-

PROJECT := Dungeoneer
TARGET := $(PROJECT)
OUTFILE	=	$(TARGETDIR)/$(TARGET)_$(GCCVER)

PLATFORM		:= PandoraLinux
BUILD_PLATFORM	:= PANDORA_LINUX
BUILD_ARCH		:= ARM
ARCH			:= arm
BITSIZE			:= 32

GCCVER	= $(shell $(CPP) -dumpversion)

BUILD_TYPE := debug

CPP = $(TOOL_PREFIX)g++

CPPFLAGS	=	-c -DZED_BUILD_$(BUILD_DEF) \
				-DZED_PLATFORM_$(BUILD_PLATFORM) \
				-DZED_PLATFORM_$(BUILD_PLATFORM)_$(BUILD_ARCH) \
				-DZED_PLATFORM_$(BUILD_PLATFORM)_$(BUILD_ARCH)_$(BITSIZE) \
				-DZED_BITSIZE_$(BITSIZE) -DZED_ARCH_$(BUILD_ARCH) \
				-DZED_WINDOWSYSTEM_X11 -DSUPPORT_X11 \
				-I$(TOPSRC)/$(PLATFORM)/Headers -I$(TOPSRC)/Common/Headers
CPPFLAGS_EXT	= -ffriend-injection -std=c++0x -mcpu=cortex-a8 \
					-Wa,-mcpu=cortex-a8 -mfpu=neon -Wa,-mfpu=neon \
					-mfloat-abi=softfp
SYSIPATH	=	-idirafter $(PNDSDK)/usr/include
INCPATH		=	-I$(ZEDSDK)/include/$(PLATFORM)
SYSLPATH	=	-L$(PNDSDK)/usr/lib
LIBPATH		=	-L$(ZEDSDK)/lib/$(PLATFORM)/$(ARCH)/$(BITSIZE)
SYSLIBS		=	-lX11 -lGLESv2 -lEGL -lrt -pthread -lc -lIMGegl -lXau -lXdmcp \
				-lsrv_um
LINKFLAGS	=

##### Debug #######
debug:		BUILD		= Debug
debug:		BUILD_TYPE	= debug
debug:		BUILD_DEF	= DEBUG
debug:		TARGET := $(TARGET)D
debug:		CPPFLAGS += -g -ggdb -Wall -D_DEBUG $(CPPFLAGS_EXT)
debug:		LIBS = -lZEDRendererD -lZEDArithmeticD -lZEDSystemD
debug:		$(TARGET)

##### Release #####
release:	BUILD		= Release
release:	BUILD_TYPE	= release
release:	BUILD_DEF	= RELEASE
release:	TARGET := $(TARGET)
release:	CPPFLAGS += -O3 $(CPPFLAGS_EXT)
release:	LIBS = -lZEDRenderer -lZEDArithmetic -lZEDSystem
release:	LINKFLAGS += -Wl,-S
release:	$(TARGET)

##### Profile #####
profile:	BUILD		= Profile
profile:	BUILD_TYPE	= profile
profile:	BUILD_DEF	= PROFILE
profile:	TARGET := $(TARGET)P
profile:	CPPFLAGS += -O3 -g -ggdb -Wall -D_DEBUG $(CPPFLAGS_EXT)
profile:	LIBS = -lZEDRendererP -lZEDArithmeticP -lZEDSystemP
profile:	$(TARGET)

##### Build the object files while not in the Obj directory ###################
ifneq ($(OBJSDIR), $(CURDIR))

VERSIONINFO:
	@printf "%s" "------------------------- Generating Version Information ---"
	@printf "%s\n" "-------------------"
	@mkdir -p Headers
	@sh ./GitVersion.sh ./Headers/GitVersion.hpp $(PROJECT)
	@printf "%s" "------------------------------------------------------------"
	@printf "%s\n" "-------------------"

TARGETDIR:
	@mkdir -p $(TARGETDIR)

OBJSDIR:
	@mkdir -p $(OBJSDIR)

CPPFILES	:= $(foreach dir,$(SOURCEDIR),$(notdir $(wildcard $(dir)/*.cpp)))
export VPATH	:= $(foreach dir,$(SOURCEDIR),$(CURDIR)/$(dir))
export OBJS		:= $(CPPFILES:.cpp=.o)

$(TARGET): OBJSDIR TARGETDIR VERSIONINFO
	@/usr/bin/time -f "%E" -o $(OBJSDIR)/compiletime $(MAKE) \
	--no-print-directory -C $(OBJSDIR) -f $(TOPSRC)/$(PLATFORM)/Makefile \
	$(BUILD_TYPE)
	@printf "\n%s" "----------------------------------------------------------"
	@printf "%s\n" "---------------------"
	@printf "Compile time: "
	@cat $(OBJSDIR)/compiletime
	@printf "%s" "------------------------------------------------------------"
	@printf "%s\n" "-------------------"

else

##### In the intermediate build directory #####################################

$(TARGET): $(OBJS)
	@printf "\n\n%s" "------------------------- MESSAGES FROM THE COMPILER ---"
	@printf "%s\n" "-----------------------"
	@cat *.cmsgs
	@#cat *.cmsgs | sed ''/error/s//`printf "\033[32error\033[0m"`/''
	@printf "%s" "------------------------------------------------------------"
	@printf "%s\n\n" "-------------------"
	@printf "Creating: $(OUTFILE)... "
	@$(CPP) -o $(OUTFILE) $(LINKFLAGS) $(OBJS) $(SYSLPATH) \
	$(LIBPATH) $(LIBS) $(SYSLIBS) 2> lmsgs;\
	RETVAL=$$?;\
	cat lmsgs | sed ''/error/s//`printf "\033[31merror\033[0m"`/'' > lmsgs;\
	cat lmsgs | sed ''/warning/s//`printf "\034[31mwarning\033[0m"`/'' > lmsgs;\
	if [[ $$RETVAL == 0 ]]; then\
		printf "[OK]\n";\
	else\
		printf "\n\n%s" "------------------------------------ ERROR --------";\
		printf "%s\n" "----------------------------";\
		cat lmsgs;\
		printf "%s" "-------------------------------------------------------";\
		printf "%s\n" "------------------------";\
		exit 1;\
	fi

%.o: %.cpp
	@printf "Compiling: $<... "
	@$(CPP) -MMD -MP -MF $*.d $(CPPFLAGS) $(SYSIPATH) $(INCPATH) \
	$< -o $@ 2> $*.cmsgs;\
	RETVAL=$$?;\
	cat $*.cmsgs | sed ''/error/s//`printf "\033[31merror\033[0m"`/'' > $*.cmsgs;\
	cat $*.cmsgs | sed ''/warning/s//`printf "\033[34mwarning\033[0m"`/'' > $*.cmsgs;\
	if [[ $$RETVAL == 0 ]]; then\
		printf "[OK]\n";\
	else\
		printf "\n\n%s" "--------------------------------- ERROR -----------";\
		printf "%s\n" "----------------------------";\
		cat $*.cmsgs;\
		printf "%s" "-------------------------------------------------------";\
		printf "%s\n\n" "------------------------";\
		exit 1;\
	fi

-include $(OBJSDIR)/*.d

endif

.PHONY: clean completeclean
clean:
	@printf "Removing files from: $(TARGETDIR)... "
	@rm -f $(TARGETDIR)/*
	@printf "[OK]\n"
	@printf "Removing files from: $(OBJSDIR)... "
	@rm -rf $(OBJSDIR)/*
	@printf "[OK]\n"

completeclean:
	@printf "Removing files from: $(TOPDIR)/Bin... "
	@rm -rf $(TOPDIR)/Bin
	@printf "[OK]\n"
	@printf "Removing files from: $(TOPDIR)/Obj... "
	@rm -rf $(TOPDIR)/Obj
	@printf "[OK]\n"

