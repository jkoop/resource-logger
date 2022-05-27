#include <inttypes.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <sys/time.h>

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
		if (stringStartsWith(argv[1], "--form")) {
			printf("hostname=%s&", hostname);
			printf("timestamp=%lli&", timestamp);
			printf("uptime=%f&", uptime);
			printf("load_avg_1=%f&", loadAvg1);
			printf("load_avg_5=%f&", loadAvg5);
			printf("load_avg_15=%f&", loadAvg15);
			printf("memory_total=%li&", totalMemory);
			printf("memory_free=%li&", freeMemory);
			printf("memory_available=%li&", availableMemory);
			printf("swap_total=%lli&", totalSwap);
			printf("swap_free=%lli\n", freeSwap);
		} else if (stringStartsWith(argv[1], "--tsv")) {
			printf("%s\t", hostname);
			printf("%lli\t", timestamp);
			printf("%f\t", uptime);
			printf("%f\t", loadAvg1);
			printf("%f\t", loadAvg5);
			printf("%f\t", loadAvg15);
			printf("%li\t", totalMemory);
			printf("%li\t", freeMemory);
			printf("%li\t", availableMemory);
			printf("%lli\t", totalSwap);
			printf("%lli\n", freeSwap);
		} else {
			printf("Usage: %s [--tsv|--form]\n", argv[0]);
			return 1;
		}
	} else {
		printf("hostname: %s\n", hostname);
		printf("timestamp: %lli\n", timestamp);
		printf("uptime: %f\n", uptime);
		printf("load_avg_1: %f\n", loadAvg1);
		printf("load_avg_5: %f\n", loadAvg5);
		printf("load_avg_15: %f\n", loadAvg15);
		printf("memory_total: %li\n", totalMemory);
		printf("memory_free: %li\n", freeMemory);
		printf("memory_available: %li\n", availableMemory);
		printf("swap_total: %lli\n", totalSwap);
		printf("swap_free: %lli\n", freeSwap);
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

void refreshMemory() {
	FILE *fp = fopen("/proc/meminfo", "r");
	char buf[48];

	while (fgets(buf, 48, fp)) {
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
