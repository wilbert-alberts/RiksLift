DZN = ../../../../dezyne-2.18.2/dzn

OUTDIR = ../src

DZNS += ForeignDestinationSensor.dzn
DZNS += ForeignEndstop.dzn
DZNS += ForeignLog.dzn
DZNS += ForeignMotor.dzn
DZNS += ForeignPlanner.dzn
DZNS += ForeignPositionSensor.dzn
DZNS += ForeignTimer.dzn
DZNS += HomeDown.dzn
DZNS += HomeUp.dzn
DZNS += IDestinationSensor.dzn
DZNS += IEndstop.dzn
DZNS += ILog.dzn
DZNS += IMotor.dzn
DZNS += IMoveDown.dzn
DZNS += IMoveUpDown.dzn
DZNS += IMoveUpDownRelaxed.dzn
DZNS += IMoveUp.dzn
DZNS += IPlanner.dzn
DZNS += IPositionSensor.dzn
DZNS += ITimer.dzn
DZNS += LiftSystem.dzn
DZNS += MoveDown.dzn
DZNS += MoveUpDown.dzn
DZNS += MoveUpDownTopArmor.dzn
DZNS += MoveUp.dzn
DZNS += StatefullMoveDown.dzn
DZNS += StatefullMoveUp.dzn

GENSRC_C = $(subst .dzn,.cc,$(DZNS))
GENSRC_H = $(subst .dzn,.hh,$(DZNS))

generate: $(GENSRC_C) $(GENSRC_H)
	

clean:
	cd $(OUTDIR) && rm -f $(GENSRC_C) $(GENSRC_H)

info:
	echo DZNS: $(DZNS)
	echo GENSRC_H $(GENSRC_H)
	echo GENSRC_H $(GENSRC_H)

%.cc: %.dzn
	$(DZN) verify $<
	$(DZN) code -l c++ -o $(OUTDIR) $<

%.hh: %.dzn
	$(DZN) code -l c++ -o $(OUTDIR) $<
