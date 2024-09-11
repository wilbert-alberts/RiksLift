# Path to dzn
DZN=dzn
DZNRUNTIME=runtime/c++
DZNRUNTIMEOBJS=pump.o runtime.o std-async.o coroutine.o # thread-pool.o
DZN_SRCDEST = .

# Name of main executable

EXE=Main.exe

# Specification of all dezyne models 

SRCS+= main.cpp
SRCS+= MQTT.cpp
SRCS+= MQTTBridge.cpp
SRCS+= MQTT_IntrusionDetection.cpp
SRCS+= MQTT_Button.cpp
SRCS+= MQTT_MoveUpDown.cpp
SRCS+= FIntrusionDetection.cpp
SRCS+= FMoveUpDown.cpp
SRCS+= FButton.cpp
SRCS+= Foreign.cpp
SRCS+= ForeignWithLocation.cpp


# Specification of all dezyne models 

DZNS+= Application.dzn
DZNS+= ForeignIntrusionDetection.dzn
DZNS+= ForeignLog.dzn
DZNS+= ForeignMoveUpDown.dzn
DZNS+= IApplication.dzn
DZNS+= IIntrusionDetection.dzn
DZNS+= IIntrusionDetectionRelaxed.dzn
DZNS+= ILog.dzn
DZNS+= IMoveUpDown.dzn
DZNS+= IMoveUpDownRelaxed.dzn
DZNS+= IntrusionDetection.dzn
DZNS+= MoveUpDownBottomArmor.dzn
DZNS+= Safety.dzn
DZNS+= SafetySystem.dzn
DZNS+= System.dzn

# Determination of generated sources and includes
GENSRCS=$(DZNS:.dzn=.cc)
GENINCS=$(DZNS:.dzn=.hh)

# Determination  of all objects
OBJS=$(SRCS:.cpp=.o) $(GENSRCS:.cc=.o) $(DZNRUNTIMEOBJS)

# Use cpp to generate dependencies
CPPFLAGS+=-MMD -Wall -g -c

# Libraries
LIBS=-lpaho-mqttpp3 -lpaho-mqtt3c
INCS=-I . -I $(DZNRUNTIME) -I $(SRCDIR)

all: $(EXE)

dzn: $(GENSRCS) $(GENINCS)

$(EXE): $(OBJS)
	g++ -o $(EXE) $(OBJS) $(LIBS)

info:
	@echo DZNS: $(DZNS)
	@echo GENSRCS: $(GENSRCS)
	@echo GENINCS: $(GENINCS)
	@echo SRCS: $(SRCS)
	@echo OBJS: $(OBJS)

clean:
	rm -f *.d *.o $(EXE) $(GENSRCS) $(GENINCS) 

coroutine.o: $(DZNRUNTIME)/coroutine.cc
	g++ $(CPPFLAGS) -c -I $(DZNRUNTIME) $<

pump.o: $(DZNRUNTIME)/pump.cc
	g++ $(CPPFLAGS) -c -I $(DZNRUNTIME) $<

runtime.o: $(DZNRUNTIME)/runtime.cc
	g++ $(CPPFLAGS) -c -I $(DZNRUNTIME) $<

std-async.o: $(DZNRUNTIME)/std-async.cc
	g++ $(CPPFLAGS) -c -I $(DZNRUNTIME) $<

thread-pool.o: $(DZNRUNTIME)/thread-pool.cc
	g++ $(CPPFLAGS) -c -I $(DZNRUNTIME) $<

main.cpp: $(GENINCS)


%.o: %.cpp
	g++ $(CPPFLAGS) $(INCS) -c $<

%.o: %.cc
	g++ $(CPPFLAGS) $(INCS) -c $<

%.hh: dzn/%.dzn
	$(DZN) code -l c++ -o . $<

%.cc: %.hh
	# $(DZN) code -l c++ -o . $<

.PRECIOUS: $(GENSRCS) $(GENINCS)

-include $(SRCS:%.cpp=%.d)
-include $(GENSRCS:%.cc=%.d)
