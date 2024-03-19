#define NOBUILD_IMPLEMENTATION
#include "./nobuild.h"

#define BIN "build/smw"
#define CC "gcc"

#define CFLAGS                          \
  "-DSYSTEM_VOLUME_MIXER_AVAILABLE=0",  \
  "-I/usr/include/SDL2",                \
  "-lSDL2",                             \
  "-I.",                                \
  "-lm"

#define LDFLAGS                       \
  "-lSDL2",                           \
  "-I.",                              \
  "-lm"


void Compile(void) {
  for (int i=0; i < srcCount ; i++) {
		CMD(CC, "-c", CFLAGS, "-o", OBJECTS[i], SOURCES[i], NULL);
  }
  CMD("python3", "assets/restool.py", NULL);
}

void Link(void) {
  CMD(CC, "-o", BIN, sources, LDFLAGS, NULL);
}

/* TODO
 * Get rid of stupidity arrays and macros for sources and objects definition
 * create dirs tree for objects in build
 * Create a friendly menu to toggle features
  
void Compile(void) {
  CMD(CC, "-o", BIN, CFLAGS, sources, NULL);
}
*/

void Wipe(void) {
  size_t objarr = (sizeof(OBJECTS) / sizeof(OBJECTS[1]));
  for (int i=0; i < objarr ; i++) {
    CMD("rm", OBJECTS[i]);
  }
  CMD("rm", BIN, "c.old");
}

int main(int argc, char *argv[]) {
	GO_REBUILD_URSELF(argc, argv);

	if (argc < 2) {
		printf("Usage: %s [-c Compile]\n", argv[0]);
	return EXIT_SUCCESS;
	}

	for (int i = 1; i < argc; i++) {
		char *arg = argv[i];

		if (arg[0] == '-') {
			for (unsigned long int j = 1; j < strlen(arg); j++) {

				switch (arg[j]) {
					case 'c': Compile(); break;
					case 'l': Link();    break;
					case 'w': Wipe();    break;
					default: printf("Unknown option: %c\n", arg[j]);
					break;
				}
			}
		}
	}
	return EXIT_SUCCESS;
}
