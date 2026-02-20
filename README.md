# Battery Health Tracker

A lightweight Windows command-line utility written in C that tracks your laptop battery's health over time by logging its full charge capacity compared to its original design capacity.

## What It Does

- Generates a Windows battery report using `powercfg`
- Automatically opens the report in your browser
- Extracts the **design capacity** and **full charge capacity** from the report (in mWh)
- Calculates the battery health percentage
- Logs the result with a timestamp to a text file (`batt-health-log.txt`) in your user profile directory
- Opens the log file so you can see your history at a glance

On first run it creates the log file and records the design capacity as a baseline. Every subsequent run appends a new entry so you can track degradation over time.

## Example Log Output

```
DESIGN CAPACITY:        62290 mWh

FULL CHARGE CAPACITY:
                        61850 mWh  01-01-2025  99.29%
                        60100 mWh  15-06-2025  96.48%
                        58340 mWh  20-02-2026  93.66%
```

## Requirements

- Windows (uses `powercfg`, `SYSTEMTIME`, and other Windows APIs)
- GCC (e.g. via [MinGW](https://www.mingw-w64.org/) or [MSYS2](https://www.msys2.org/))
- Must be run with sufficient privileges for `powercfg` to generate a battery report (run as Administrator if needed)

## Files

| File | Description |
|------|-------------|
| `batt health.c` | Main source file |
| `appicon.opc` | Compiled resource object containing the application icon image, linked in at compile time |

## Compiling

```bash
gcc appicon.opc "batt health.c" -o "batt health.exe"
```

`appicon.opc` is a pre-compiled resource object file that embeds the application icon. It is linked alongside the source file so the resulting executable displays the icon in Windows Explorer.

## Usage

Simply run the executable:

```bash
"batt health.exe"
```

Or double-click it in Windows Explorer. The battery report and log file will open automatically.

## Log File Location

The log is saved to:

```
%USERPROFILE%\batt-health-log.txt
```

## License

Feel free to use and modify this project as you like.
