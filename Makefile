all:
	+$(MAKE) -C LiftSystem
	+$(MAKE) -C MainSystem

clean:
	+$(MAKE) -C LiftSystem clean
	+$(MAKE) -C MainSystem clean

