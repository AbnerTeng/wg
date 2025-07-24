# wg

A CLI to choose which gpu to use

## Quick Usage Guide

List all available GPUs with specific minimum idle memory:

```bash
wg list --idle <GB>
```

Above usage with ranking by idle memory:

```bash
wg list --idle <GB> --rank
```

Select a random GPU with at least a specified amount of idle memory:

```bash
wg choose --idle <GB>

# And this will return a string like cuda:0
```

Above usage with direct export with `CUDA_VISIBLE_DEVICES`:

```bash
wg choose -idle <GB> --export
```

## Installation

This CLI is linux-only and requires `nvidia-smi` to be installed. Once you have `nvidia-smi` installed, you can install this CLI using `setup.sh`
```bash

chmod +x setup.sh && bash setup.sh
```

