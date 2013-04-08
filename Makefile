CC = clang
LIBS = -lnetpbm -lm
CFLAGS = -std=c99 -Wall -Wno-unknown-pragmas -O2 -Iinclude/ -I/usr/include/netpbm

compile_stereobm_core_tb:
	$(CC) src/*.c test/stereobm_core_tb.c -o stereobm_core_tb $(CFLAGS) $(LIBS)

compile_stereobm_prefilter_tb:
	$(CC) src/*.c test/stereobm_prefilter_tb.c -o stereobm_prefilter_tb $(CFLAGS) $(LIBS)
