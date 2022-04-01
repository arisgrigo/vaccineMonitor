run:
	@echo "Compiling...";
	gcc -o vaccineMonitor main.c bloomFilter.c citizen.c skipList.c country.c queries.c

.PHONY: clean

clean:
	rm -f vaccineMonitor