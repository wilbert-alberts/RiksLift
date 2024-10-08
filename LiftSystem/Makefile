# Path to dzn
DZN=dzn
DZNRUNTIME=./runtime/c++
DZNRUNTIMEOBJS=pump.o runtime.o std-async.o coroutine.o # thread-pool.o
DZN_SRCDEST = .

# Name of main executable
EXE=Lift.exe

# Specification of all manually written c++ sources

SRCS+= main.cpp
SRCS+= MQTT.cpp
SRCS+= MQTTBridge.cpp
SRCS+= MQTT_MoveUpDown.cpp
SRCS+= Foreign.cpp
SRCS+= FMoveUpDown.cpp
SRCS+= FMotor.cpp
SRCS+= FTimer.cpp
SRCS+= FPositionSensor.cpp
SRCS+= FPlanner.cpp
SRCS+= FEndstop.cpp
SRCS+= FDestinationSensor.cpp



# Specification of all dezyne models 

DZNS+= ForeignDestinationSensor.dzn
DZNS+= ForeignEndstop.dzn
DZNS+= ForeignLog.dzn
DZNS+= ForeignMotor.dzn
DZNS+= ForeignPlanner.dzn
DZNS+= ForeignPositionSensor.dzn
DZNS+= ForeignTimer.dzn
DZNS+= HomeDown.dzn
DZNS+= HomeUp.dzn
DZNS+= IDestinationSensor.dzn
DZNS+= IEndstop.dzn
DZNS+= ILog.dzn
DZNS+= IMotor.dzn
DZNS+= IMoveDown.dzn
DZNS+= IMoveUpDown.dzn
DZNS+= IMoveUpDownRelaxed.dzn
DZNS+= IMoveUp.dzn
DZNS+= IPlanner.dzn
DZNS+= IPositionSensor.dzn
DZNS+= ITimer.dzn
DZNS+= LiftSystem.dzn
DZNS+= MoveDown.dzn
DZNS+= MoveUpDown.dzn
DZNS+= MoveUpDownTopArmor.dzn
DZNS+= MoveUp.dzn
DZNS+= StatefullMoveDown.dzn
DZNS+= StatefullMoveUp.dzn
DZNS+= Types.dzn


# Determination of generated sources and includes
GENSRCS=$(DZNS:.dzn=.cc)
GENINCS=$(DZNS:.dzn=.hh)

# Determination  of all objects
OBJS=$(SRCS:.cpp=.o) $(GENSRCS:.cc=.o) $(DZNRUNTIMEOBJS)

# Use cpp to generate dependencies
#CPPFLAGS+=-MMD -Wall -g -c -include "Position.hh" -include "Delay.hh"
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


Types.cc: Types.hh	
	touch Types.cc

# Generate and adapt Types.hh in order to
# include c++ files defining the types
Types.hh: dzn/Types.dzn
	$(DZN) code -l c++ -o . dzn/Types.dzn
	echo '#include "Position.hh"' >> ./Types.hh
	echo '#include "Delay.hh"' >> ./Types.hh
	

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
