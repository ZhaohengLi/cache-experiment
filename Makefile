build: clean
	cmake -B build
	make -C build

update:
	make -C build

run: build
	./bin/CacheSim -i ./data/wangzherongyao60fps_b620_1.txt > ./result/wangzherongyao60fps_b620_1.txt

clean:
	rm -rf build bin result
