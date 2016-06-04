all:	libMX.so standalone display

clean:
	rm libMX.so standalone

standalone: standalone.cc
	`root-config --cxx` `root-config --cflags --libs` -L. -lMX $^ -o standalone

display: display.cc
	`root-config --cxx` `root-config --cflags --libs` -L. -lMX $^ -o display

libMX.so: mxHit.o mxParty.o mxCoalition.o mxGeometry.o mxReconstruction.o
	 `root-config --cxx` -shared -WL,-soname,libMX.so `root-config --cflags --libs` $^ -o libMX.so 
	rm $^

mxHit.o: mxHit.cxx
	`root-config --cxx` `root-config --cflags --libs` -c $^

mxParty.o: mxParty.cxx
	`root-config --cxx` `root-config --cflags --libs` -c $^

mxCoalition.o: mxCoalition.cxx
	`root-config --cxx` `root-config --cflags --libs` -c $^

mxReconstruction.o: mxReconstruction.cxx
	`root-config --cxx` `root-config --cflags --libs` -c $^

mxGeometry.o: mxGeometry.cxx
	`root-config --cxx` `root-config --cflags --libs` -c $^

