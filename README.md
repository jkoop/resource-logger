# Simple Server Stats Logger

Something lightweight to add to a crontab or something.

Example output:
```plain
$ ./a.out
hostname: OfficeComp
timestamp: 1653527758
uptime: 10439.269531
loadAvg1: 1.720000
loadAvg5: 1.640000
loadAvg15: 1.550000
totalMemory: 24496544
freeMemory: 16169268
availableMemory: 20476300
totalSwap: 2097148
freeSwap: 2097148
```
```plain
$ ./a.out --tsv
OfficeComp	1653527848	10529.059570	1.540000	1.570000	1.530000	24496544	16102412	20409728	2097148	2097148
```
