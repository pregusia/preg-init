#include <base.h>
#include <getopt.h>
#include <signal.h>
#include <syslog.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <dirent.h>
#include <application/Application.h>
#include <io/io.h>

// ************************************************************************************
void printUsage(const char* prgName) {
	printf("Usage: %s -c CONFIG_FILE\n", prgName);
	exit(0);
}

// ************************************************************************************
int main(int argc, char** argv) {
	std::string configFile;

    while (1) {
    	int c = getopt(argc, argv, "c:");
    	if (c == -1) break;
		switch (c) {
			case 'c':
				configFile = optarg;
				break;

			case '?':
			default:
				printUsage(argv[0]);
				break;
		}
    }

    if (configFile.empty()) {
    	printf("Invalid config file\n");
    	printUsage(argv[0]);
    	return 1;
    }
   	return g_app.run(configFile);
}
