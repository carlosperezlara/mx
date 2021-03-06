all:	libMX.so reconstruct recoMPC simqa raw_energy display

clean:
	rm libMX.so reconstruct simqa raw_energy display

reconstruct: reconstruct.cc
	`root-config --cxx` `root-config --cflags --libs` -L. -lMX $^ -o reconstruct

recoMPC: recoMPC.cc
	`root-config --cxx` `root-config --cflags --libs` -L. -lMX $^ -o recoMPC

simqa: simqa.cc
	`root-config --cxx` `root-config --cflags --libs` -L. -lMX $^ -o simqa

raw_energy: raw_energy.cc
	`root-config --cxx` `root-config --cflags --libs` -L. -lMX $^ -o raw_energy

display: display.cc
	`root-config --cxx` `root-config --cflags --libs` -L. -lMX $^ -o display

libMX.so: mxHit.o mxParty.o mxCoalition.o mxUnion.o mxGeometry.o mxReconstruction.o mxQAReconstruction.o mxMCParticle.o mxCoalitionCuts.o
	 `root-config --cxx` -shared -WL,-soname,libMX.so `root-config --cflags --libs` $^ -o libMX.so 
	rm $^

mxHit.o: mxHit.cxx
	`root-config --cxx` `root-config --cflags --libs` -c $^

mxParty.o: mxParty.cxx
	`root-config --cxx` `root-config --cflags --libs` -c $^

mxCoalition.o: mxCoalition.cxx
	`root-config --cxx` `root-config --cflags --libs` -c $^

mxCoalitionCuts.o: mxCoalitionCuts.cxx
	`root-config --cxx` `root-config --cflags --libs` -c $^

mxUnion.o: mxUnion.cxx
	`root-config --cxx` `root-config --cflags --libs` -c $^

mxReconstruction.o: mxReconstruction.cxx
	`root-config --cxx` `root-config --cflags --libs` -c $^

mxQAReconstruction.o: mxQAReconstruction.cxx
	`root-config --cxx` `root-config --cflags --libs` -c $^

mxGeometry.o: mxGeometry.cxx
	`root-config --cxx` `root-config --cflags --libs` -c $^

mxMCParticle.o: mxMCParticle.cxx
	`root-config --cxx` `root-config --cflags --libs` -c $^
