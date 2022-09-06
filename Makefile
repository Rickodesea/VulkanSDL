##################################
#  VULKANSDL GENERATED MAKEFILE  #
#                                #
#  © ALL RIGHTS RESERVED         #
#  ALGODAL™                      #
#                                #
#  2022-09-05 09:21 PM           #
##################################

ARGS=
INC= -Isrc
DEP= -Lbin
GCC= gcc
CFLAGS= -Wall -g
EFLAGS= 
BFLAGS= 
FLAGS_SHARED= -fPIC
FLAGS_STATIC= 
LFLAGS_SHARED= --shared
LFLAGS_STATIC= -rcs
EXTERN_LIB_BIN= -lSDL2 -lvulkan -ldl
CLEAN_BIN= $(wildcard bin/*.*)
CLEAN_EXEC= $(wildcard bin/exec/*.*)
CLEAN_STATIC= $(wildcard bin/static/*.*)
CLEAN_SHARED= $(wildcard bin/shared/*.*)



########  BUILD  ########
build: bin/libvulkansdl.a
build-src: bin/libvulkansdl.a

#########  BIN  #########
bin/main.out: bin/exec/main.o bin/libvulkansdl.a | bin bin/exec bin/shared bin/static
	$(GCC) $(BFLAGS) $(CFLAGS) $(DEP) bin/exec/main.o -lvulkansdl $(EXTERN_LIB_BIN) `sdl2-config --libs` -o bin/main.out



###  STATIC LIBRARY  ###
bin/libvulkansdl.a: bin/static/renderer_01.o
	$(AR) $(LFLAGS_STATIC) -o bin/libvulkansdl.a bin/static/renderer_01.o



###  SHARED LIBRARY  ###
bin/libvulkansdl.so: bin/shared/renderer_01.o
	$(GCC) $(LFLAGS_SHARED) -o bin/libvulkansdl.so bin/shared/renderer_01.o



#####  EXEC OBJECT  #####
bin/exec/main.o: src/main.c src/renderer_01.h | bin bin/exec bin/shared bin/static
	gcc -c $(EFLAGS) $(CFLAGS) $(INC) src/main.c `sdl2-config --cflags` -o bin/exec/main.o



####  STATIC OBJECT  ####
bin/static/renderer_01.o: src/renderer_01.c src/renderer_01.h | bin bin/exec bin/shared bin/static
	gcc -c $(FLAGS_STATIC) $(CFLAGS) $(INC) src/renderer_01.c  -o bin/static/renderer_01.o



####  SHARED OBJECT  ####
bin/shared/renderer_01.o: src/renderer_01.c src/renderer_01.h | bin bin/exec bin/shared bin/static
	gcc -c $(FLAGS_SHARED) $(CFLAGS) $(INC) src/renderer_01.c  -o bin/shared/renderer_01.o



#########  RUN  #########
main: bin/main.out
	./bin/main.out $(ARGS)



########  CLEAN  ########
clean:
ifeq ($(CLEAN_BIN),)
else
	rm $(CLEAN_BIN)
endif
ifeq ($(CLEAN_EXEC),)
else
	rm $(CLEAN_EXEC)
endif
ifeq ($(CLEAN_STATIC),)
else
	rm $(CLEAN_STATIC)
endif
ifeq ($(CLEAN_SHARED),)
else
	rm $(CLEAN_SHARED)
endif



###  OUTPUT FOLDERS  ###
bin:
	mkdir -p bin

bin/exec:
	mkdir -p bin/exec

bin/shared:
	mkdir -p bin/shared

bin/static:
	mkdir -p bin/static

