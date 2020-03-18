build: clean
	cmake -B build
	make -C build

update:
	make -C build

run:
	./bin/CacheSim -i ./data/fortnite60fps_b620_2.txt

clean:
	rm -rf build bin result
