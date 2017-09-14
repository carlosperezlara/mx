all:	libMXR.so

clean:
	rm libMXR.so

libMXR.so: mxHit.o mxCluster.o mxCandidate.o mxDecayCandidate.o mxGeometry.o mxReconstruction.o mxCandidateCuts.o
	 `root-config --cxx` -shared -WL,-soname,libMXR.so `root-config --cflags --libs` $^ -o libMXR.so 
	rm $^

mxHit.o: mxHit.cxx
	`root-config --cxx` `root-config --cflags --libs` -c $^

mxCluster.o: mxCluster.cxx
	`root-config --cxx` `root-config --cflags --libs` -c $^

mxCandidate.o: mxCandidate.cxx
	`root-config --cxx` `root-config --cflags --libs` -c $^

mxCandidateCuts.o: mxCandidateCuts.cxx
	`root-config --cxx` `root-config --cflags --libs` -c $^

mxDecayCandidate.o: mxDecayCandidate.cxx
	`root-config --cxx` `root-config --cflags --libs` -c $^

mxReconstruction.o: mxReconstruction.cxx
	`root-config --cxx` `root-config --cflags --libs` -c $^

mxGeometry.o: mxGeometry.cxx
	`root-config --cxx` `root-config --cflags --libs` -c $^
