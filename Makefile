tetris99.c: Makefile mod2gbt tetris99.mod
	./mod2gbt tetris99.mod tetris99 2
	mv output.c tetris99.c
	sed -i -e 's/bank=2/bank 2/g' ./tetris99.c
	rm tetris99.c-e

moderntetris.gb: *.c *.h *.s Makefile
	lcc -o moderntetris.gb -Wl-yt1 -Wl-yo4 -Wl-ya0 *.c *.s

run: moderntetris.gb
	java -cp ../pb/out/production/pb/ org.the429ers.gameboy.GameBoy moderntetris.gb

sameboy: moderntetris.gb
	open moderntetris.gb