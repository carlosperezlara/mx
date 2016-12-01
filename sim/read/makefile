all:	readsim mergeSN

clean:
	rm readsim mergeSN

readsim: readsim.cc
	`root-config --cxx` `root-config --cflags --libs` -L. $^ -o readsim

mergeSN: mergeSN.cc
	`root-config --cxx` `root-config --cflags --libs` -L. $^ -o mergeSN
