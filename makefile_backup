PTHREAD_LIB=-lpthread
################# x64 GCC C ################
# C file extension name must be *.c
ifeq ($(ARCH),x64GCC)
	CC=gcc
	STRIP=strip
	CFLAGS=-O2 -lstdc++	
	LIBS=/usr/local/lib/

$(EXEC): $(EXEC).c
	$(CC) $(CFLAGS) -o $(EXEC)a $(EXEC).c $(LIBS)libmxio_x64.a $(PTHREAD_LIB)
	$(CC) $(CFLAGS) -o $(EXEC)so $(EXEC).c $(LIBS)libmxio_x64.so $(PTHREAD_LIB)
	$(STRIP) $(EXEC)a
	$(STRIP) $(EXEC)so
endif
################# x64 GCC CPP ################
# C file extension name must be *.c
ifeq ($(ARCH),x64GCCCPP)
	CC=gcc
	STRIP=strip
	CFLAGS=-O2 -lstdc++	
	LIBS=/usr/local/lib/

$(EXEC): $(EXEC).cpp
	$(CC) $(CFLAGS) -o $(EXEC)a $(EXEC).cpp $(LIBS)libmxio_x64.a $(PTHREAD_LIB)
	$(CC) $(CFLAGS) -o $(EXEC)so $(EXEC).cpp $(LIBS)libmxio_x64.so $(PTHREAD_LIB)
	$(STRIP) $(EXEC)a
	$(STRIP) $(EXEC)so
endif
################# x64 ################
ifeq ($(ARCH),x64)
	CC=g++
	STRIP=strip
	CFLAGS=-O2
	LIBS=/usr/local/lib/

$(EXEC): $(EXEC).cpp
	$(CC) $(CFLAGS) -o $(EXEC)a $(EXEC).cpp $(LIBS)libmxio_x64.a $(PTHREAD_LIB)
	$(CC) $(CFLAGS) -o $(EXEC)so $(EXEC).cpp $(LIBS)libmxio_x64.so $(PTHREAD_LIB)
	$(STRIP) $(EXEC)a
	$(STRIP) $(EXEC)so
endif
################# X86 GCC C ################
# C file extension name must be *.c
ifeq ($(ARCH),x86GCC)
	CC=gcc
	STRIP=strip
	CFLAGS=-O2 -lstdc++	
	LIBS=/usr/local/lib/

$(EXEC): $(EXEC).c
	$(CC) $(CFLAGS) -o $(EXEC)a $(EXEC).c $(LIBS)libmxio.a $(PTHREAD_LIB)
	$(CC) $(CFLAGS) -o $(EXEC)so $(EXEC).c $(LIBS)libmxio.so $(PTHREAD_LIB)
	$(STRIP) $(EXEC)a
	$(STRIP) $(EXEC)so
endif
################# X86 GCC CPP ################
# C file extension name must be *.c
ifeq ($(ARCH),x86GCCCPP)
	CC=gcc
	STRIP=strip
	CFLAGS=-O2 -lstdc++	
	LIBS=/usr/local/lib/

$(EXEC): $(EXEC).cpp
	$(CC) $(CFLAGS) -o $(EXEC)a $(EXEC).cpp $(LIBS)libmxio.a $(PTHREAD_LIB)
	$(CC) $(CFLAGS) -o $(EXEC)so $(EXEC).cpp $(LIBS)libmxio.so $(PTHREAD_LIB)
	$(STRIP) $(EXEC)a
	$(STRIP) $(EXEC)so
endif
################# X86 ################
ifeq ($(ARCH),x86)
	CC=g++
	STRIP=strip
	CFLAGS=-O2
	LIBS=/usr/local/lib/

$(EXEC): $(EXEC).cpp
	$(CC) $(CFLAGS) -o $(EXEC)a $(EXEC).cpp $(LIBS)libmxio.a $(PTHREAD_LIB)
	$(CC) $(CFLAGS) -o $(EXEC)so $(EXEC).cpp $(LIBS)libmxio.so $(PTHREAD_LIB)
	$(STRIP) $(EXEC)a
	$(STRIP) $(EXEC)so
endif

################UC-7110################
ifeq ($(ARCH),uc)
	CC=arm-elf-g++
	LDFLAGS=-Wl,-elf2flt="-z -r -s 262144"
	CFLAGS=-O2 -I/usr/local/arm-elf/include
	LIBS=/usr/local/arm-elf/lib/

$(EXEC): $(EXEC).cpp
	$(CC) $(LDFLAGS) $(CFLAGS) -o $(EXEC)a $(EXEC).cpp $(LIBS)libmxio.a $(PTHREAD_LIB)
endif

################UC-7408################
ifeq ($(ARCH),lx)
	CC=mxscaleb-g++
	STRIP=mxscaleb-strip
	CFLAGS=-O2 -I/usr/local/mxscaleb/include
	LIBS=/usr/local/mxscaleb/lib/

$(EXEC): $(EXEC).cpp
	$(CC) $(CFLAGS) -o $(EXEC)a $(EXEC).cpp $(LIBS)libmxio.a $(PTHREAD_LIB)
	$(CC) $(CFLAGS) -o $(EXEC)so $(EXEC).cpp $(LIBS)libmxio.so $(PTHREAD_LIB)
	$(STRIP) $(EXEC)a
	$(STRIP) $(EXEC)so
endif

################IA-240################
ifeq ($(ARCH),ia)
	CC=arm-linux-g++
	STRIP=arm-linux-strip
	CFLAGS=-O2 -I/usr/local/arm-linux/include -static
	LIBS=/usr/local/arm-linux/lib/

$(EXEC): $(EXEC).cpp
	$(CC) $(CFLAGS) -o $(EXEC)a $(EXEC).cpp $(LIBS)libmxio.a $(PTHREAD_LIB)
	$(CC) $(CFLAGS) -o $(EXEC)so $(EXEC).cpp $(LIBS)libmxio.so $(PTHREAD_LIB)
	$(STRIP) $(EXEC)a
	$(STRIP) $(EXEC)so
endif

################# DA-66X / UC7408 Plus #############
ifeq ($(ARCH),xs)
	CC=xscale_be-g++
	STRIP=xscale_be-strip
	CFLAGS=-O2 -I/usr/local/xscale_be/include
	LIBS=/usr/local/xscale_be/lib

$(EXEC): $(EXEC).cpp
	$(CC) $(CFLAGS) -o $(EXEC)a $(EXEC).cpp $(LIBS)libmxio.a $(PTHREAD_LIB)
	$(CC) $(CFLAGS) -o $(EXEC)so $(EXEC).cpp $(LIBS)libmxio.so $(PTHREAD_LIB)
	$(STRIP) $(EXEC)a
	$(STRIP) $(EXEC)so
endif

.PHONY : clean
clean :
	rm -rf $(EXEC)so $(EXEC)a  $(EXEC).o $(EXEC).gdb *.bak
