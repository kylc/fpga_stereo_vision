compile_stereobm_core_tb:
	gcc -std=c99 -Iinclude/ -I/usr/include/ -Wall -o stereobm_core_tb src/*.c test/stereobm_core_tb.c -lnetpbm -lm -O2

compile_stereobm_prefilter_tb:
	gcc -std=c99 -Iinclude/ -I/usr/include/ -Wall -o stereobm_prefilter_tb src/*.c test/stereobm_prefilter_tb.c -lnetpbm -lm
