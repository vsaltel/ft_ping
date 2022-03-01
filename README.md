# ft_ping

My version of the standard ping program.
Display route packets to network host.

42 Paris school network project.

## Build

Compilation :
```
make
```

Clean objects files :
```
make clean
```

Clean entire project :
```
make fclean
```

## Usage

```
./ft_ping [-vqD] [-c count] [-s packetsize] [-t ttl] host
```

## Options

- -v : Verbose mode
- -q : Quiet mode
- -D : Show packet sending timestamp
- -c count : Define the number of ICMP packet before exit
- -s packetsize : Define the size of the padding in the packet
- -t ttl : Define the farthest machine on the route before return
