# BMP File generator
This project was made for System Programming course at the Faculty of Informatics of the University of Debrecen.

## Compile
```
 gcc main.c header.c -o chart
```

## Options:
```
--help        Lists the arguments that can be used
--version     It lists the version of the program, the name of the creator and the last day of modification
-send         Sets the program to send mode (default)
-receive      Sets the program to receive mode
-file         Set the sending method to file-based (default)
-socket       Set the sending method to socket-based
```

## Error Codes
```
1 - invalid filename
2 - no given command line arguments
3 - couldn't allocate memory
```