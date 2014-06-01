list= exp               fonctionsimple
all: dirs files

files:$(list)
	echo "on a fini $(list)"
dirs:
	cd lg;$(MAKE)
	cd lgfem;$(MAKE)
clean:
	-rm $(list) *.o *~
	cd lg;$(MAKE) clean
	cd lgfem;$(MAKE) clean
