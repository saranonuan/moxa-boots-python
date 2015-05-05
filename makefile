INC=-I/usr/local/include
PYTHON = /usr/include/python2.6
BOOST_INC=/usr/include/boost
BOOST_LIB=/usr/lib
MIXIO_LIBS=/usr/local/lib/libmxio_x64.a

moxa.so: moxa.o
	g++ -shared -Wl,--export-dynamic \
	moxa.o $(MIXIO_LIBS) \
	-L$(BOOST_LIB) -lboost_python \
	-L/usr/lib/python2.6/config -lpython2.6 \
	-o moxa.so
	sudo cp moxa.so /usr/lib/python2.6/lib-dynload/
    
moxa.o:
	g++ -I$(PYTHON) -I$(BOOST_INC) $(INC) -fPIC -c moxa.cpp 

clean: moxa.so
	rm *.so *.o
