#
# Rules for building application
#

# ----------------------------------------------------------------------------
# project local config
# ----------------------------------------------------------------------------
include localconfig.mk

# ----------------------------------------------------------------------------
# common rules
# ----------------------------------------------------------------------------
ROOT_PATH := ../../..

include $(ROOT_PATH)/gcc.mk

# ----------------------------------------------------------------------------
# project and objects
# ----------------------------------------------------------------------------
PROJECT := evb_audio

INCLUDE_PATHS += -I$(ROOT_PATH)/project/$(PROJECT)

DIRS_IGNORE := ../gcc% ../image% $(ROOT_PATH)/project/common/board/%
DIRS_ALL := $(shell find .. $(ROOT_PATH)/project/common -type d)
DIRS := $(filter-out $(DIRS_IGNORE),$(DIRS_ALL))
DIRS += $(ROOT_PATH)/project/common/board/$(__PRJ_CONFIG_BOARD)

SRCS := $(basename $(foreach dir,$(DIRS),$(wildcard $(dir)/*.[csS])))

OBJS := $(addsuffix .o,$(SRCS))

# extra libs
# PRJ_EXTRA_LIBS :=

# ----------------------------------------------------------------------------
# override project variables
# ----------------------------------------------------------------------------
LINKER_SCRIPT_PATH := .
LINKER_SCRIPT := ./xr871_appos_xip.ld
IMAGE_CFG_PATH := .
# IMAGE_CFG := ./image_xip.cfg
# IMAGE_NAME :=

# project make rules
include $(PRJ_MAKE_RULES)
