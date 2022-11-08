# Resource Logger

Something lightweight to add to a crontab or something.

Example output:

```plain
$ ./resource-logger
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
$ ./resource-logger --tsv
OfficeComp	1653616581	4	7791.330078	0.650000	0.780000	0.860000	24496544	15445272	19328644	2097148	2097148
```

```plain
$ ./resource-logger --form
hostname=OfficeComp&timestamp=1653616581&cpus=4&uptime=7791.319824&load_avg_1=0.650000&load_avg_5=0.780000&load_avg_15=0.860000&memory_total=24496544&memory_free=15445272&memory_available=19328644&swap_total=2097148&swap_free=2097148
```

## Usage

Some example crontab entries:

```crontab
* * * * * /path/to/resource-logger --tsv >> resource.log
* * * * * curl -X POST https://example.com/endpoint --data-binary $(/path/to/resource-logger --form)
```

## Build

```sh
gcc resource-logger.c -o resource-logger
```
