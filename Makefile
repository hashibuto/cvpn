PHONEY: build

build:
	gcc -o cvpn *.c

run: build
	./cvpn serve -i tap22 -n 10.199.0.0/24