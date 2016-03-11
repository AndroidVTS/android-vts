LOCAL_PATH := $(call my-dir)

################################
include $(CLEAR_VARS)

LOCAL_MODULE    := CVE-2013-6282
LOCAL_SRC_FILES := put_get_user_check.c

LOCAL_CFLAGS += -Ijni/include/

include $(BUILD_SHARED_LIBRARY)
################################

################################
include $(CLEAR_VARS)

LOCAL_MODULE    := cve-2013-6282check
LOCAL_SRC_FILES := put_get_user_check.c

LOCAL_CFLAGS += -Ijni/include/

include $(BUILD_EXECUTABLE)
################################



###############################
include $(CLEAR_VARS)

LOCAL_MODULE    := CVE-2014-4943
LOCAL_SRC_FILES := l2tp_exploit_check.c
LOCAL_C_INCLUDES := $(LOCAL_PATH)/include/

include $(BUILD_SHARED_LIBRARY)
################################


###############################
include $(CLEAR_VARS)

LOCAL_MODULE    := cve-2014-3153
LOCAL_SRC_FILES := futex_exploit_check.c
LOCAL_C_INCLUDES := $(LOCAL_PATH)/include/

LOCAL_CFLAGS += -Ijni/include/

include $(BUILD_SHARED_LIBRARY)
################################

################################
include $(CLEAR_VARS)

LOCAL_MODULE    := cve-2014-3153check
LOCAL_SRC_FILES := futex_exploit_check.c
LOCAL_C_INCLUDES := $(LOCAL_PATH)/include/


include $(BUILD_EXECUTABLE)
################################

################################
include $(CLEAR_VARS)

LOCAL_MODULE    := cve-2011-1149
LOCAL_SRC_FILES := ashmem_bug.c

include $(BUILD_SHARED_LIBRARY)
################################

################################
include $(CLEAR_VARS)

LOCAL_MODULE    := cve-2011-1149check
LOCAL_SRC_FILES := ashmem_bug.c

include $(BUILD_EXECUTABLE)
################################

################################
include $(CLEAR_VARS)

LOCAL_MODULE    := cve-2009-1185
LOCAL_SRC_FILES := exploid.c

include $(BUILD_SHARED_LIBRARY)
################################

################################
include $(CLEAR_VARS)

LOCAL_MODULE    := cve-2009-1185check
LOCAL_SRC_FILES := exploid.c

include $(BUILD_EXECUTABLE)
################################

################################
include $(CLEAR_VARS)

LOCAL_MODULE    := cve-2012-6422
LOCAL_SRC_FILES := exynos.c

include $(BUILD_SHARED_LIBRARY)
################################

################################
include $(CLEAR_VARS)

LOCAL_MODULE    := cve-2012-6422check
LOCAL_SRC_FILES := exynos.c

include $(BUILD_EXECUTABLE)
################################

################################
include $(CLEAR_VARS)

LOCAL_MODULE    := cve-2011-1350
LOCAL_SRC_FILES := levitator.c

include $(BUILD_SHARED_LIBRARY)
################################

################################
include $(CLEAR_VARS)

LOCAL_MODULE    := cve-2011-1350check
LOCAL_SRC_FILES := levitator.c

include $(BUILD_EXECUTABLE)
################################

################################
include $(CLEAR_VARS)

LOCAL_MODULE    := cve-2012-0056
LOCAL_SRC_FILES := mempodroid.c

include $(BUILD_SHARED_LIBRARY)
################################

################################
include $(CLEAR_VARS)

LOCAL_MODULE    := cve-2012-0056check
LOCAL_SRC_FILES := mempodroid.c

include $(BUILD_EXECUTABLE)
################################

################################
include $(CLEAR_VARS)

LOCAL_MODULE    := cve-2009-2692
LOCAL_SRC_FILES := wunderbar.c

include $(BUILD_SHARED_LIBRARY)
################################

################################
include $(CLEAR_VARS)

LOCAL_MODULE    := cve-2009-2692check
LOCAL_SRC_FILES := wunderbar.c

include $(BUILD_EXECUTABLE)
################################

################################
include $(CLEAR_VARS)

LOCAL_MODULE    := crashCheck
LOCAL_SRC_FILES := crash_check.c

include $(BUILD_EXECUTABLE)
################################

################################
include $(CLEAR_VARS)

LOCAL_MODULE    := crashCheck-pie
LOCAL_SRC_FILES := crash_check.c
LOCAL_CFLAGS    := -fpie -pie
LOCAL_LDFLAGS   := -pie

include $(BUILD_EXECUTABLE)
################################

################################
include $(CLEAR_VARS)

LOCAL_MODULE    := stagefright
LOCAL_SRC_FILES := stage_fright.c

include $(BUILD_SHARED_LIBRARY)
################################

################################
include $(CLEAR_VARS)

LOCAL_MODULE    := stagefrightCheck
LOCAL_SRC_FILES := stage_fright.c

include $(BUILD_EXECUTABLE)
################################

################################
include $(CLEAR_VARS)

LOCAL_MODULE    := stagefrightCheck-pie
LOCAL_SRC_FILES := stage_fright.c
LOCAL_CFLAGS    := -fpie -pie
LOCAL_LDFLAGS   := -pie

include $(BUILD_EXECUTABLE)
################################

###############################
include $(CLEAR_VARS)

LOCAL_MODULE    := x509serializationhelper
LOCAL_SRC_FILES := x509serializationhelper.c
LOCAL_C_INCLUDES := $(LOCAL_PATH)/include/

include $(BUILD_SHARED_LIBRARY)
################################

################################
include $(CLEAR_VARS)

LOCAL_MODULE    := pingpong
LOCAL_SRC_FILES := ping_pong.c
LOCAL_C_INCLUDES := $(LOCAL_PATH)/include/

include $(BUILD_SHARED_LIBRARY)
################################

################################
include $(CLEAR_VARS)

LOCAL_MODULE    := graphicsbufferoverflow
LOCAL_SRC_FILES := graphics_into_overflow_test.c
LOCAL_C_INCLUDES := $(LOCAL_PATH)/include/

include $(BUILD_SHARED_LIBRARY)
################################

################################
include $(CLEAR_VARS)

LOCAL_MODULE    := graphicsbufferoverflowcheck-pie
LOCAL_SRC_FILES := graphics_into_overflow_test.c
LOCAL_CFLAGS    := -fpie -pie
LOCAL_LDFLAGS   := -pie
LOCAL_C_INCLUDES := $(LOCAL_PATH)/include/

include $(BUILD_EXECUTABLE)
################################


################################
include $(CLEAR_VARS)

LOCAL_MODULE    := cve20151528
LOCAL_SRC_FILES := cve20151528.c
LOCAL_LDFLAGS   := -llog
LOCAL_C_INCLUDES := $(LOCAL_PATH)/include/

include $(BUILD_SHARED_LIBRARY)
################################

################################
include $(CLEAR_VARS)

LOCAL_MODULE    := cve20151528check
LOCAL_SRC_FILES := cve20151528.c
LOCAL_CFLAGS    := -fpie -pie
LOCAL_LDFLAGS   := -pie -llog
LOCAL_C_INCLUDES := $(LOCAL_PATH)/include/

include $(BUILD_EXECUTABLE)
################################

################################
include $(CLEAR_VARS)

LOCAL_MODULE    := pingpongCheck
LOCAL_SRC_FILES := ping_pong.c
LOCAL_CFLAGS    := -fpie -pie
LOCAL_C_INCLUDES := $(LOCAL_PATH)/include/

include $(BUILD_EXECUTABLE)
################################


################################
include $(CLEAR_VARS)

LOCAL_MODULE    := pingpongCheck-pie
LOCAL_SRC_FILES := ping_pong.c
LOCAL_CFLAGS    := -fpie -pie
LOCAL_LDFLAGS   := -pie
LOCAL_C_INCLUDES := $(LOCAL_PATH)/include/

include $(BUILD_EXECUTABLE)
################################

################################
include $(CLEAR_VARS)

LOCAL_MODULE    := libutilcheck-pie
LOCAL_SRC_FILES := libutils_bug.c
LOCAL_CFLAGS    := -fpie -pie
LOCAL_LDFLAGS   := -pie
LOCAL_C_INCLUDES := $(LOCAL_PATH)/include/

include $(BUILD_EXECUTABLE)
################################

################################
include $(CLEAR_VARS)

LOCAL_MODULE    := cve-2016-0808
LOCAL_SRC_FILES := cve20160808.c

include $(BUILD_SHARED_LIBRARY)
################################

################################
include $(CLEAR_VARS)

LOCAL_MODULE    := cve-2016-1818check
LOCAL_SRC_FILES := cve20160808.c

include $(BUILD_EXECUTABLE)
################################


