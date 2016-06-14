LOCAL_PATH:=$(shell pwd)

iden_TOOL_PREFIX:=/vobs/jem/cee4_lsp/mobilinux/devkit/arm/v6_vfp_le/bin/arm_v6_vfp_le-

host_CC:=$(shell which gcc 2>/dev/null)
iden_CC:=${iden_TOOL_PREFIX}gcc
android_BUILD_TOOL:=$(shell which ndk-build 2>/dev/null)

bin_send_PRGM:=serialsend
bin_recv_PRGM:=serialrecv

.PHONY:android pc iden

all:android pc iden

pc:
	${host_CC} -g -O2 -c serialsend.c -D__HOST_PC_TEST__ -DDEBUG
	${host_CC} serialsend.o -o ${bin_send_PRGM}
	${host_CC} -g -O2 -c serialrecv.c -D__HOST_PC_TEST__ -DDEBUG
	${host_CC} serialrecv.o -o ${bin_recv_PRGM}

android:
	@if [ -z ${android_BUILD_TOOL} ] || [ ! -x ${android_BUILD_TOOL} ]; \
	then \
		echo -e "\033[32mNo ndk-build found\033[0m"; \
	else \
		ndk-build NDK_PROJECT_PATH=${LOCAL_PATH}/android APP_BUILD_SCRIPT=${LOCAL_PATH}/android/Android.mk; \
	fi

iden:
	@if [ ! -e ${iden_CC} ] || [ ! -x ${iden_CC} ]; \
	then \
		echo -e "\033[32mIden build tool not exists\033[0m"; \
	else \
		${iden_CC} -O2 -DIDEN_PLATFORM iden/serialsend.c -o iden/${bin_send_PRGM}; \
		${iden_TOOL_PREFIX}strip iden/${bin_send_PRGM}; \
		${iden_CC} -O2 -DIDEN_PLATFORM iden/serialrecv.c -o iden/${bin_recv_PRGM}; \
		${iden_TOOL_PREFIX}strip iden/${bin_recv_PRGM}; \
	fi

clean:
	@rm -vf ${bin_send_PRGM} serialsend.o ${bin_recv_PRGM} serialrecv.o null
	@if [ ! -z ${android_BUILD_TOOL} ] && [ -x ${android_BUILD_TOOL} ]; \
	then \
		ndk-build NDK_PROJECT_PATH=${LOCAL_PATH}/android APP_BUILD_SCRIPT=${LOCAL_PATH}/android/Android.mk clean; \
	fi
	@rm -rvf ${LOCAL_PATH}/android/obj ${LOCAL_PATH}/android/libs
	@rm -vf iden/${bin_send_PRGM} iden/${bin_recv_PRGM}

