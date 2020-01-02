# FlightGear
FlightGear project connects to "flightGear" simulator and send commands to the server,
in order to take off.

## Installation

1. Clone project:

```bash
git clone https://github.com/CohenOfir/FlightGear.git
```

2. Install [Flightgear](http://home.flightgear.org)

## Setup Simulator

1. Open FlightGear Simulator.
2. Click Settings
3. insert the following commands into Additional Settings box:

```bash
--telnet=socket,in,10,127.0.0.1,5402,tcp=8080
--generic=socket,out,10,127.0.0.1,5400,tcp,generic_small
```

### Run Insturctions:

1. Open terminal in project directory and type:

```bash
sudo g++ -std=c++14 *.cpp -Wall -Wextra -Wshadow -Wnon-virtual-dtor -pedantic -o a.out -pthread
```
After compilation, type:
```bash
./a.out
```
2. After the program is running, open the simulator and click **Fly!**

3. Safe Flight !
