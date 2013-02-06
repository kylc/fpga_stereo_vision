LIBS = -lnetpbm -lm
CFLAGS = -std=c99 -Wall -O2 -Iinclude/

compile_stereobm_core_tb:
	gcc src/*.c test/stereobm_core_tb.c -o stereobm_core_tb $(CFLAGS) $(LIBS)

compile_stereobm_prefilter_tb:
	gcc src/*.c test/stereobm_prefilter_tb.c -o stereobm_prefilter_tb $(CFLAGS) $(LIBS)
