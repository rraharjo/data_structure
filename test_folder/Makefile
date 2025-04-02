COMP = g++
INCLFLAG = -I /home/raharjo/project/data_structure/header

SRC = /home/raharjo/project/data_structure/source

BNODE = bnode
BTREE = btree

bnode:
	$(COMP) -g BTree/bnode.cpp $(INCLFLAG) -o $(BNODE).exe

clean_bnode:
	rm $(BNODE).exe

btree:
	$(COMP) -g BTree/btree.cpp $(INCLFLAG) -o $(BTREE).exe

clean_btree:
	rm $(BTREE).exe

clean:
	make clean_bnode clean_btree