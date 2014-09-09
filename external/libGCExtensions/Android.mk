LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := gcextensions_static

LOCAL_MODULE_FILENAME := libgcextensions

LOCAL_CFLAGS := \
-DSQLITE_HAS_CODEC

LOCAL_SRC_FILES := \
wxSqlite3/sqlite3secure.c \

LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH) \
						   $(LOCAL_PATH)/wxSqlite3
						   
LOCAL_C_INCLUDES := $(LOCAL_PATH) \
					$(LOCAL_PATH)/wxSqlite3

include $(BUILD_STATIC_LIBRARY)
