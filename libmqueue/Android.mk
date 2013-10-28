LOCAL_PATH:= $(call my-dir)

include $(CLEAR_VARS)
LOCAL_MODULE := libmqueue
LOCAL_MODULE_TAGS := optional
LOCAL_PRELINK_MODULE := false
LOCAL_ARM_MODE := arm

LOCAL_C_INCLUDES += $(LOCAL_PATH)/include $(LOCAL_PATH)/..

LOCAL_CFLAGS += -fPIC -DHAVE_ANDROID_OS
LOCAL_CFLAGS += -g3 -O0

#Enable when code finalized
#LOCAL_CFLAGS += -DNDEBUG

#Enable only to check if a new target supports CTORS/DTORS
#mechanism (typically only once)
#LOCAL_CFLAGS += -DINITFINI_SHOW

LOCAL_SRC_FILES := \
   mqueue.c

LOCAL_SHARED_LIBRARIES :=

LOCAL_LDLIBS += -lm

ifdef LIB_DYNAMIC
include $(BUILD_SHARED_LIBRARY)
else
include $(BUILD_STATIC_LIBRARY)
endif
