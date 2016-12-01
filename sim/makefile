all:	readsim

clean:
	rm readsim

readsim: readsim.cc
	`root-config --cxx` `root-config --cflags --libs` -L. $^ -o readsim
