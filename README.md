# ulordrig

ulordrig is a high performance Ulord CPU miner, with official support for Windows.

#### Table of contents
* [Features](#features)
* [Install](#Install)
* [Usage](#usage)
* [Common Issues](#common-issues)
* [Other information](#other-information)
* [Release checksums](#release-checksums)
* [Contacts](#contacts)



## Features
* High performance.
* Official Windows support.
* Small Windows executable, without dependencies.
* x86/x64 support.
* keepalived support.
* Command line options compatible with cpuminer.
* Smart automatic [CPU configuration](https://github.com/UlordChain/ulordrig).
* It's open source software.



## Install
* Binary releases: https://github.com/UlordChain/ulordrig/releases
    
* Git tree: https://github.com/UlordChain/ulordrig.git
```
    sudo apt-get install git build-essential cmake libuv1-dev libmicrohttpd-dev openssl    
    git clone https://github.com/UlordChain/ulordrig.git   
    cd ulordrig     
    mkdir build && cd build    
    cmake ..    
    make    
```



## Usage
As an example of testnet: `./ulordrig -o stratum+tcp://test-pool.ulord.one:7200 -u uTRHNH71eiW2KMc2rSwmanpXGgDeKMZXdW.worker1 -p x -t 1`
### Options
```    
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



## Common Issues
### HUGE PAGES unavailable
* Run ulordrig as Administrator.
* Ulordrig automatically enables SeLockMemoryPrivilege for current user, but reboot or sign out still required. [Manual instruction](https://msdn.microsoft.com/en-gb/library/ms190730.aspx).



## Other information
* No HTTP support, only stratum protocol support.
* No TLS support.

### CPU mining performance
* Intel(R) Xeon(R) CPU E3-1231 v3 - 860 H/s (8 threads)

Please note performance is highly dependent on system load. The numbers above are obtained on an idle system. Tasks heavily using a processor cache, such as video playback, can greatly degrade hashrate. Optimal number of threads depends on the size of the L3 cache of a processor, 1 thread requires 1 MB of cache.

### Maximum performance checklist
* Idle operating system.
* Do not exceed optimal thread count.
* Use modern CPUs with AES-NI instruction set.
* Try setup optimal cpu affinity.
* Enable fast memory (Large/Huge pages).



## Release checksums
### SHA-256



## Contacts
* support@ulordrig.org
* https://www.jianshu.com/c/a63d65402fd7
