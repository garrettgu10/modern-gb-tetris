moderntetris.gb: *.c *.h
	lcc -o moderntetris.gb *.c

run: moderntetris.gb
	java -cp ../pb/out/production/pb/ org.the429ers.gameboy.GameBoy moderntetris.gb

sameboy: moderntetris.gb
	open moderntetris.gb