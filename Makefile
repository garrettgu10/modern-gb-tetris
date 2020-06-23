moderntetris.gb: *.c *.h *.s Makefile
	lcc -o moderntetris.gb -Wl-yt1 -Wl-yo4 -Wl-ya0 *.c *.s

run: moderntetris.gb
	java -cp ../pb/out/production/pb/ org.the429ers.gameboy.GameBoy moderntetris.gb

sameboy: moderntetris.gb
	open moderntetris.gb