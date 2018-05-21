cmake_minimum_required (VERSION 2.6)
project (libmqueue C)

include_directories("${PROJECT_BINARY_DIR}")
include_directories("${PROJECT_SOURCE_DIR}")

set(MQUEUE_SOURCE
	mqueue.c
)

if (ENABLE_MQUEUE)
	set(MQUEUE_SOURCE
		${MQUEUE_SOURCE}
		initfini.c
	)
	set(CMAKE_MODULE_LINKER_FLAGS "${CMAKE_MODULE_LINKER_FLAGS} -Wl,--undefined=__libmqueue_init")
	set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Wl,--undefined=__libmqueue_init")
endif (ENABLE_MQUEUE)

set(MQUEUE_SOURCE
	${MQUEUE_SOURCE}
)

configure_file (
	"${PROJECT_SOURCE_DIR}/config.h.in"
	"${PROJECT_BINARY_DIR}/config.h"
)

add_library(libmqueue ${MQUEUE_SOURCE})
