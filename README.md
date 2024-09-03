# Riks Lift

This repository provides three projects:
- MainSystem: a main controller intended to be deployed on a Raspberry Pi
- LiftSystem: a temporary version of the LiftSystem that can be build on a Raspberry PI but should not be deployed there.
- LiftSystem.esp: a controller that translates movement commands of the MainSystem into corresponding actions for the LiftMotor.

The MainSystem and the LiftSystem exchange data using MQTT. Common definitions like topics and specific 
messages are shared between the MainSystem and LiftSystem(.esp). The files are defined in the MainSystem 
and are visible in the LiftSystems via symbolic links.
MainSystem and Liftsystem(.esp) are based on a state machine based design which has been developed using 
Dezyne. Most of the development of the state model has been done in the LiftSystem. They have been 
replicated into LiftSystem.esp via symbolic links.

The Makefile in the root of the repository can be used to build or clean (make resp. make clean) almost 
all three projects. It completely builds/cleans MainSystem and LiftSystem. However, LiftSystem.esp is a 
platformio project and can/should be built using vscode. However, LiftSystem.esp also uses Dezyne. The 
Dezyne (*.dzn) files are present in LiftSystem.esp/dzn. This directory also contains a Makefile and 
this Makefile can be used to generate (or clean) the files generated using Dezyne.
