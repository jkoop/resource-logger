# Simple Server Stats Logger

Something lightweight to add to a crontab or something.

Example output:
```plain
$ ./a.out
hostname: OfficeComp
timestamp: 1653616581
cpus: 4
uptime: 7791.319824
load_avg_1: 0.650000
load_avg_5: 0.780000
load_avg_15: 0.860000
memory_total: 24496544
memory_free: 15445052
memory_available: 19328424
swap_total: 2097148
swap_free: 2097148
```
```plain
$ ./a.out --tsv
OfficeComp	1653616581	4	7791.330078	0.650000	0.780000	0.860000	24496544	15445272	19328644	2097148	2097148
```
```plain
$ ./a.out --form
hostname=OfficeComp&timestamp=1653616581&cpus=4&uptime=7791.319824&load_avg_1=0.650000&load_avg_5=0.780000&load_avg_15=0.860000&memory_total=24496544&memory_free=15445272&memory_available=19328644&swap_total=2097148&swap_free=2097148
```

## Build

```sh
gcc sss-logger.c
```
