#include <inttypes.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <sys/time.h>

const int MEMINFO_BUFFER_LENGTH = 48;

char hostname[256];
long long timestamp;
float uptime;
float loadAvg1;
float loadAvg5;
float loadAvg15;
long totalMemory;
long freeMemory;
long availableMemory;
long long totalSwap;
long long freeSwap;

int main();
void refreshHostname();
void refreshTimestamp();
void refreshUptime();
void refreshLoadAverages();
void refreshMemory();

bool stringStartsWith();
bool stringEndsWith();

int main(int argc, char *argv[]) {
	refreshHostname();
	refreshTimestamp();
	refreshUptime();
	refreshLoadAverages();
	refreshMemory();

	if (argc > 1) {
		if (stringStartsWith(argv[1], "--tsv")) {
			printf("%s\t%lli\t%f\t%f\t%f\t%f\t%li\t%li\t%li\t%lli\t%lli\n", hostname, timestamp, uptime, loadAvg1, loadAvg5, loadAvg15, totalMemory, freeMemory, availableMemory, totalSwap, freeSwap);
		} else {
			printf("Usage: %s [--tsv]\n", argv[0]);
			return 1;
		}
	} else {
		printf("hostname: %s\n", hostname);
		printf("timestamp: %lli\n", timestamp);
		printf("uptime: %f\n", uptime);
		printf("loadAvg1: %f\n", loadAvg1);
		printf("loadAvg5: %f\n", loadAvg5);
		printf("loadAvg15: %f\n", loadAvg15);
		printf("totalMemory: %li\n", totalMemory);
		printf("freeMemory: %li\n", freeMemory);
		printf("availableMemory: %li\n", availableMemory);
		printf("totalSwap: %lli\n", totalSwap);
		printf("freeSwap: %lli\n", freeSwap);
	}

	return 0;
}

void refreshHostname() {
	FILE *fp;
	fp = fopen("/etc/hostname", "r");
	fgets(hostname, sizeof(hostname), fp);
	strtok(hostname, "\n");
	fclose(fp);
}

void refreshTimestamp() {
	struct timeval te;
	gettimeofday(&te, NULL); // get current time
	timestamp = te.tv_sec; // seconds since epoch
}

void refreshUptime() {
	FILE *fp;
	fp = fopen("/proc/uptime", "r");
	fscanf(fp, "%f", &uptime);
	fclose(fp);
}

void refreshLoadAverages() {
	FILE *fp;
	fp = fopen("/proc/loadavg", "r");
	fscanf(fp, "%f %f %f", &loadAvg1, &loadAvg5, &loadAvg15);
	fclose(fp);
}

void refreshMemory(void) {
	FILE *fp = fopen("/proc/meminfo", "r");
	char buf[MEMINFO_BUFFER_LENGTH];

	while (fgets(buf, MEMINFO_BUFFER_LENGTH, fp)) {
		if (stringStartsWith(buf, "MemTotal")) {
			if (stringEndsWith(buf, "kB\n")) {
				totalMemory = strtoimax(buf + 9, NULL, 10);
			} else {
				totalMemory = -1;
			}
		} else if (stringStartsWith(buf, "MemFree")) {
			if (stringEndsWith(buf, "kB\n")) {
				freeMemory = strtoimax(buf + 8, NULL, 10);
			} else {
				freeMemory = -1;
			}
		} else if (stringStartsWith(buf, "MemAvailable")) {
			if (stringEndsWith(buf, "kB\n")) {
				availableMemory = strtoimax(buf + 13, NULL, 10);
			} else {
				availableMemory = -1;
			}
		} else if (stringStartsWith(buf, "SwapTotal")) {
			if (stringEndsWith(buf, "kB\n")) {
				totalSwap = strtoimax(buf + 10, NULL, 10);
			} else {
				totalSwap = -1;
			}
		} else if (stringStartsWith(buf, "SwapFree")) {
			if (stringEndsWith(buf, "kB\n")) {
				freeSwap = strtoimax(buf + 9, NULL, 10);
			} else {
				freeSwap = -1;
			}
		}
	}

	fclose(fp);
}

// https://stackoverflow.com/a/4770992/3642588
bool stringStartsWith(const char *haystack, const char *needle) {
	return strncmp(needle, haystack, strlen(needle)) == 0;
}

// https://stackoverflow.com/a/744822/3642588
bool stringEndsWith(const char *haystack, const char *needle) {
	if (!haystack || !needle) return false;

	int lengthOfHaystack = strlen(haystack);
	int lengthOfNeedle = strlen(needle);

	if (lengthOfHaystack < lengthOfNeedle) return false;

	return strncmp(haystack + lengthOfHaystack - lengthOfNeedle, needle, lengthOfNeedle) == 0;
}
