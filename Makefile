build: main.c
	gcc main.c include/dynstr.c -o barchen -I include/ -L lib -lraylib -lopengl32 -lgdi32 -lwinmm
