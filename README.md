# wg

A CLI to quickly choose which gpu to use.

## Quick Usage Guide

wg has three main functions:

1. `wg list_idle --idle <GB>` - List all available GPUs with at least a specified amount of idle memory.
2. `wg list_busy` - List all GPUs that currently have less than 50% of their memory free.
3. `wg choose_idle --idle <GB>` - Select a random GPU with at least a specified amount of idle memory.
   - This command can also be used as `CUDA_VISIBLE_DEVICES:$(wg choose_idle --idle <GB>)`

## Installation

This CLI is linux-only and requires `nvidia-smi` to be installed. Once you have `nvidia-smi` installed, you can install this CLI using `setup.sh`

```bash
chmod +x setup.sh && bash setup.sh
```

## More Usage

### `list_idle`

```bash
wg list_idle --idle <GB> [--rank] [--json]
```

List all available GPUs with at least `<GB>` of idle memory. The `--rank` option sorts the GPUs by their idle memory in descending order, and the `--json` option outputs the result in JSON format.

### `list_busy`

```bash
wg list_busy [--rank] [--json]
```

List all GPUs that currently have less than 50% of their memory free. The `--rank` option sorts the GPUs by their used memory in descending order, and the `--json` option outputs the result in JSON format.

### `choose_idle`

- Exports the chosen GPU as an environment variable `CUDA_VISIBLE_DEVICES`.

```bash
wg choose_idle --idle <GB> [--export]
```

Or you can use it in a command like this:

```bash
eval $(wg choose_idle --idle <GB> --export)
```

to modify the environment variable in the current shell.

- Injects the chosen GPU into a specified YAML file under a specified key.

```bash
wg choose_idle --idle <GB> --yaml <YAML_PATH> --key <YAML_KEY>
```
