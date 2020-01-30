JOB_NO ?= $(shell nproc)
BUILD_DIR=build
SRC_DIR=.
INSTALL_PREFIX=install
CMAKE_CONFIG=-DCMAKE_INSTALL_PREFIX=$(INSTALL_PREFIX) -DCMAKE_BUILD_TYPE=Release -DCMAKE_TOOLCHAIN_FILE=cmake/toolchain-arm64-qnx.cmake

all: staging

${BUILD_DIR}/Makefile:
	mkdir -p ${BUILD_DIR}
	-cd ${BUILD_DIR} && cmake --verbose=1 -DCMAKE_VERBOSE_MAKEFILE:BOOL=ON ../${SRC_DIR} ${CMAKE_CONFIG}

.PHONY: staging
staging: ${BUILD_DIR}/Makefile
	$(MAKE) --no-print-directory -C ${BUILD_DIR} --jobs $(JOB_NO)

.PHONY: test
test: ${BUILD_DIR}/Makefile
	$(MAKE) --no-print-directory -C ${BUILD_DIR} test

.PHONY: install
install: ${BUILD_DIR}/Makefile
	rm -rf ${INSTALL_PREFIX}/*
	$(MAKE) --no-print-directory -C ${BUILD_DIR} --jobs $(JOB_NO) install

.PHONY: clean
clean:
	-rm -rf ${BUILD_DIR}
