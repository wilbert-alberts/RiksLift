all:
#	+$(MAKE) -C LiftSystem
	+$(MAKE) -C MainSystem
	+$(MAKE) -C LiftSystem.esp/dzn

clean:
#	+$(MAKE) -C LiftSystem clean
	+$(MAKE) -C MainSystem clean
	+$(MAKE) -C LiftSystem.esp/dzn clean

