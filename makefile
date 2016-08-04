all:	libMX.so standalone simqa display

clean:
	rm libMX.so standalone simqa

standalone: standalone.cc
	`root-config --cxx` `root-config --cflags --libs` -L. -lMX $^ -o standalone

simqa: simqa.cc
	`root-config --cxx` `root-config --cflags --libs` -L. -lMX $^ -o simqa

display: display.cc
	`root-config --cxx` `root-config --cflags --libs` -L. -lMX $^ -o display

libMX.so: mxHit.o mxParty.o mxCoalition.o mxUnion.o mxGeometry.o mxReconstruction.o mxQAReconstruction.o
	 `root-config --cxx` -shared -WL,-soname,libMX.so `root-config --cflags --libs` $^ -o libMX.so 
	rm $^

mxHit.o: mxHit.cxx
	`root-config --cxx` `root-config --cflags --libs` -c $^

mxParty.o: mxParty.cxx
	`root-config --cxx` `root-config --cflags --libs` -c $^

mxCoalition.o: mxCoalition.cxx
	`root-config --cxx` `root-config --cflags --libs` -c $^

mxUnion.o: mxUnion.cxx
	`root-config --cxx` `root-config --cflags --libs` -c $^

mxReconstruction.o: mxReconstruction.cxx
	`root-config --cxx` `root-config --cflags --libs` -c $^

mxQAReconstruction.o: mxQAReconstruction.cxx
	`root-config --cxx` `root-config --cflags --libs` -c $^

mxGeometry.o: mxGeometry.cxx
	`root-config --cxx` `root-config --cflags --libs` -c $^
