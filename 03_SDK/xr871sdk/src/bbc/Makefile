#
# Rules for building library
#

# ----------------------------------------------------------------------------
# common rules
# ----------------------------------------------------------------------------
ROOT_PATH := ../..

include $(ROOT_PATH)/gcc.mk

# ----------------------------------------------------------------------------
# library and objects
# ----------------------------------------------------------------------------
LIBS := libbbc.a

DIRS := .

SRCS := $(basename $(foreach dir,$(DIRS),$(wildcard $(dir)/*.[csS])))

OBJS := $(addsuffix .o,$(SRCS))

CC_FLAGS += -Wno-error

# library make rules
include $(LIB_MAKE_RULES)
