LOCAL_PATH:= $(call my-dir)

include $(CLEAR_VARS)
LOCAL_MODULE := mqueuetest
LOCAL_MODULE_TAGS := optional
LOCAL_PRELINK_MODULE := false
LOCAL_ARM_MODE := arm
LOCAL_CFLAGS += -fPIC
LOCAL_CFLAGS += -DNDEBUG

LOCAL_C_INCLUDES += $(LOCAL_PATH)/libmqueue/include

LOCAL_SRC_FILES:= \
   assert_np.c \
   test-posix.c

LOCAL_SHARED_LIBRARIES := libmqueue

include $(LOCAL_PATH)/common.mk
include $(BUILD_EXECUTABLE)
$(call import-module,libmqueue)
