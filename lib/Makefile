SRCTOP = ..

include $(SRCTOP)/Makefile.defs

CXXSRC = perceptron.cc \
         passive_aggressive.cc \
         confidence_weighted.cc \
         arow.cc \
         scw.cc \
         svmlight_reader.cc \
         simple_text_serializer.cc \
         simple_text_deserializer.cc \

CXXFLAG += -fPIC -Wall

include $(SRCTOP)/Makefile.rules

lib:$(CXXOBJ)
	$(CXX) -shared -o libonline.so $^ $(CXXFLAG)

clean:
	rm -f *~ *.o *.out *.so
