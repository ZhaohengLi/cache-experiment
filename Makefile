build: clean
	cmake -B build
	make -C build

update:
	make -C build

run:
	./bin/CacheSim -i ./data/example.txt

clean:
	rm -rf build bin result
