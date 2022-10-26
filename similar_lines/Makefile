CC       = gcc
CPPFLAGS =
CFLAGS   = -std=c11 -Wall -Wextra -O2
LDFLAGS  =

.PHONY: all clean

all: bin/similar_lines

bin/similar_lines: 								\
	bin/main.o									\
	bin/comparators.o 							\
	bin/memory_allocation.o						\
	bin/lines_processing.o 						\
	bin/output_recovery.o 						\
	bin/char_functions.o						\
	bin/line_formatting.o						\
	bin/line_parsing.o							\
	bin/word_parsing.o
	$(CC) $(LDFLAGS) $^ -o $@

bin/main.o 										\
bin/comparators.o								\
bin/memory_allocation.o							\
bin/lines_processing.o							\
bin/output_recovery.o							\
bin/char_functions.o							\
bin/line_formatting.o							\
bin/line_parsing.o								\
bin/word_parsing.o:
	$(CC) $(CPPFLAGS) $(CFLAGS) -I include -c $< -o $@


bin/main.o: 									\
	src/main.c									\
		include/structures.h					\
		include/memory_allocation.h				\
		include/line_formatting.h				\
		include/line_parsing.h					\
		include/lines_processing.h				\
		include/output_recovery.h

bin/memory_allocation.o: 						\
	src/memory_allocation.c						\
		include/memory_allocation.h

bin/comparators.o:								\
	src/comparators.c							\
		include/comparators.h					\
		include/structures.h

bin/char_functions.o:							\
	src/char_functions.c						\
		include/char_functions.h

bin/line_formatting.o:							\
	src/line_formatting.c						\
		include/line_formatting.h				\
		include/char_functions.h

bin/word_parsing.o:								\
	src/word_parsing.c							\
		include/word_parsing.h					\
		include/memory_allocation.h

bin/line_parsing.o: 							\
	src/line_parsing.c							\
		include/line_parsing.h					\
		include/word_parsing.h 					\
		include/structures.h					\
		include/comparators.h					\
		include/char_functions.h				\
		include/memory_allocation.h

bin/lines_processing.o:							\
	src/lines_processing.c						\
		include/lines_processing.h				\
		include/structures.h					\
		include/comparators.h					\
		include/memory_allocation.h

bin/output_recovery.o:							\
	src/output_recovery.c						\
		include/output_recovery.h


clean:
	rm -f bin/*