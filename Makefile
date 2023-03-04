build:
	gcc tema1.c -g -o tema

clean:
	rm tema

test:
	gcc tema1.c -g -o tema
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes ./tema INPUTS/mkdir


checker:
	python3 checker.py
