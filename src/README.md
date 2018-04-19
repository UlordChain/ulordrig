# ulordrig




ulordrig is a high performance Ulord CPU miner, with official support for Windows.



#### Table of contents
* [Features](#features)
* [Download](#download)
* [Usage](#usage)
* [Algorithm variations](#algorithm-variations)
* [Build](https://github.com/ulordrig/ulordrig/wiki/Build)
* [Common Issues](#common-issues)
* [Other information](#other-information)
* [Release checksums](#release-checksums)
* [Contacts](#contacts)

## Features
* High performance.
* Official Windows support.
* Small Windows executable, without dependencies.
* x86/x64 support.
* Support for backup (failover) mining server.
* keepalived support.
* Command line options compatible with cpuminer.
* Smart automatic [CPU configuration](https:https://github.com/UlordChain/ulordrig).
* Nicehash support
* It's open source software.

## Download
* Binary releases: https://github.com/UlordChain/ulordrig/releases
* Git tree: https://github.com/UlordChain/ulordrig.git
  * Clone with `https://github.com/UlordChain/ulordrig.git` :hammer: [Build instructions](https://github.com/UlordChain/ulordrig/WIKI).

## Usage
Use [config.ulordrig.com](https://config.ulordrig.com/ulordrig) to generate, edit or share configurations.

### Options
```
  -a, --algo=ALGO          cryptonight (default) or cryptonight-lite
  -o, --url=URL            URL of mining server
  -O, --userpass=U:P       username:password pair for mining server
  -u, --user=USERNAME      username for mining server
  -p, --pass=PASSWORD      password for mining server
  -t, --threads=N          number of miner threads
  -v, --av=N               algorithm variation, 0 auto select
  -k, --keepalive          send keepalived for prevent timeout (need pool support)
  -r, --retries=N          number of times to retry before switch to backup server (default: 5)
  -R, --retry-pause=N      time to pause between retries (default: 5)
      --cpu-affinity       set process affinity to CPU core(s), mask 0x3 for cores 0 and 1
      --cpu-priority       set process priority (0 idle, 2 normal to 5 highest)
      --no-huge-pages      disable huge pages support
      --no-color           disable colored output
      --variant            algorithm PoW variant
      --donate-level=N     donate level, default 5% (5 minutes in 100 minutes)
      --user-agent         set custom user-agent string for pool
  -B, --background         run the miner in the background
  -c, --config=FILE        load a JSON-format configuration file
  -l, --log-file=FILE      log all output to a file
  -S, --syslog             use system log for output messages
      --max-cpu-usage=N    maximum CPU usage for automatic threads mode (default 75)
      --safe               safe adjust threads and av settings for current CPU
      --nicehash           enable nicehash/ulordrig-proxy support
      --print-time=N       print hashrate report every N seconds
      --api-port=N         port for the miner API
      --api-access-token=T access token for API
      --api-worker-id=ID   custom worker-id for API
  -h, --help               display this help and exit
  -V, --version            output version information and exit
```

Also you can use configuration via config file, default **config.json**. You can load multiple config files and combine it with command line options.

## Algorithm variations
Since version 0.8.0.
* `--av=1` For CPUs with hardware AES.
* `--av=2` Lower power mode (double hash) of `1`.
* `--av=3` Software AES implementation.
* `--av=4` Lower power mode (double hash) of `3`.

## Common Issues
### HUGE PAGES unavailable
* Run ulordrig as Administrator.
* Since version 0.8.0 ulordrig automatically enables SeLockMemoryPrivilege for current user, but reboot or sign out still required. [Manual instruction](https://msdn.microsoft.com/en-gb/library/ms190730.aspx).

## Other information
* No HTTP support, only stratum protocol support.
* No TLS support.



### CPU mining performance
* **Intel i7-7700** - 307 H/s (4 threads)
* **AMD Ryzen 7 1700X** - 560 H/s (8 threads)

Please note performance is highly dependent on system load. The numbers above are obtained on an idle system. Tasks heavily using a processor cache, such as video playback, can greatly degrade hashrate. Optimal number of threads depends on the size of the L3 cache of a processor, 1 thread requires 2 MB of cache.

### Maximum performance checklist
* Idle operating system.
* Do not exceed optimal thread count.
* Use modern CPUs with AES-NI instruction set.
* Try setup optimal cpu affinity.
* Enable fast memory (Large/Huge pages).



## Release checksums
### SHA-256
```
b070d06a3615f3db67ad3beab43d6d21f3c88026aa2b4726a93df47145cd30ec ulordrig-0.0.1-xenial-amd64.tar.gz/ulordrig-0.0.1/ulordrig
4852135d3f04fd450ba39abce51ca40ff9131d222220c8b30804be05f6679295 ulordrig-0.0.1-gcc-win32.zip/ulordrig.exe
284309d07f08261af19c937ece6d2031910d9124a7359c207ded65890b2d7c5f ulordrig-0.0.1-gcc-win64.zip/ulordrig.exe
e1dc46158a578fb030538fb06e5663a6acc5763545fb447a00ce0a6b388c5226 ulordrig-0.0.1-msvc-win64.zip/ulordrig.exe
```

## Contacts
* support@ulordrig.org
* https://www.jianshu.com/c/a63d65402fd7
