jam:
	gcc src/*.c -o bin/prog \
    -I include \
    -L lib \
    -lmingw32 \
    -lSDL2main \
    -lSDL2_mixer \
    -lSDL2_image \
    -lSDL2_ttf \
    -lSDL2 \
    -lwinmm

	cd bin && prog.exe

exec:
	cd bin && prog.exe

.PHONY: jam

