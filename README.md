# Test log [windows]

## More info and test log [ubuntu] in Github.Actions: [![statusbadge](../../actions/workflows/buildtest.yaml/badge.svg?branch=main&event=pull_request)](../../actions/workflows/buildtest.yaml)

Build log (can be empty):
```
D:\a\spbsu-comp-arch22-lab4-Mell0r\spbsu-comp-arch22-lab4-Mell0r\measurements.cpp:20:15: warning: compound assignment to object of volatile-qualified type 'volatile double' is deprecated [-Wdeprecated-volatile]
                        total_time += finish - start;
                                   ^
D:\a\spbsu-comp-arch22-lab4-Mell0r\spbsu-comp-arch22-lab4-Mell0r\measurements.cpp:41:15: warning: compound assignment to object of volatile-qualified type 'volatile double' is deprecated [-Wdeprecated-volatile]
                        total_time += finish - start;
                                   ^
D:\a\spbsu-comp-arch22-lab4-Mell0r\spbsu-comp-arch22-lab4-Mell0r\measurements.cpp:60:14: warning: compound assignment to object of volatile-qualified type 'volatile double' is deprecated [-Wdeprecated-volatile]
                total_time += finish - start;
                           ^
3 warnings generated.

```

Stdout+stderr (./omp4 0 in.pgm out0.pgm):
```
OK [program completed with code 0]
    [STDERR]:  
    [STDOUT]: Time (0 thread(s)): 71.8227 ms
77 130 187

```
     
Stdout+stderr (./omp4 -1 in.pgm out-1.pgm):
```
OK [program completed with code 0]
    [STDERR]:  
    [STDOUT]: Time (-1 thread(s)): 21.1121 ms
77 130 187

```

Input image:

![Input image](test_data/in.png?sanitize=true&raw=true)

Output image 0:

![Output image 0](test_data/out0.pgm.png?sanitize=true&raw=true)

Output image -1:

![Output image -1](test_data/out-1.pgm.png?sanitize=true&raw=true)