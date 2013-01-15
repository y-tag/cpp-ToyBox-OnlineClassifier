SRCTOP = ..

include $(SRCTOP)/Makefile.defs

CXXSRC = cpp_unit_test_runner.cc \
         perceptron_test.cc \
         passive_aggressive_test.cc \
         confidence_weighted_test.cc \
         arow_test.cc \
         scw_test.cc \
         svmlight_reader_test.cc \
         simple_text_serializer_test.cc \

INCLUDES += -I../lib \

LIBS += -L../lib \
        -Wl,-rpath,../lib \
        -lonline \
        -lcppunit \

include $(SRCTOP)/Makefile.rules

test:$(CXXOBJ)
	$(CXX) -Wall $^ -o CppUnitTestRunner $(INCLUDES) $(LIBS) $(CXXFLAG)
	./CppUnitTestRunner

clean:
	rm -f *~ *.o *.out CppUnitTestRunner
