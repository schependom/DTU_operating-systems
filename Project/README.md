# OS Challenge - Brute Force Brigade

## Introduction to the OS Challenge

### Goal

The goal of this assignment is to implement a server that can **reverse cryptographic hashes** through **brute-force** search within a specified range.

Normally, cryptographic hash functions such as _SHA-256_ are designed to be **one-way**: they map any input to a fixed-size output (a 256-bit hash) in a deterministic but practically irreversible way. The only viable method to recover the original input from a hash is to try every possible input until a match is found.

### Hash details

The server must accept client **requests** that contain:

-   A SHA-256 hash to reverse
    -   The hash is provided as a 32-byte array.
    -   The input that produced this hash is guaranteed to be a **64-bit unsigned integer**
-   A `start` and `end` value, which define the search interval for possible inputs
    -   This limits the brute-force search to a manageable subset of the full 64-bit space
    -   Without this, we would need to search from `0x0` to `0xFFFFFFFFFFFFFFFF`, giving us 2^64 possible values - over 18 quintillion possibilities!
-   A `priority` level for the request.

### Server Requirements

The server must:

-   Correctly implement the protocol defined in the assignment.
-   Communicate using **TCP** sockets, where each connection represents one reverse hashing request.
-   Respond with exactly an **8-byte message** containing the discovered (64-bit unsigned integer) input.
-   Handle multiple **concurrent** client connections efficiently.
-   Carefully manage **byte order** conversions:
    -   All packet fields in transit use big-endian (network order)
    -   The 64-bit integers used to generate hashes are little-endian.
    -   To handle this correctly, conversion functions such as `htobe64`, `htole64`, `be64toh`, and `le64toh` are required.

## Project Structure

```
os-challenge-bruteforcebrigade/
│
├── .gitignore                      # ignore build artifacts
├── bruteforcebrigade.csv           # team information
├── BruteForceBrigade-milestone.csv # milestone submission
├── Internal-README.md              # internal notes and knowledge base
├── Makefile                        # builds server at repo root
├── README.md                       # the file you are reading now :D
├── submission.py                   # submission script
│
├── benchmark-clients/              # benchmark client scripts
├── common/                         # from os-challenge-common
│
├── src/                            # implementation files
│   ├── bruteforce.c                # brute-force logic
│   ├── handleClient.c              # per-client connection handling
│   ├── handleRequest.c             # handling for client requests, parses data and wrap it in a struct
│   ├── queue.c                     # max-heap setup and helper functions
│   ├── server.c                    # main entry point for the program
│   ├── socketSetup.c               # socket setup and management
│   ├── threadpool.c                # thread pool setup and cleanup
│   └── utils.c                     # utility functions (sendn and recvn)
│
├── include/                        # own headers
│   ├── bruteforce.h                # brute-force function declarations
│   ├── common.h                    # a shared header containing the standard libraries used
│   ├── handleClient.h              # per-client connection handling function declarations
│   ├── handleRequest.h             # defines structs for a client request, bruteforce request, and chunk data.
│   ├── messages.h                  # provided by Xefa
│   ├── queue.h                     # scaffold for queue functions and a struct that defines a task in the queue
│   ├── requestPacket.h             # request packet structure, plus other constants
│   ├── signals.h                   # defines a shared shutdown signal to sync cleanup
│   ├── socketSetup.h               # socket setup and management function declarations
│   ├── threadPool.h                # defines a struct to keep track of threads and other scaffolding functions
│   ├── timelog.h                   # timing and logging utilities
│   └── utils.h                     # utility function declarations
│
├── experiments/                    # each experiment has its own folder
│   ├── 1-Baseline/
│   ├── 2-Caching-Hashmap/
│   ├── 2-Caching-Hashmap-BloomFilter/
│   ├── 3-InterRequest-Parallelism/
│   ├── 4-Multithreading-Pool/
│   ├── 4-Multithreading-Pool-Utilize-Idle/
│   └── 5-Priority/
│
└── images/                         # images for README.md
```

## Compiling and Running the production server (main folder)

To compile the production code (in the root directory), navigate to the project root directory, and run:

```bash
make clean
make
```

This will generate an executable named `server`, which you can run with:

```bash
./server <port>
```

## VMware installation on Apple Silicon

### Installing Rosetta

Rosetta is needed in order to run x86_64 applications on Apple Silicon:

```bash
/usr/sbin/softwareupdate --install-rosetta --agree-to-license
```

### Installing Vagrant

```bash
brew install vagrant
```

### Installing VMWare Fusion 13

VMWare Fusion _Player_ is discontinued, use _Pro_ instead.

Click [here](https://support.broadcom.com/group/ecx/productdownloads?subfamily=VMware%20Fusion&freeDownloads=true) to download.

### Installing VMWare Utility

There are two steps to install the VMWare Utility.

```bash
brew install --cask vagrant-vmware-utility
```

```bash
vagrant plugin install vagrant-vmware-desktop
```

### Running Vagrant

Inside the `common/arm64` folder, run:

```bash
vagrant up
```

Now the VM should open automatically in VMWare Fusion. **Don't close it**, but also don't continue working in the VM window.\
Instead, connect to it via **your own terminal** (so that you can copy-paste stuff).

First, list all running VMs:

```bash
vagrant global-status
```

Now, copy the ID of both the `client` and `server` VM, and connect to them (in two separate terminal windows) via:

```bash
vagrant ssh <id>
```

### Cloning the repository on your VM

Create an SSH key on your VM (doing it via your own terminal though (see above)):

```bash
ssh-keygen
```

Now just accept the default destination and continue without passkey for ease.

```bash
cat ~/.ssh/id_rsa.pub
```

Add the key to your GitHub/GitLab account and then clone the repository:

```bash
git clone <repository-url>
```

## Compiling and Running

To compile the code, navigate to the project root directory, and run:

```bash
make clean
make
```

This will generate an executable named `server`, which you can run with:

```bash
./server <port>
```

## Running the code on Apple Silicon without a VM

Because we make use of `openssl` version `1.1`, which is not available on Apple Silicon via Homebrew, we need to install it manually.
We'll do this using conda.

### Install Conda

You can install Miniconda by following the instructions [here](https://docs.conda.io/en/latest/miniconda.html).

#### Create a new Conda environment

Create a new Conda environment with name (`-n`) `ossl11` and with `openssl` version `1.1` (the `-y` flag automatically confirms the installation):

```bash
conda create -n ossl11 -y openssl=1.1
```

#### Activate the environment

Activate the newly created environment with:

```bash
conda activate ossl11
```

### Running a 'client' request via your local machine

#### Start the server

Navigate to the experiment folder you're working on:

```bash
cd experiments/x-your-experiment
```

Compile the server:

```bash
make clean
make
```

Run on a specific port (see the `client` sript for which port to use):

```bash
./server <port>
```

#### Run the client benchmark script

Navigate to the `benchmark-clients` folder ...

```bash
cd benchmark-clients
```

... and run your favourite client script, e.g.:

```bash
./run-client-milestone.sh
```

---

**NOTE!**

Make sure that...

-   ... OpenSSL 1.1 is correctly installed via conda (see above) and that you have activated the `ossl11` environment before running the client script.
-   ... your benchmark script is correctly pointing to the `common` folder.
-   ... your benchmark script uses `./locate-openssl.sh` to find the OpenSSL 1.1 library path (see `run-client.sh` and `run-client-milestone.sh` for examples).
-   ... the server is running before starting the client script.
-   ... the host is the IP of the `server` VM (`192.168.101.10`)
-   ... the `PORT` variable in the script matches the port you started the server on.

## Experiments

### Timing and logging

The server logs each request's processing time, thread ID, and other relevant information to `server.log` in the experiment's folder **if you enable logging** when running the server.

To enable this logging, pass the optional string `log` as a second argument when starting the server, e.g.:

```bash
./server <port> log
```

---

# Experiment 1: _Baseline performance_

_Conducted by Vincent Van Schependom (**s251739**)_

Created the infrastructure for measuring performance of our different server implementations.

---

# Experiment 2a: Caching with Hashmap

_Conducted by Lucas Kemmer (**s252278**)_

## 1. Question

If we implement caching, can we reduce latency (and in turn the score) when the client sends requests that have already been processed.

## 2. Hypothesis

Caching repeated request results will reduce average latency and improve score. The benefits will be more pronounced at higher repetition rates and negligable at lower repetition rates.

## 3. Experiment Design

Three implementations were compared:

-   **Baseline (our current implementation)**: All requests are brute-forced.
-   **Cached (No Bloom)**: Caches results using a linear-probing hash table.

Each implementation was tested under three repetition scenarios:

-   **No Repetition (0%)**: No matching requests
    -   run-client-milestone-norep.sh
-   **Low Repetition (20%)**: Acouple matching requests have been sent to the server.
    -   run-client-milestone.sh
-   **High Repetition (80%)**: Most of the requests have already been sent to the server.
    -   run-client-milestone-highrep.sh

Metrics recorded:

-   **NOTE**: The following are just measurement of the caching/bruteforcing logic not the overall send/recv latency of the server. See [here](#a-calculations) for how the metrics were timed.
-   **Average Latency (ms)**: Mean duration across all requests.
-   **Weighted Latency**: Latency × priority, averaged.
-   **First-Time Request Latency (ms)**: For unique/new requests (that need to be bruteforced).
-   **Repeated Request Latency (ms)**: For repeated/duplicate requests (that should be stored in cache).

**How the metric's were timed** {#a-calculations}

-   **Start**: Immediately before dispatching the request to either bruteforce or cache lookup.
-   **End**: Immediately after the answer is computed, found by the cache, or a mixture of both (ie bruteforced and added to cache).

## 4.1 Results (only caching/bruteforce logic)

### 0% Repetition — Baseline vs Cached (No Bloom)

| Metric                   | Baseline | Cached (No Bloom) |
| ------------------------ | -------- | ----------------- |
| Average Latency (ms)     | 576.58   | 576.05            |
| Score (Weighted Latency) | 741.05   | 739.05            |
| First-Time Latency (ms)  | N/A      | N/A               |
| Repeated Latency (ms)    | N/A      | N/A               |

### Low Repetition (20%) — Baseline vs Cached (No Bloom)

| Metric                   | Baseline | Cached (No Bloom) |
| ------------------------ | -------- | ----------------- |
| Average Latency (ms)     | 587.18   | 512.80            |
| Score (Weighted Latency) | 747.24   | 671.04            |
| First-Time Latency (ms)  | 584.72   | 596.28            |
| Repeated Latency (ms)    | 607.37   | 258.57            |

### High Repetition (80%) — Baseline vs Cached (No Bloom)

| Metric                   | Baseline | Cached (No Bloom) |
| ------------------------ | -------- | ----------------- |
| Average Latency (ms)     | 509.79   | 146.02            |
| Score (Weighted Latency) | 660.07   | 202.07            |
| First-Time Latency (ms)  | 741.99   | 768.49            |
| Repeated Latency (ms)    | 475.64   | 63.35             |

## Visualizations (only caching/bruteforce logic)

#### No Repetition (0%)

-   **Baseline VS Cache (no Bloom Filter)**
    -   ![Image](experiments/2-Caching-Hashmap/image/baseline-cache-norep-histogram-duration.png)
    -   ![Image](experiments/2-Caching-Hashmap/image/baseline-cache-norep-scatter-duration.png)
    -   ![Image](experiments/2-Caching-Hashmap/image/baseline-cache-norep-score-by-priority.png)

#### Low Repetition (20%)

-   **Baseline VS Cache (no Bloom Filter)**
    -   ![Image](experiments/2-Caching-Hashmap/image/baseline-cache-lowrep-histogram-duration.png)
    -   ![Image](experiments/2-Caching-Hashmap/image/baseline-cache-lowrep-scatter-duration.png)
    -   ![Image](experiments/2-Caching-Hashmap/image/baseline-cache-lowrep-score-by-priority.png)

#### High Repetition (80%)

-   **Baseline VS Cache (no Bloom Filter)**
    -   ![Image](experiments/2-Caching-Hashmap/image/baseline-cache-highrep-histogram-duration.png)
    -   ![Image](experiments/2-Caching-Hashmap/image/baseline-cache-highrep-scatter-duration.png)
    -   ![Image](experiments/2-Caching-Hashmap/image/baseline-cache-highrep-score-by-priority.png)

## 4.2 Results (based on provided score/latency)

###

| Rep % | Baseline  | Cached (No Bloom) |
| ----- | --------- | ----------------- |
| 0%    | 1,976,092 | 2,188,159         |
| 20%   | 2,582,482 | 1,287,667         |
| 80%   | 966,363   | 682,078           |

## Visualizations (based on provided score/latency)

### Weigthed Latency Over Time (score given by Xefa's client)

#### No Repetition (0%)

![Image](experiments/2-Caching-Hashmap/image/norep_combined_weighted_latency.png)

#### Low Repetition (20%)

![Image](experiments/2-Caching-Hashmap/image/20rep_combined_weighted_latency.png)

#### High Repetition (80%)

![Image](experiments/2-Caching-Hashmap/image/80rep_combined_weighted_latency.png)

### Top 50 Weighted Latencies

No Repetition (0%)

-   **Baseline**
    -   ![Image](experiments/2-Caching-Hashmap/image/norep-baseline_top_50_weighted_latencies.png)
-   **Cache (No Bloom)**
    -   ![Image](experiments/2-Caching-Hashmap/image/norep-cache-nobloom_top_50_weighted_latencies.png)

#### Low Repetition (20%)

-   **Baseline**
    -   ![Image](experiments/2-Caching-Hashmap/image/20rep-baseline_top_50_weighted_latencies.png)
-   **Cache (No Bloom)**
    -   ![Image](experiments/2-Caching-Hashmap/image/20rep-cache-nobloom_top_50_weighted_latencies.png)

#### High Repetition (80%)

-   **Baseline**
    -   ![Image](experiments/2-Caching-Hashmap/image/80rep-baseline_top_50_weighted_latencies.png)
-   **Cache (No Bloom)**
    -   ![Image](experiments/2-Caching-Hashmap/image/80rep-cache-nobloom_top_50_weighted_latencies.png)

## 5. Analysis

-   **Caching consistently reduced repeated request latency**, even under low repetition.
-   **High repetition amplified caching benefits**, with dramatic reductions in latency.
-   **Caching performance at no repetition was on par with the baseline**, proving minimal/no downside for implmentation.
-   **First-time latency remained high across all setups**, confirming that the bruteforce cost (atleast for the first run) is unavoidable.

## 6. Conclusion

This experiment demonstrates that caching significantly improves performance in workloads with repeated requests, reducing both average latency and weighted score. Additionally, the data shows that as the repetition percentage increases the benefits of caching only become more noticable. For example, the tests with 80% repetition showed that repeated request latency with cachine dropped to only ~65 ms from the baseline of ~475 ms.

Most importantly, data showed that caching introduced minimal overhead even in the absence of repetition. In the 0% repetition case, where every request would need to be bruteforced, both caching implementations performed identical or nearly identical to the baseline. Further proving that implmenting some form of caching provides minimal to no performance penality, even when repetition is low/non-existent.

---

# Experiment 2b: Caching with Bloom Filter

_Conducted by Lucas Kemmer (**s252278**)_

## 1. Question

If we implement caching with a bloom filter, can we reduce latency even further in the low/no repetition cases.

## 2. Hypothesis

Caching at high repetition rates already shows a reduced latency. By adding a Bloom Filter we can reduce the latency on low-repetition clients by quickly identifying and rejecting new requests. Reducing the amount of time we probe (search) the cache for something that doesnt exist yet.

## 3. Experiment Design

Three implementations were compared:

-   **Baseline (our current implementation)**: All requests are brute-forced.
-   **Cached (No Bloom)**: Caches results using a linear-probing hash table.
-   **Cached (With Bloom)**: Usings a linear-probing hash table, but adds a Bloom filter to more efficiently identify unseen hashes (ie. ends search early).

Each implementation was tested under three repetition scenarios:

-   **No Repetition (0%)**: No matching requests
    -   run-client-milestone-norep.sh
-   **Low Repetition (20%)**: Acouple matching requests have been sent to the server.
    -   run-client-milestone.sh
-   **High Repetition (80%)**: Most of the requests have already been sent to the server.
    -   run-client-milestone-highrep.sh

Metrics recorded:

-   **NOTE**: The following are just measurement of the caching/bruteforcing logic not the overall send/recv latency of the server. See [here](#a-calculations) for how the metrics were timed.
-   **Average Latency (ms)**: Mean duration across all requests.
-   **Weighted Latency**: Latency × priority, averaged.
-   **First-Time Request Latency (ms)**: For unique/new requests (that need to be bruteforced).
-   **Repeated Request Latency (ms)**: For repeated/duplicate requests (that should be stored in cache).

**How the metric's were timed** {#a-calculations}

-   **Start**: Immediately before dispatching the request to either bruteforce or cache lookup.
-   **End**: Immediately after the answer is computed, found by the cache, or a mixture of both (ie bruteforced and added to cache).

## 4.1 Results (only caching/bruteforce logic)

### 0% Repetition — Baseline vs Cached (No Bloom)

| Metric                   | Baseline | Cached (No Bloom) |
| ------------------------ | -------- | ----------------- |
| Average Latency (ms)     | 576.58   | 576.05            |
| Score (Weighted Latency) | 741.05   | 739.05            |
| First-Time Latency (ms)  | N/A      | N/A               |
| Repeated Latency (ms)    | N/A      | N/A               |

### 0% Repetition — Baseline vs Cached (With Bloom)

| Metric                   | Baseline | Cached (With Bloom) |
| ------------------------ | -------- | ------------------- |
| Average Latency (ms)     | 576.58   | 579.52              |
| Score (Weighted Latency) | 741.05   | 746.57              |
| First-Time Latency (ms)  | N/A      | N/A                 |
| Repeated Latency (ms)    | N/A      | N/A                 |

### Low Repetition (20%) — Baseline vs Cached (No Bloom)

| Metric                   | Baseline | Cached (No Bloom) |
| ------------------------ | -------- | ----------------- |
| Average Latency (ms)     | 587.18   | 512.80            |
| Score (Weighted Latency) | 747.24   | 671.04            |
| First-Time Latency (ms)  | 584.72   | 596.28            |
| Repeated Latency (ms)    | 607.37   | 258.57            |

### Low Repetition (20%) — Baseline vs Cached (With Bloom)

| Metric                   | Baseline | Cached (With Bloom) |
| ------------------------ | -------- | ------------------- |
| Average Latency (ms)     | 587.18   | 507.79              |
| Score (Weighted Latency) | 747.24   | 665.60              |
| First-Time Latency (ms)  | 584.72   | 590.45              |
| Repeated Latency (ms)    | 607.37   | 260.16              |

### High Repetition (80%) — Baseline vs Cached (No Bloom)

| Metric                   | Baseline | Cached (No Bloom) |
| ------------------------ | -------- | ----------------- |
| Average Latency (ms)     | 509.79   | 146.02            |
| Score (Weighted Latency) | 660.07   | 202.07            |
| First-Time Latency (ms)  | 741.99   | 768.49            |
| Repeated Latency (ms)    | 475.64   | 63.35             |

### High Repetition (80%) — Baseline vs Cached (With Bloom)

| Metric                   | Baseline | Cached (With Bloom) |
| ------------------------ | -------- | ------------------- |
| Average Latency (ms)     | 509.79   | 152.08              |
| Score (Weighted Latency) | 660.07   | 208.46              |
| First-Time Latency (ms)  | 741.99   | 800.39              |
| Repeated Latency (ms)    | 475.64   | 63.61               |

## Visualizations (only caching/bruteforce logic)

#### No Repetition (0%)

-   **Baseline VS Cache (no Bloom Filter)**
    -   ![Image](experiments/2-Caching-Hashmap-BloomFilter/image/baseline-cache-norep-histogram-duration.png)
    -   ![Image](experiments/2-Caching-Hashmap-BloomFilter/image/baseline-cache-norep-scatter-duration.png)
    -   ![Image](experiments/2-Caching-Hashmap-BloomFilter/image/baseline-cache-norep-score-by-priority.png)
-   **Baseline VS Cache (with Bloom Filter)**
    -   ![Image](experiments/2-Caching-Hashmap-BloomFilter/image/baseline-bloom-norep-histogram-duration.png)
    -   ![Image](experiments/2-Caching-Hashmap-BloomFilter/image/baseline-bloom-norep-scatter-duration.png)
    -   ![Image](experiments/2-Caching-Hashmap-BloomFilter/image/baseline-bloom-norep-score-by-priority.png)

#### Low Repetition (20%)

-   **Baseline VS Cache (no Bloom Filter)**
    -   ![Image](experiments/2-Caching-Hashmap-BloomFilter/image/baseline-cache-lowrep-histogram-duration.png)
    -   ![Image](experiments/2-Caching-Hashmap-BloomFilter/image/baseline-cache-lowrep-scatter-duration.png)
    -   ![Image](experiments/2-Caching-Hashmap-BloomFilter/image/baseline-cache-lowrep-score-by-priority.png)
-   **Baseline VS Cache (with Bloom Filter)**
    -   ![Image](experiments/2-Caching-Hashmap-BloomFilter/image/baseline-bloom-lowrep-histogram-duration.png)
    -   ![Image](experiments/2-Caching-Hashmap-BloomFilter/image/baseline-bloom-lowrep-scatter-duration.png)
    -   ![Image](experiments/2-Caching-Hashmap-BloomFilter/image/baseline-bloom-lowrep-score-by-priority.png)

#### High Repetition (80%)

-   **Baseline VS Cache (no Bloom Filter)**
    -   ![Image](experiments/2-Caching-Hashmap-BloomFilter/image/baseline-cache-highrep-histogram-duration.png)
    -   ![Image](experiments/2-Caching-Hashmap-BloomFilter/image/baseline-cache-highrep-scatter-duration.png)
    -   ![Image](experiments/2-Caching-Hashmap-BloomFilter/image/baseline-cache-highrep-score-by-priority.png)
-   **Baseline VS Cache (with Bloom Filter)**
    -   ![Image](experiments/2-Caching-Hashmap-BloomFilter/image/baseline-bloom-highrep-histogram-duration.png)
    -   ![Image](experiments/2-Caching-Hashmap-BloomFilter/image/baseline-bloom-highrep-scatter-duration.png)
    -   ![Image](experiments/2-Caching-Hashmap-BloomFilter/image/baseline-bloom-highrep-score-by-priority.png)

## 4.2 Results (based on provided score/latency)

###

| Rep % | Baseline  | Cached (No Bloom) | Cached (With Bloom) |
| ----- | --------- | ----------------- | ------------------- |
| 0%    | 1,976,092 | 2,188,159         | 2,333,941           |
| 20%   | 2,582,482 | 1,287,667         | 1,262,300           |
| 80%   | 966,363   | 682,078           | 262,037             |

## Visualizations (based on provided score/latency)

### Weigthed Latency Over Time (score given by Xefa's client)

#### No Repetition (0%)

![Image](experiments/2-Caching-Hashmap-BloomFilter/image/norep_combined_weighted_latency.png)

#### Low Repetition (20%)

![Image](experiments/2-Caching-Hashmap-BloomFilter/image/20rep_combined_weighted_latency.png)

#### High Repetition (80%)

![Image](experiments/2-Caching-Hashmap-BloomFilter/image/80rep_combined_weighted_latency.png)

### Top 50 Weighted Latencies

No Repetition (0%)

-   **Baseline**
    -   ![Image](experiments/2-Caching-Hashmap-BloomFilter/image/norep-baseline_top_50_weighted_latencies.png)
-   **Cache (No Bloom)**
    -   ![Image](experiments/2-Caching-Hashmap-BloomFilter/image/norep-cache-nobloom_top_50_weighted_latencies.png)
-   **Cache (With Bloom)**
    -   ![Image](experiments/2-Caching-Hashmap-BloomFilter/image/norep-cache-withbloom_top_50_weighted_latencies.png)

#### Low Repetition (20%)

-   **Baseline**
    -   ![Image](experiments/2-Caching-Hashmap-BloomFilter/image/20rep-baseline_top_50_weighted_latencies.png)
-   **Cache (No Bloom)**
    -   ![Image](experiments/2-Caching-Hashmap-BloomFilter/image/20rep-cache-nobloom_top_50_weighted_latencies.png)
-   **Cache (With Bloom)**
    -   ![Image](experiments/2-Caching-Hashmap-BloomFilter/image/20rep-cache-withbloom_top_50_weighted_latencies.png)

#### High Repetition (80%)

-   **Baseline**
    -   ![Image](experiments/2-Caching-Hashmap-BloomFilter/image/80rep-baseline_top_50_weighted_latencies.png)
-   **Cache (No Bloom)**
    -   ![Image](experiments/2-Caching-Hashmap-BloomFilter/image/80rep-cache-nobloom_top_50_weighted_latencies.png)
-   **Cache (With Bloom)**
    -   ![Image](experiments/2-Caching-Hashmap-BloomFilter/image/80rep-cache-withbloom_top_50_weighted_latencies.png)

## 5. Analysis

-   **Caching consistently reduced repeated request latency**, even under low repetition.
-   **Bloom filter provided minimal gains** over plain caching in low-repetition scenarios. Which shouldn't be the case, this data warrant additional testing.
-   **High repetition amplified caching benefits**, with dramatic reductions in latency.
-   **Caching performance at no repetition was on par with the baseline**, proving minimal/no downside for implmentation.
-   **The bloom filter's performance under high repetition** was much better than plain caching, which is inconsistent with its intended purpose.
-   **First-time latency remained high across all setups**, confirming that the bruteforce cost (atleast for the first run) is unavoidable.

## 6. Conclusion

This experiment demonstrates that caching significantly improves performance in workloads with repeated requests, reducing both average latency and weighted score. Additionally, the data shows that as the repetition percentage increases the benefits of caching only become more noticable. For example, the tests with 80% repetition showed that repeated request latency with cachine dropped to only ~65 ms from the baseline of ~475 ms.

Surprisingly, the Bloom filter showed the best performance gains under high repetition, despite being designed to optimize the low repetition cases. This suggest that it's either ineffiective for these types of hashs, or the setup/configuration is wrong. More tweaking is needed so that it can provide gains in both the HIGH and LOW repetition cases.

Most importantly, data showed that caching introduced minimal overhead even in the absence of repetition. In the 0% repetition case, where every request would need to be bruteforced, both caching implementations performed identical or nearly identical to the baseline. Further proving that implmenting some form of caching provides minimal to no performance penality, even when repetition is low/non-existent.

# Experiment 3: _Per-request Parallelism_

_Conducted by Vincent Van Schependom (**s251739**)_

## 1. Overview

Multiprocessing and multithreading are two common ways to achieve parallelism in concurrent server applications, like our `server` that receives multiple client hash-requests. In this experiment, we will implement a parallel version of our `server` using both of these techniques. We will compare their performance, resource usage, and complexity.

### 1.1 Types of Parallelism

There are two types of parallelism we will consider in this OS challenge:

1. **Inter-Request Parallelism**:
    - Handling multiple client connections simultaneously.
    - This is crucial so that one client's long-running bruteforce request does not block others.
2. **Intra-Request Parallelism**:
    - Distributing the bruteforcing workload of a single client request across multiple processes or threads.
    - For a single client request with a huge range (e.g., 0 to 1,000,000,000), we can use multiple CPU cores to check parts of that range simultaneously, finding the answer faster.

#### Which to choose?

If we could only choose one of the above, in theory, each type works best if:

|                        | **Inter-Request Parallelism**            | **Intra-Request Parallelism**    |
| ---------------------- | ---------------------------------------- | -------------------------------- |
| **Number of requests** | Many requests                            | Few requests                     |
| **Request difficulty** | Low to moderate difficulty (small range) | Very high difficulty (big range) |

#### Combining Both

We can combine both types of parallelism in a single server implementation, but should be careful to **avoid overwhelming the scheduler with too many processes/threads**.

### 1.2 Scope of this Experiment

In this experiment, we will only focus on **inter-request parallelism**. This means that we will try to handle (many) client requests using a **separate process or thread for each of the requests**.

In a later experiment, we will implement **intra-request parallelism** by distributing the bruteforce workload of a single request across multiple processes/threads to handle very high difficulty requests.

### 1.3 Comparison of Server Implementations

We will compare three primary server implementations for **inter-request parallelism**:

1. **Synchronous Server**
    - This is the original baseline (milestone) server from `experiments/1-baseline`.
    - Handles one client at a time, blocking other clients until the current request is completed.
2. **Multiprocess Server**
    - Uses the `fork()` system call to create a **new process for each request**
    - Each process runs independently, with its **own memory space**.
        - No need for synchronization mechanisms (locks) since there is no shared data.
        - _In theory_ high overhead (in terms of memory and CPU) due to process creation and destruction.
3. **Multithreaded Server**
    - Uses a thread pool to handle multiple client connections using **threads**.
    - Threads share the **same memory space**
        - Requires synchronization mechanisms (locks, mutexes) to protect shared data.
        - _In theory_ more lightweight than processes, but still has overhead due to context switching and synchronization.

## 2. Implementation

### 2.1 Multiprocess Server (`fork`-per-request)

This is implemented in `server_mp.c`.

-   **How it works:** The main server loop `accept()`s a connection and immediately calls `fork()`.

    -   The Parent Process closes the client socket and loops back to `accept()` instantly, ready for the next client.
    -   The Child Process inherits the client socket, calls `handle_client()`, sends the response, and then `exit()`.

-   **Pros:**

    -   **Simplicity**: Very easy to implement from the synchronous model.
    -   **Isolation**: Each client is handled in its own process. A crash in one child process will not affect the server or other clients.
    -   **No Shared State**: No need for mutexes or locks, as memory is **not** shared

-   **Cons:**

    -   **High Overhead:** `fork()` is a "heavy" system call. Creating and destroying a full process for every single connection is slow and resource-intensive.
    -   **Poor Scalability:** Cannot handle thousands of concurrent connections. The OS will be overwhelmed by process management.

### 2.2 Multithreaded Server (threadpool)

This is implemented in `server_pool.c`, `threadpool.h` and `threadpool.c`.

-   **How it works:**: A **thread pool** design is used to limit the number of concurrent threads to a fixed number (equal to the number of CPU cores, which is 4 in the case of the `server` VM).

    1. On startup, the server creates
        - a **thread pool** with a **fixed number of worker threads** (equal to the number of **CPU cores**, which is 4 in the case of the `server` VM)
        - a thread-safe task queue (implemented using a mutex and condition variables).
    2. The main thread's only job is to `accept()` connections.
    3. When a client connects, the main thread bundles the `client_fd` into a "task" and adds it to the queue (a very fast, non-blocking operation).
    4. The worker threads wait for tasks to appear in the queue. One of them will grab the task, call `handle_client()`, and then go back to waiting for more tasks.

-   **Pros:**

    -   **High Performance**: Extremely fast. Thread creation overhead is paid only once at the start.
    -   **Low Latency**: The main thread is never blocked and can accept new clients instantly.
    -   **Resource Efficient**: Manages resources perfectly. The server will only ever run $N=4$ concurrent brute-force jobs, preventing it from being overwhelmed.

-   **Cons:**

    -   **Complexity**: Requires implementing a **thread-safe queue** with **mutexes** and **condition variables** (i.e. `pthread_mutex_t` and `pthread_cond_t`). The condition variables are used to signal worker threads when new tasks are available.
    -   **Shared State**: All threads share the **same memory**. Global variables (like the log file `g_logf`) must be protected by mutexes to prevent race conditions.

## 3. Difficulties during implementation

### 3.1 Multithreaded Logging

In a first implementation of this experiment, the `timelog.h` script was modified to be thread-safe using a mutex. This implementation correctly logged timestamps from multiple threads, but was very slow, because each thread had to acquire the mutex on `server_pool.log` before logging anything. This caused a bottleneck, as threads spent a lot of time waiting to log their timestamps, as could be seen from the diagnostic output of `performance/compare.py`.

To solve this, the logging mechanism was changed to use **per-thread log buffers**. Each thread maintains its own in-memory log buffer (a string), and appends timestamps to it without any locking. Only when the server is shut down (using `Ctrl+C`), all threads get joined, and each thread flushes its log buffer to the shared log file `server_pool.log`.

### 3.2 `SHA256` with Multiple Threads

OpenSSL's SHA256 implementation uses internal locking mechanisms to ensure thread safety. When multiple threads all use SHA256 simultaneously, they end up competing for these locks, which caused severe performance degradation in my first implementations of `server_pool.c`.

This was only a problem in the multithreaded server `server_pool`, as the multiprocess server `server_mp` spawns separate processes with their **own memory space** (and thus separate OpenSSL state copies) and the sequential server `server` only has one thread, of course.

To solve this, I stopped using the `SHA256()` _all-in-one_ function. Instead, I created a separate `SHA256_CTX` context for each thread and called all three functions myself on this context: each thread initializes its own context using `SHA256_Init()`, updates it with `SHA256_Update()`, and finalizes it with `SHA256_Final()`. This eliminated contention on OpenSSL's internal locks, and restored performance.

## 4. Testing and Evaluation

The three servers `server`, `server_mp`, and `server_pool` can be built using the provided `Makefile`. They each write to separate log files: `server_baseline.log`, `server_mp.log`, and `server_pool.log`.

If you copy these log files to the `performance/logs` directory, the `compare.py` script in `performance/` can generate comparison graphs for request latency, throughput, and CPU usage.

In order to use the `compare.py` script, ensure you have the necessary Python packages installed (inside a conda environment):

```bash
pip install pandas seaborn matplotlib tabulate
```

Then run the script:

```bash
python3 performance/compare.py
```

## 5. Results

I ran a couple of different `client` scripts, which can all be found in `benchmark-clients/`. These scripts send multiple requests to the server with varying difficulties and delays.

> _Note_:
> priorities and repetition probabilities are _not_ relevant for this experiment and are thus not mentioned in the results.

### Test 1: Many small, easy requests

#### I) Specifications

|                            |                       |
| -------------------------- | --------------------- |
| **Client script**          | `run-client-rapid.sh` |
| **Number of requests**     | 50                    |
| **Difficulty**             | 300,000               |
| **Delay between requests** | 6,000 $\mu s$ = 6ms   |
| **Seed**                   | 3435245               |

#### II) Results

| Metric                 | Synchronous Server | Multiprocess Server | Multithreaded Server |
| ---------------------- | ------------------ | ------------------- | -------------------- |
| **Total time (s)**     | 0.36               | 0.36                | 0.36                 |
| **Throughput (req/s)** | 138.88             | 139.16              | 140.82               |
| **Mean latency (ms)**  | 6.71               | 8.61                | 9.22                 |
| **Std. latency (ms)**  | 2.99               | 5.35                | 7.32                 |
| **Min latency (ms)**   | 0.35               | 0.35                | 0.35                 |
| **Max latency (ms)**   | 15.37              | 27.42               | 31.95                |

![test 1 - total time](experiments/3-Interrequest-Parallelism/performance/runs-on-vm/client-rapid/graphs/6_total_execution_time.png)
![test 1 - concurrency](experiments/3-Interrequest-Parallelism/performance/runs-on-vm/client-rapid/graphs/4_concurrency_timeline.png)
![test 1 - boxplot request duration](experiments/3-Interrequest-Parallelism/performance/runs-on-vm/client-rapid/graphs/1_duration_distribution_boxplot.png)

#### III) Analysis

In this test with many small, easy requests, all three servers perform similarly in terms of total time and throughput. This is because the requests are so lightweight (300,000 difficulty with only ~7ms mean latency) that they **complete faster than new requests arrive** (6ms delay). Consequently, there is minimal opportunity for parallelism to provide an advantage. The synchronous server can handle each request and return to accepting the next connection before another client connects.

In terms of overhead, we observe a clear increase in both mean and maximum latency for the parallel implementations. The synchronous server has the lowest mean latency (6.71ms), while the multiprocess server (8.61ms) and multithreaded server (9.22ms) show 28% and 37% increases respectively. This overhead stems from the cost of creating processes/threads and managing the task queue, which becomes noticeable when the actual work is very quick.

### Test 2: Many small, harder requests

Now, we use the same parameters as in Test 1, but increase the difficulty of each request by a factor of 10, to 3,000,000.

#### I) Specifications

|                            |                              | comparison to previous test |
| -------------------------- | ---------------------------- | --------------------------- |
| **Client script**          | `run-client-rapid-harder.sh` |                             |
| **Number of requests**     | 50                           | _unchanged_                 |
| **Difficulty**             | **3,000,000**                | **increased** by factor 10  |
| **Delay between requests** | 6,000 $\mu s$ = 6ms          | _unchanged_                 |
| **Seed**                   | 3435245                      | _unchanged_                 |

#### II) Results

| Metric                 | Synchronous Server | Multiprocess Server | Multithreaded Server |
| ---------------------- | ------------------ | ------------------- | -------------------- |
| **Total time (s)**     | 2.59               | 1.36                | 1.35                 |
| **Throughput (req/s)** | 19.30              | 36.83               | 37.02                |
| **Mean latency (ms)**  | 51.75              | 718.43              | 103.55               |
| **Std. latency (ms)**  | 31.96              | 399.13              | 64.17                |
| **Min latency (ms)**   | 0.38               | 0.34                | 0.34                 |
| **Max latency (ms)**   | 112.91             | 1244.30             | 235.05               |

![test 2 - total time](experiments/3-Interrequest-Parallelism/performance/runs-on-vm/client-rapid-harder/graphs/6_total_execution_time.png)
![test 2 - concurrency](experiments/3-Interrequest-Parallelism/performance/runs-on-vm/client-rapid-harder/graphs/4_concurrency_timeline.png)
![test 2 - boxplot request duration](experiments/3-Interrequest-Parallelism/performance/runs-on-vm/client-rapid-harder/graphs/1_duration_distribution_boxplot.png)

#### III) Analysis

When we compare the results to the first experiment (where requests were 10 times easier), we see a dramatic shift in the performances of the servers. The parallel implementations now provide substantial benefits: both the multiprocess and multithreaded servers complete all requests in approximately 1.35 seconds, achieving a 1.9x speedup over the synchronous server (2.59 seconds). This translates to nearly doubling the throughput, from 19.30 req/s to ~37 req/s.

The reason for this improvement is that with the 10x increase in difficulty, each request now takes approximately 50ms to complete (as seen in the synchronous server's mean latency). Since requests arrive every 6ms but take 50ms to process, multiple requests quickly queue up. The parallel servers can process up to 4 requests simultaneously (limited by the 4 CPU cores), while the synchronous server must handle them sequentially.

However, this parallelism comes at a cost in terms of individual request latency. The multiprocess server shows extremely high mean latency (718ms) and maximum latency (1244ms), far exceeding both the synchronous server and the multithreaded server. This occurs because the multiprocess server spawns a new process for every incoming request without limiting concurrency. With requests arriving rapidly (every 6ms) and taking ~50ms each to complete, dozens of processes accumulate, all competing for CPU time. This process explosion causes severe context-switching overhead and scheduler contention.

The reason the **total time is the same for both parallel servers** is that they are both CPU-bound. The total time to complete the entire batch of 50 requests is determined by the total computational work divided by the 4 available CPU cores. **Both servers successfully saturate all 4 cores**.

-   The multithreaded server does this **efficiently**: it runs exactly 4 jobs at a time, so context-switching is low. Other jobs wait in the queue, which increases their latency.
-   The multiprocess server does this **inefficiently**: it runs dozens of processes at once, each making tiny bits of progress. The massive context-switching and scheduler contention dramatically increases the latency for each individual request, but the 4 CPUs are still fully utilized, leading to the same overall throughput and total time.

The multithreaded server performs much better, with mean latency (103.55ms) only about 2x higher than the synchronous server. The thread pool design with 4 worker threads prevents the server from being overwhelmed: it processes exactly 4 requests concurrently while queuing others.

### Test 3: Milestone test

This test simulates a more realistic server workload with moderate difficulty requests (30,000,000) arriving at regular intervals (600ms). The relatively long delay between requests means the server has ample time to complete each request before the next one arrives, limiting the potential for inter-request parallelism, as we will see.

#### I) Specifications

|                            |                                |
| -------------------------- | ------------------------------ |
| **Client script**          | `run-client-milestone.sh`      |
| **Number of requests**     | 100                            |
| **Difficulty**             | 30,000,000                     |
| **Delay between requests** | 600,000 $\mu s$ = 600ms = 0.6s |
| **Seed**                   | 3435245                        |

#### II) Results

| Metric                 | Synchronous Server | Multiprocess Server | Multithreaded Server |
| ---------------------- | ------------------ | ------------------- | -------------------- |
| **Total time (s)**     | 61.52              | 60.30               | 60.30                |
| **Throughput (req/s)** | 1.63               | 1.66                | 1.66                 |
| **Mean latency (ms)**  | 615.04             | 635.10              | 637.44               |
| **Std. latency (ms)**  | 346.85             | 346.44              | 347.76               |
| **Min latency (ms)**   | 3.43               | 3.33                | 3.32                 |
| **Max latency (ms)**   | 1213.38            | 1126.68             | 1151.60              |

![test 3 - total time](experiments/3-Interrequest-Parallelism/performance/runs-on-vm/client-milestone/graphs/6_total_execution_time.png)
![test 3 - concurrency](experiments/3-Interrequest-Parallelism/performance/runs-on-vm/client-milestone/graphs/4_concurrency_timeline.png)
![test 3 - boxplot request duration](experiments/3-Interrequest-Parallelism/performance/runs-on-vm/client-milestone/graphs/1_duration_distribution_boxplot.png)

#### III) Analysis

The results show that all three server implementations perform **nearly identically**, with total execution times around 60-61 seconds and minimal differences in latency metrics. The parallel implementations provide essentially no benefit in this scenario, achieving only a marginal improvement in total time (~1.2 seconds, or 2%).

This outcome is expected given the test parameters. With a 600ms delay between requests and mean request latencies around 615-637ms, each request typically completes just before or shortly after the next one arrives, causing **requests to be processed nearly sequentially** regardless of the server architecture. The concurrency timeline graph confirms this: the "staircase" pattern shows that the server frequently sits idle waiting for the next client connection, rather than processing multiple requests simultaneously.

##### IV) Conclusion

This test **highlights the fundamental limitation of inter-request parallelism**: it can only provide speedup **when multiple requests overlap in time**. On the second figure above, we can see that due to the non-ideal ratio between difficulty and delay, there are a lot of 'stairs' in the concurrency graph, indicating that the server is often idle waiting for new requests. This highlights the need to split up high-difficulty requests using intra-request parallelism (see other experiments).

### Test 4: Shorter-delay Milestone

In the previous test, the delay between requests was quite high (600ms), which limited the potential speedup we could achieve with inter-request parallelism. In this test, we reduce the delay between requests to 200ms (a 3x decrease) while keeping the same difficulty level, **creating significant overlap** between requests and allowing us to evaluate how well each server architecture handles higher concurrent load.

#### I) Specifications

|                            |                                    | comparison to previous test |
| -------------------------- | ---------------------------------- | --------------------------- |
| **Client script**          | `run-client-milestone-faster.sh`   |                             |
| **Number of requests**     | 100                                | _unchanged_                 |
| **Difficulty**             | 30,000,000                         | _unchanged_                 |
| **Delay between requests** | **200,000 $\mu s$ = 200ms = 0.2s** | **decreased** by factor 3   |
| **Seed**                   | 3435245                            | _unchanged_                 |

#### II) Results

| Metric                 | Synchronous Server | Multiprocess Server | Multithreaded Server |
| ---------------------- | ------------------ | ------------------- | -------------------- |
| **Total time (s)**     | 61.93              | 32.04               | 31.97                |
| **Throughput (req/s)** | 1.61               | 3.12                | 3.13                 |
| **Mean latency (ms)**  | 619.12             | **11175.19**        | 1256.86              |
| **Std. latency (ms)**  | 346.85             | 6391.06             | 704.17               |
| **Min latency (ms)**   | 3.47               | 3.45                | 11.40                |
| **Max latency (ms)**   | 1146.20            | **23439.40**        | 2261.13              |

![test 4 - total time](experiments/3-Interrequest-Parallelism/performance/runs-on-vm/client-milestone-faster/graphs/6_total_execution_time.png)
![test 4 - concurrency](experiments/3-Interrequest-Parallelism/performance/runs-on-vm/client-milestone-faster/graphs/4_concurrency_timeline.png)
![test 4 - boxplot request duration](experiments/3-Interrequest-Parallelism/performance/runs-on-vm/client-milestone-faster/graphs/1_duration_distribution_boxplot.png)

#### III) Analysis

By reducing the inter-request delay from 600ms to 200ms while keeping the same difficulty (and thus ~600ms processing time per request), we create a scenario where requests arrive much faster than they can be completed.

The **synchronous** server shows essentially **identical** performance to Test 3 (61.93s vs 61.52s total time), as it processes requests sequentially regardless of how quickly they arrive. Its mean latency (619.12ms) and standard deviation (346.85ms) remain unchanged, confirming that the bottleneck is purely computational: it simply queues incoming requests and processes them one by one.

The parallel implementations achieve approximately 1.94x speedup, completing all requests in ~32 seconds compared to ~62 seconds for the synchronous server. This **near-doubling of throughput** (from 1.61 to ~3.12 req/s) demonstrates effective use of the 4 available CPU cores to process multiple requests concurrently.

However, the **multiprocess server exhibits catastrophic latency degradation**. Its mean latency of 11,175ms (over 11 seconds) and maximum latency of 23,439ms (over 23 seconds) represent a 18x and 20x increase respectively compared to the synchronous server. This occurs because the multiprocess server spawns a new process for every incoming connection without any concurrency control. This enormous number of processes overwhelm the scheduler, causing severe CPU contention.

This is the same "throughput vs. latency" effect from Test 2, but magnified. "Total time" measures the throughput of the entire system (how long to process the 100-request batch), while "latency" measures the response time for individual clients. Both parallel servers are CPU-bound and fully saturate all 4 CPU cores. Therefore, they process the total batch of 100 requests in the same amount of time (≈32s), achieving the same high throughput.

The multithreaded server with thread pool performs far better, with mean latency of 1,256ms and maximum of 2,261ms. This is roughly 9x lower than the multiprocess server. While this is still approximately 2x higher than the synchronous server's latency, it represents a reasonable trade-off: the thread pool limits concurrency to 4 active requests at a time, allowing efficient parallel processing without overwhelming system resources. Requests queue in memory (which is fast) rather than all competing for CPU simultaneously.

The concurrency timeline clearly shows that both parallel servers maintain high utilization throughout the test period, unlike the sequential pattern in Test 3. The multithreaded server shows consistent, controlled concurrency at the 4-thread level, while the multiprocess server's behavior reflects uncontrolled process explosion.

#### IV) Conclusion

This test decisively demonstrates that controlled parallelism (thread pool) vastly outperforms uncontrolled parallelism (fork-per-request) under realistic concurrent load. The multiprocess approach's simplicity becomes a critical liability when request arrival rate exceeds processing capacity.

### Test 5: One, single, big request

This (and the following) test evaluates how each server implementation handles a single, computationally intensive request with no concurrency involved, evaluating the **pure processing overhead** of each architecture. This isolates the pure overhead costs of each architecture without any parallelism benefits.

#### I) Specifications

|                            |                            |
| -------------------------- | -------------------------- |
| **Client script**          | `run-client-single-big.sh` |
| **Number of requests**     | 1                          |
| **Difficulty**             | 100,000,000                |
| **Delay between requests** | Doesn't matter             |
| **Seed**                   | 3435240                    |

#### II) Results

| Metric                      | Synchronous Server | Multiprocess Server | Multithreaded Server |
| --------------------------- | ------------------ | ------------------- | -------------------- |
| **Total time (s)**          | 3.77               | 3.85                | 3.77                 |
| **Latency=total time (ms)** | 3765.03            | 3852.51             | 3772.05              |

![test 5 - request duration](experiments/3-Interrequest-Parallelism/performance/runs-on-vm/client-single-big/graphs/2_duration_histograms_overlay.png)

#### III) Analysis

With only a single request, there is **no opportunity for inter-request parallelism**, so this **test reveals the pure overhead of each architecture**. The synchronous server and multithreaded server perform nearly identically (3765ms vs 3772ms, a difference of only 7ms or 0.2%). This negligible difference demonstrates that **the thread pool architecture, once initialized, adds minimal overhead** when actually processing work.

The multiprocess server is slower at 3852ms, representing a 2.3% **overhead** (87ms) compared to the baseline. This overhead stems from the `fork()` system call, which **must copy the parent process's memory space and create a new process**, and the subsequent process **cleanup** via `exit()`.

On the figure above, we see the $\log_{10}$ of the request duration in milliseconds. We observe that the baseline server has the lowest duration, closely followed by the multithreaded server. The multiprocess server has the highest duration, due to the overhead of process creation and destruction. The logarithmic scale makes the differences appear more pronounced than they are in absolute terms, but the ordering clearly demonstrates that `fork()` imposes measurable costs even for a single connection.

#### IV) Conclusion

This test confirms that when parallelism is not needed, the synchronous approach has the lowest overhead, followed closely by the thread pool approach (whose initialization cost is amortized across many requests), with the fork-per-request approach trailing due to its per-connection overhead.

### Test 6: One, single, small request

This test examines the overhead of each architecture on a **single, extremely lightweight, request**. This represents a worst-case scenario for parallelization overhead, as the actual computation time is very short, making any architectural overhead highly visible.

#### I) Specifications

|                            |                              | Comparison to previous test  |
| -------------------------- | ---------------------------- | ---------------------------- |
| **Client script**          | `run-client-single-small.sh` |                              |
| **Number of requests**     | 1                            | _unchanged_                  |
| **Difficulty**             | 100,000                      | **decreased** by factor 1000 |
| **Delay between requests** | Doesn't matter               | _unchanged_                  |
| **Seed**                   | 3435240                      | _unchanged_                  |

#### II) Results

| Metric                      | Synchronous Server | Multiprocess Server | Multithreaded Server |
| --------------------------- | ------------------ | ------------------- | -------------------- |
| **Latency=total time (ms)** | 5.10               | 9.23                | 5.05                 |

![test 6 - request duration](experiments/3-Interrequest-Parallelism/performance/runs-on-vm/client-single-small/graphs/2_duration_histograms_overlay.png)

#### III) Analysis

With a 1000x reduction in difficulty compared to the previous test, the actual bruteforce computation now completes in approximately 5ms, making any architectural overhead highly visible. The synchronous server completes the request in 5.10ms, establishing the baseline for minimal overhead.

On the figure above, we see the $\log_{10}$ of the request duration in milliseconds. The baseline and multithreaded servers are essentially indistinguishable, while the multiprocess server is clearly separated, visually confirming its overhead penalty.

The multithreaded server achieves 5.05ms, which is actually **slightly _faster_** than the synchronous baseline. This counterintuitive result is within **measurement noise** for such short durations. Even though all three server implementations were **run on the same VM under similar conditions**, small variations in CPU load and OS scheduling can lead to minor differences in execution time, which become especially noticeable for very short tasks. This sub-millisecond variation (0.05ms difference) is **statistically insignificant** and should be considered **equivalent performance**.

The key takeaway is that the **thread pool adds no measurable overhead to request processing once initialized**.

The multiprocess server takes 9.23ms, representing an 81% overhead (4.13ms additional time) compared to the baseline. For this lightweight request where the actual computation takes ~5ms, the `fork()` overhead of ~4ms nearly **_doubles_ the response time**. This dramatically illustrates the multiprocess architecture's inefficiency for small, fast requests: the process creation and destruction costs become the dominant factor.

#### IV) Conclusion

This test definitively shows that the **thread pool architecture has no practical overhead for individual requests regardless of their size**, while the **fork-per-request approach imposes significant fixed costs** that make it **unsuitable for handling lightweight or rapidly-arriving requests**.

## 6. Conclusion

This experiment evaluated three server architectures for inter-request parallelism: a synchronous baseline, a fork-per-request multiprocess server, and a thread-pool-based multithreaded server. The results highlight several key trade-offs:

**Architecture Overhead:**

-   The synchronous baseline server has the lowest per-request overhead but cannot leverage multiple CPU cores
-   The multithreaded thread pool architecture adds virtually no overhead (~0-0.2%) to individual request processing while enabling parallelism
-   The fork-per-request multiprocess architecture imposes significant fixed overhead (2-81% depending on request size) due to process creation and destruction costs

**Parallelism Benefits:** When requests overlap in time (arrival rate exceeds processing rate), the parallel implementations provide substantial throughput improvements:

-   Tests 2 and 4 demonstrated 1.9-2.0x speedup when concurrent load was high
-   Test 3 showed that parallelism provides no benefit when requests don't overlap (0.6s delay, 0.6s processing time)

**Controlled vs. Uncontrolled Concurrency:** The most striking finding was the dramatic performance difference between controlled (thread pool) and uncontrolled (fork-per-request) parallelism:

-   Under high concurrent load (Test 4), the multiprocess server exhibited catastrophic latency degradation (11-23 seconds mean/max latency) due to process explosion
-   The thread pool maintained reasonable latency (1.3s mean) while achieving equivalent throughput by limiting concurrency to 4 active workers
-   This demonstrates that blindly spawning a process/thread per request can severely harm performance. Bounded concurrency is essential.

---

# Experiment 4a/b: Thread Pool for Client Connections vs Thread Pool with Bruteforce chunking (within-request parallelism)

_Conducted by Lucas Kemmer (**s252278**)_

## 1. Question

Can we improve responsiveness and throughput by introducing a thread pool, and does chunking bruteforce tasks further improve performance under varying task priorities and request pacing?

## 2. Hypothesis

Using a thread pool will reduce queue buildup and improve latency under load. Chunking bruteforce tasks into smaller units will further improve responsiveness by enabling finer-grained scheduling and better thread utilization, especially when requests arrive rapidly and with mixed priorities.

## 3.1 Experiment Design

Three implementations were compared:

-   **Baseline**: Single-threaded, handles one request at a time.
-   **Thread Pool (No Chunking)**: Fixed-size pool of worker threads executing full bruteforce tasks.
-   **Thread Pool (With Chunked Bruteforce Tasks)**: Tasks are split into smaller chunks and a dispatcher distributed chunks across threads.

Each implementation was tested under three load scenarios:

-   **Uniform Priority + High Delay**: All tasks have the same priority, and there are long delays between sending requests.
    -   `run-client-uniform-priority-long-delay.sh`
-   **Mixed Priority + Medium Delay**: Tasks have a range of priorities, and there are medium delays between sending requests.
    -   `run-client-mixed-priority-medium-delay.sh`
-   **High Priority Spread + Short Delay**: Tasks span all priority levels, and there are short delays between sending requests.
    -   `run-client-highspread-priority-short-delay.sh`

Metrics recorded:

-   **Queue Depth Over Time**: Number of tasks waiting in the queue.
-   **Thread Idle Time**: Time each thread spent waiting for work.

## How the metrics were timed

-   **Queue Depth Over Time**: Logged at each enqueue and dequeue event, recording the current queue size and timestamp. This captures how the backlog grows or shrinks throughout execution.

-   **Thread Idle Time**: Each thread logs how long it waits for work between tasks. Idle duration is measured from the moment it starts waiting until it successfully dequeues a task.

---

## 3.2 How each implementation works?

Both implmentations define a max heap, which has special interactive functions that utilize mutex locks to prevent race conditions between threads. The difference between the implementation come with the **DATA** stored in this max heap structure (ie the nodes).

-   **Thread Pool (No Chunking)**
    -   The only nodes inserted to the head are the client requests themselves and the corresponding bruteforce task that spawns from the request.
    -   Each bruteforce "task" is given its related priority and designated as resumable (which just means it gets requeued if it doesnt finish), so that given a higher priority task the thread can switch
        -   However, after further testing (as you see later) the queue doesnt seem to expand as expected indicating that this doesn't always work in this implementation
-   **Thread Pool (With Chunking)**
    -   This method keeps the client request node on the heap, but breaks the bruteforce task into a dispatcher and a chunk.
    -   A dispatcher "task" is enqueued from a client request and, once picked up by a thread, will break up bruteforce task into "chunks" by taking the start,end, and a defined chunk range.
    -   Chunks act as smaller bruteforce tasks with special logic to indicate if a answer is found, thus allowing many threads to parralize the same request.
        -   Chunks are also resumable (and can be requeued) meaning if a dispatcher/chunks appear with a higher priority the threads pull those off the heap first.

## 4.1 Results (Thread Specific Performance)

### Uniform Priority + High Delay

| Metric              | Baseline | Thread Pool | Thread Pool + Chunking |
| ------------------- | -------- | ----------- | ---------------------- |
| Max Queue Depth     | N/A      | 1           | 298                    |
| Avg Thread Idle (%) | N/A      | 98.32%      | 86.73%                 |

### Mixed Priority + Medium Delay

| Metric              | Baseline | Thread Pool | Thread Pool + Chunking |
| ------------------- | -------- | ----------- | ---------------------- |
| Max Queue Depth     | N/A      | 1           | 300                    |
| Avg Thread Idle (%) | N/A      | 88.80%      | 79.97%                 |

### High Priority Spread + Short Delay

| Metric              | Baseline | Thread Pool | Thread Pool + Chunking |
| ------------------- | -------- | ----------- | ---------------------- |
| Max Queue Depth     | N/A      | 1           | 299                    |
| Avg Thread Idle (%) | N/A      | 74.34%      | 68.04%                 |

---

## Visualizations (Threading Performance)

### Uniform Priority + High Delay

-   **Queue Depth Over Time**

    -   ![Image](experiments/4-Multithreading-Pool-Utilize-Idle/image/uniformspread-queue-depth-comparison.png)

-   **Thread Idle Time**
    -   ![Image](experiments/4-Multithreading-Pool-Utilize-Idle/image/uniformspread-idle-time-comparison.png)

### Mixed Priority + Medium Delay

**Note the baseline implemenation doesn't have any idle time or a queue, thus it stays at 0 throughout.**

-   **Queue Depth Over Time**

    -   ![Image](experiments/4-Multithreading-Pool-Utilize-Idle/image/mixedspread-queue-depth-comparison.png)

-   **Thread Idle Time**
    -   ![Image](experiments/4-Multithreading-Pool-Utilize-Idle/image/mixedspread-idle-time-comparison.png)

### High Priority Spread + Short Delay

-   **Queue Depth Over Time**

    -   ![Image](experiments/4-Multithreading-Pool-Utilize-Idle/image/highspread-queue-depth-comparison.png)

-   **Thread Idle Time**
    -   ![Image](experiments/4-Multithreading-Pool-Utilize-Idle/image/highspread-idle-time-comparison.png)

---

## 4.2 Results (Overall Score)

| Scenario                           | Baseline Score | Thread Pool Score | Chunked Score |
| ---------------------------------- | -------------- | ----------------- | ------------- |
| Uniform Priority + High Delay      | 671535         | 667408            | 213087        |
| Mixed Priority + Medium Delay      | 1152384        | 1086554           | 331796        |
| High Priority Spread + Short Delay | 49082555       | 1738060           | 523880        |

---

## Visualizations (Overall Score)

### Weighted Latency Over Time

#### Baseline

-   ![Image](experiments/4-Multithreading-Pool-Utilize-Idle/image/baseline-weighted-latency.png)
-   Notice that the y-axis is 10^8

#### Thread Pool (no chunking)

-   ![Image](experiments/4-Multithreading-Pool-Utilize-Idle/image/threadpool-no-chunk-weighted-latency.png)
-   Notice that the y-axis is 10^7

#### Thread pool (with chunking)

-   ![Image](experiments/4-Multithreading-Pool-Utilize-Idle/image/threadpool-with-chunk-weighted-latency.png)
-   Notice that the y-axis is 10^6

---

## 5. Analysis

-   **Chunking bruteforce tasks vastly increased the depth of the queue**, however once a answer was found many tasks early exit the queue causing minimial overhead.
-   **Chunking bruteforce tasks further improved responsiveness**, especially under short-delay, high-priority-spread conditions.
-   **Thread idle time decreased** as request pacing increased, indicating better utilization.
-   **Baseline performance degraded** under fast, heavy load due to its single-threaded bottleneck.
-   No chunking had **minimal queue spikes** because without a dispathcer threads seemed to run only the request they just obtained.

---

## 6. Conclusion

This experiment shows that introducing a thread pool significantly improves system responsiveness and throughput. By further breaking up bruteforce tasks into smaller chunks, thread ulitlization increased and request were able to be solved much faster. As such, the experiment shows that under short-delay, high-priority-spread conditions breaking up tasks into smaller chunks and completing them in the order of priority decreases the weighted latency. Additionally, even in high-delay, low-priority-spread scenarios, chunking introduces minimal overhead against the baseline implementation and as such is a much more efficient solution.

---

### Experiment 5: _Priority Scheduling_

_Conducted by Daniel Lamb (s252343)_

## 1. Question

How do different priority scheduling strategies affect the weighted latency score when requests arrive with varying priority levels? Since the scoring formula penalizes high-priority requests more heavily, can we improve the overall score by prioritizing high-priority requests over low-priority ones?

## 2. Hypothesis

Implementing priority-aware scheduling will reduce the weighted latency score by ensuring high-priority requests are processed first. The benefits will be most pronounced when there is a wide spread of priority levels and requests arrive concurrently, as this creates opportunities to reorder processing to minimize the weighted penalty.

## 3. Experiment Design

I compared 3 implementations:

-   **FIFO (First-In-First-Out)**: Requests are processed in the order they arrive, regardless of priority level. This is the baseline.
-   **Priority Queue (Static)**: Requests are always processed in strict priority order (highes first), using a priority queue
-   **Priority Queue (Preemptive)**: High-priority requests can preempt or interrupt low-priority work in progress, letting immediate processing of urgent requests happen.

Each implementation was tested under three priority distribution scenarios:

-   **Uniform Priority**: All requests have the same priority level (priority = 1).
    -   `run-client-uniform-priority-long-delay.sh`
-   **Mixed Priority Spread**: Requests have a moderate range of priorities (e.g., priorities 1-8), following a mixed distribution.
    -   `run-client-mixed-priority-medium-delay.sh`
-   **High Priority Spread**: Requests span the full priority range (priorities 1-16), with an exponential distribution favoring lower priorities but including high-priority requests.
    -   `run-client-highspread-priority-short-delay.sh`

## Metrics Recorded

The following metrics are produced directly by the client and were used to evaluate each server implementation:

-   **Weighted Latency Score**  
    The primary performance metric. Computed by the client as (1/N)*(The sum of latency*priority)
    Higher-priority requests contribute more heavily to the score

-   **Average Latency (ms)**  
    Mean latency across all requests, regardless of priority.

-   **Reliability (%)**  
    Percentage of successfully completed requests (should be 100%)

-   **Priority Distribution**  
    The actual distribution of priorities used during the test, based on the client's `lambda` parameter.

### How Metrics Were Timed

All timing is handled internally by the client by measuring the duration between start and end times, used for avg latency and weighted latency scores

-   **Start**: When the server accepts the client connection and the request packet is received.
-   **End**: When the server sends the response and closes the client connection.

## 4. Results & Visualizations

This section summarizes the performance of all three server implementations across the three priority test scenarios

### 4.1 Weighted Latency Score Summary Table

This table shows the weighted latency score produced by each server under all three test scenarios:

| Server Type    | Test 1: Uniform (λ = 0.0) | Test 2: Mixed (λ = 1.0) | Test 3: High Priority (λ = 1.5) |
| -------------- | ------------------------- | ----------------------- | ------------------------------- |
| **FIFO**       | **8,041,250**             | **35,409,754**          | **37,840,483**                  |
| **Priority**   | 9,172,220                 | 48,721,573              | 42,640,374                      |
| **Preemptive** | 10,830,741                | 41,904,116              | 55,770,271                      |

FIFO achieves the lowest (best) score in all three scenarios.

### 4.2 Visualizations

Below I organized are the visualizations generated from the full dataset for easier understanding
The raw data is visible at https://docs.google.com/document/d/1scGI96jFFEp1z4fSvDkt8Nef4we7UbxFG1BlicKF5Gs/edit?usp=sharing

### 4.2.1 Comparison Across All Server Types

This graph compares all three server implementations directly across the three test configurations. It shows the average of what scored the best across all 9 tests conducted across the three different strategies

![Scheduling Algorithm Performance Comparison](images/scheduling-algorithm-comparison.png)

![Scheduler Performance vs Priority Parameter λ](images/scheduler-performance-vs-lambda.png)

Interpretation of Results

Across all plots:

-   FIFO consistently had the **best weighted latency score**.
-   Priority scheduling introduces measurable overhead without improving results.
-   Preemption generates the highest overhead and performs the worst—especially under high concurrency.
-   Even as λ increases (more high-priority requests), FIFO still outperforms both priority-aware methods.

So, **priority reordering does not improve performance for this CPU-bound workload**, and the overhead of maintaining priority queues or preempting tasks outweighs any theoretical benefit.

## 5. Analysis

### 5.1 Overall Performance Comparison

Across all three test scenarios, the experimental results reveal a clear and unexpected pattern:  
**the FIFO (baseline) implementation consistently achieves the lowest weighted latency score**, outperforming both the static priority queue and the preemptive priority scheduler.

This directly contradicts the initial hypothesis, which assumed that serving high-priority requests first would reduce the weighted latency score.

#### Key Findings per Test Scenario

1. **Test 1 — Uniform Priority (λ = 0.0)**

    - FIFO: **8,041,250** (best)
    - Priority: 9,172,220 (≈14% worse)
    - Preemptive: 10,830,741 (≈35% worse)

    **Explanation:**  
    With all requests having the same priority, there is no opportunity for priority scheduling to provide any benefit.  
    The priority and preemptive implementations simply incur additional overhead (heap operations, per-request priority checks, and thread-pool synchronization) without gaining any score advantage.

2. **Test 2 — Mixed Priority (λ = 1.0)**

    - FIFO: **35,409,754** (best)
    - Preemptive: 41,904,116 (≈18% worse)
    - Priority: 48,721,573 (≈38% worse)

    **Explanation:**  
    Even with a moderate spread of priorities, FIFO continues to outperform.  
    While priority scheduling theoretically should help high-priority requests complete sooner, the implementation cost—particularly maintaining a max-heap—adds significant overhead.  
    This overhead outweighs any benefit from request reordering.

3. **Test 3 — High Priority Spread (λ = 1.5)**

    - FIFO: **37,840,483** (best)
    - Priority: 42,640,374 (≈13% worse)
    - Preemptive: 55,770,271 (≈47% worse)

    **Explanation:**  
    This test introduces the highest concurrency and largest priority variation, where priority scheduling _should_ have the clearest benefit.  
    Instead:

    - Priority scheduling still cannot compensate for overhead.
    - Preemption performs worst due to task interruption, context switching, and re-enqueuing costs.
    - FIFO, with its simple O(1) enqueue/dequeue behavior, scales more efficiently.

### 5.2 Implications

The results highlight several important conclusions about scheduling for this specific workload:

-   **Simplicity is a performance advantage.**  
    FIFO’s constant-time operations make it highly efficient under heavy CPU load.

-   **Priority queue overhead is non-trivial.**  
    The O(log n) cost of maintaining a binary heap meaningfully increases latency.

-   **Preemption is expensive.**  
    Interrupting threads, saving state, and re-enqueuing adds substantial overhead—especially harmful in CPU-bound tasks.

-   **Workload characteristics matter.**  
    This experiment used **CPU-bound**, **uniform-cost**, **hash-search requests**.  
    Since every request takes roughly the same time, reordering them based on priority rarely improves overall performance.

### 5.3 Why Priority Scheduling Did Not Help

Although priority scheduling is beneficial in many systems, the following characteristics of this experiment are what probably limit it here:

1. **The queue and thread-pool implementations introduce overhead.**  
   Priority scheduling adds computational cost on each request; FIFO does not
2. **Requests are CPU-bound with no waiting or I/O overlap.**  
   There is no advantage to switching tasks mid-execution

3. **The weighted latency score penalizes latency, not throughput.**  
   Priority scheduling does not reduce total work—only rearranges it

These could be why FIFO = dominant performer

## 6. Conclusion

### 6.1 Hypothesis Evaluation and Outcome

The hypothesis that priority-aware scheduling would reduce the weighted latency score was **not supported**. Across all tests—uniform, mixed, and high-spread—**FIFO consistently produced the lowest weighted latency score**, outperforming both the static and preemptive priority schedulers.

-   **FIFO is the most efficient strategy** for this workload, with the lowest scores in all scenarios.
-   **Priority scheduling adds overhead** (heap operations, thread-pool coordination) that outweighs its benefits when all requests take roughly the same time to process.
-   **Preemption is the most expensive**, performing worst due to interruption and re-enqueue overhead.

This experiment uses **CPU-bound, uniform-cost tasks**. Since every request takes nearly the same amount of time, reordering them by priority does **not** meaningfully reduce latency. The extra computation required for priority handling lowers performance instead of improving it.

### 6.2 My Takeaways

-   For uniform, CPU-heavy workloads, **simple scheduling (FIFO)** is optimal.
-   Priority scheduling only helps when task durations vary or when delaying low-priority work meaningfully benefits high-priority work.
-   The experiment reinforces the importance of empirical testing rather than assuming priority scheduling will always improve performance.

---

# Fixes in Final Implementation (Based on experiment 4b)

_Conducted by Vincent Van Schependom (**s251739**)_

Lucas' multithreaded server was experiencing two major "freezing" issues. I identified two distinct bottlenecks and implemented solutions for each.

## 1. First Problem: The "Big Request" Freeze (Dispatcher Bottleneck)

### What Was Wrong?

When the server received a big (large number of requests) and hard (large bruteforce range) client request, it would become unresponsive.

-   **Cause:** A single worker thread would execute `bruteForceDispatcher`, which then entered a massive `for` loop to create and enqueue _all_ (potentially a really large number) of the work chunks at once.
-   **Effect:** This one thread **hijacked the queue's mutex**, constantly locking it to add new tasks. This created extreme **lock contention**, preventing other worker threads from dequeuing tasks. It also flooded the queue, **starving** any new client requests that came in.

### How I Fixed It

I refactored `bruteForceDispatcher` in `bruteforce.c` to work in batches.

1.  I defined a `MAX_CHUNKS_PER_DISPATCH` to limit the work done in one go.
2.  The dispatcher now enqueues just **one batch** of chunks, instead of all chunks at once.
3.  If more work remains, it **re-enqueues itself** with an updated starting point for the _next_ batch.
4.  This breaks the monolithic task (i.e. the massive `for` loop), frees the worker thread, and releases the queue lock, allowing other tasks (like new client requests) to be processed fairly.

## 2. Second Problem: The "Slow Client" Freeze (Main Thread Blocking)

### What Was Wrong?

The server would also freeze if a slow client connected but never sent any data.

-   **Cause:** The `main` server thread (the one responsible for `accept()`) was calling `handleRequest`. This function immediately called the **blocking I/O** function `recv_n` to get the 49-byte request.
-   **Effect:** If a client connected but never sent its data, the `main` thread would get stuck inside `recv_n` _forever_. Since the `main` thread was blocked, it could no longer call `accept()`, and the entire server stopped accepting **any new connections**.

### How I Fixed It

I refactored `handleRequest.c` to move all blocking I/O off the main thread.

1.  The `main` thread's `handleRequest` function is now 100% non-blocking. It _only_ creates a `ClientRequestData` struct and enqueues a new, simple task (`processClientConnection`) containing the `client_fd`.
2.  A **worker thread** from the pool picks up this initial task.
3.  This worker thread (not the main thread) is now responsible for calling the blocking `recv_n`.
4.  Once the worker successfully reads the data, _it_ then enqueues the _actual_ processing task (`handleClientThreadTasks`).

This change ensures the `main` thread is never blocked and is always free to accept new client connections.
