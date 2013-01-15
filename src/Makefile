SRCTOP = ..

include $(SRCTOP)/Makefile.defs

INCLUDES += -I../lib \

LIBS += -L../lib \
        -Wl,-rpath,../lib \
        -lonline \

include $(SRCTOP)/Makefile.rules

all:eval-online

eval-online:eval_online.cc
	$(CXX) -o $@ $^ $(CFLAG) $(INCLUDES) $(LIBS)

clean:
	rm -f *~ *.o eval-online

