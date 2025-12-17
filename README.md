# WareHouse Simulation

C++11 console simulation of a warehouse dispatch center. A config file seeds customers (soldier/civilian) and volunteers (collectors/drivers, limited or unlimited). At runtime you can place orders, advance time, inspect status, and back up/restore the warehouse state.

## Prerequisites
- Windows with a C++ toolchain (`g++` with C++11 support).
- `make` (or `mingw32-make`) on PATH.

## Build
From the project root:
```pwsh
make        # or: mingw32-make
```

## Run
Pass a config file path (sample: `src/configfile.txt`):
```pwsh
.\bin\main.exe .\src\configfile.txt
```
After “Warehouse is open!” enter interactive commands.

## Clean
```pwsh
make clean   # removes bin/*
```

## Runtime commands
Type these while the program is running:
- `order <customerId>`
- `orderStatus <orderId>`
- `step <n>`
- `volunteerStatus <volunteerId>`
- `customer <name> soldier|civilian <distance> <maxOrders>`
- `customerStatus <customerId>`
- `log`
- `backup`
- `restore`
- `close`

## Config file format
Each line defines one entity:
- `customer <name> soldier|civilian <distance> <maxOrders>`
- `volunteer <name> collector <cooldown>`
- `volunteer <name> limited_collector <cooldown> <maxOrders>`
- `volunteer <name> driver <maxDistance> <distancePerStep>`
- `volunteer <name> limited_driver <maxDistance> <distancePerStep> <maxOrders>`

Example snippet (see `src/configfile.txt`):
```
customer Moshe soldier 3 2
customer Ron civilian 2 1
volunteer Tamar collector 2
volunteer Tal driver 7 4
```

## Quick sanity script
After launching with the sample config, paste:
```
order 0
order 1
step 3
orderStatus 0
orderStatus 1
log
close
```
This places two orders, advances time, shows status, prints the log, and closes.
