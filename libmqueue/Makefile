PHONIES=install build all clean uninstall

ifneq (${MAKELEVEL},0)

LOCAL_MODULE := libmqueue
LOCAL_SRC_FILES := \
   mqueue.c \


LOCAL_LIBS := \
   rt \
   pthread

LOCAL_CFLAGS := -fPIC
#LOCAL_CFLAGS += -DNDEBUG
LOCAL_C_INCLUDES += ${HOME}/include
LOCAL_LDLIBS += ${HOME}/lib
LOCAL_SUBMODULES :=

.PHONY: ${PHONIES} ${LOCAL_SUBMODULES}

#--- Devs settings ends here. Rest is generic ---
WHOAMI := $(shell whoami)
ifeq (${WHOAMI},root)
    INSTALLDIR=/usr/local
else
    INSTALLDIR=${HOME}
endif
EXHEADERS=$(shell ls include/*.h)
LOCAL_C_INCLUDES := $(addprefix -I , ${LOCAL_C_INCLUDES})
LOCAL_LDLIBS:= $(addprefix -L, ${LOCAL_LDLIBS})
LOCAL_LIBS:= ${LOCAL_LDLIBS} $(addprefix -l, ${LOCAL_LIBS})
LOCAL_AR_LIBS:= $(addprefix ${LOCAL_LDLIBS}/lib, ${LOCAL_AR_LIBS})
LOCAL_AR_LIBS:= $(addsuffix .a, ${LOCAL_AR_LIBS})
CFLAGS += -O0 -g3 ${LOCAL_CFLAGS}
CFLAGS += -I ./include -I ../ ${LOCAL_C_INCLUDES}
CLEAN_MODS := $(patsubst %, $(MAKE) clean -C %;,$(LOCAL_SUBMODULES))
UNINST_MODS := $(patsubst %, $(MAKE) uninstall -C %;,$(LOCAL_SUBMODULES))
RINFO := echo
.INTERMEDIATE: ${LOCAL_SRC_FILES:.c=.d}

ifdef LIB_DYNAMIC
    $(info Build will build and use it's libraries: dynamic )
    $(info To build/use static libs: unset LIB_DYNAMIC )
    LOCAL_MODULE := $(addsuffix .so, ${LOCAL_MODULE})
    MODULE_FLAGS := -shared
    EXLIBS=$(shell ls lib/*.so)
else
    $(info Build will build and use it's libraries: static )
    $(info To build all dynamic: export LIB_DYNAMIC='y' )
    LOCAL_MODULE := $(addsuffix .a, ${LOCAL_MODULE})
    MODULE_FLAGS := -static
    EXLIBS=$(shell ls lib/*.a)
endif

LOCAL_MODULE := lib/${LOCAL_MODULE}
build: tags $(LOCAL_MODULE)
all: install

${LOCAL_SUBMODULES}:
	$(MAKE) -e -k -C $@ install

clean:
	$(CLEAN_MODS)
	rm -f *.o
	rm -f *.tmp
	rm -f *.d
	rm -f $(LOCAL_MODULE)
	rm -f tags

install: ${INSTALLDIR}/${LOCAL_MODULE} ${INSTALLDIR}/${EXHEADERS}

uninstall:
	$(UNINST_MODS)
	rm -rf ${INSTALLDIR}/${LOCAL_MODULE}
	bash -c '${INSTALLDIR}; rm ${EXHEADERS}'

${INSTALLDIR}/${LOCAL_MODULE}: ${EXLIBS} ${LOCAL_MODULE}
	mkdir -p ${INSTALLDIR}/lib
	rm -f ${INSTALLDIR}/${LOCAL_MODULE}
	cp $(LOCAL_MODULE) ${INSTALLDIR}/${LOCAL_MODULE}

${INSTALLDIR}/${EXHEADERS}: ${EXHEADERS}
	mkdir -p $(shell dirname ${@})
	rm -f ${@}
	cp include/$(shell basename ${@}) ${@}

tags: $(shell ls *.[ch])
	ctags --options=.cpatterns --exclude=@.cexclude -o tags -R *


$(LOCAL_MODULE): Makefile $(LOCAL_SRC_FILES:c=o)
	rm -f $(LOCAL_MODULE)
ifdef LIB_DYNAMIC
	gcc $(CFLAGS) $(MODULE_FLAGS) $(LOCAL_SRC_FILES:c=o) ${LOCAL_LIBS} -o ${@}
else
	ar rcs ${@} ${LOCAL_AR_LIBS} $(LOCAL_SRC_FILES:c=o)
endif
	@echo ">>>> Build $(LOCAL_MODULE) success! <<<<"

#Cancel out built-in implicit rule
%.o: %.c

%.tmp: %.c Makefile
	gcc -MM $(CFLAGS) ${@:tmp=c} > ${@}

%.d: %.tmp
	cat ${@:d=tmp} | sed  -E 's/$*.c/$*.c $@/' > ${@}

%.o: %.d Makefile
	gcc -c $(CFLAGS) ${@:o=c} -o $@


include $(FOUND_CDEPS)

#========================================================================
else #First recursion level only executes this. Used for colorized output
#========================================================================

.PHONY: ${PHONIES} ${LOCAL_SUBMODULES}

ifeq ($(VIM),)
     HAS_GRCAT_BIN := $(shell which grcat)
endif

ifeq ($(HAS_GRCAT_BIN),)
    ifeq ($(VIM),)
        $(info No grcat installed. Build will not be colorized.)
    endif
    HAS_GRCAT=no
else
    HAS_GRCAT := $(shell if [ -f ~/.grc/conf.gcc ]; then echo "yes"; else echo "no"; fi)
    ifeq ($(HAS_GRCAT), no)
        $(warning NOTE: You have grcat installed, but no configuration file in for it (~/.grc/conf.gcc))
    endif
endif

${PHONIES} $(patsubst %.c,%.o,$(wildcard *.c)) tags:
ifeq ($(HAS_GRCAT), yes)
	( $(MAKE) $(MFLAGS) -e -C . $@ 2>&1 ) | grcat conf.gcc
else
	$(MAKE) $(MFLAGS) -e -C . $@
endif

endif
