
LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_ARM_MODE := arm
ifeq ($(BUILD_WITH_NEON),1)
LOCAL_ARM_NEON := true
endif

LOCAL_MODULE := iconv

LOCAL_CFLAGS += \
    -DHAVE_CONFIG_H \
    -DBUILDING_LIBICONV \
    -DIN_LIBRARY     
 #  -DLIBICONV_PLUG

LOCAL_C_INCLUDES += \
    $(LOCAL_PATH)/include \
	$(LOCAL_PATH)/libcharset \
	$(LOCAL_PATH)/lib \
    $(LOCAL_PATH)/libcharset/include \
	$(LOCAL_PATH)/srclib

LOCAL_SRC_FILES := \
    lib/iconv.c \
	libcharset/lib/localcharset.c \
	lib/relocatable.c

LOCAL_STATIC_LIBRARIES += charset

include $(BUILD_STATIC_LIBRARY)

include $(CLEAR_VARS)

include $(call all-makefiles-under,$(LOCAL_PATH))

