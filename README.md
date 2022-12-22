# Test log [windows]

## More info and test log [ubuntu] in Github.Actions: [![statusbadge](../../actions/workflows/buildtest.yaml/badge.svg?branch=main&event=pull_request)](../../actions/workflows/buildtest.yaml)
        
### Compile failed (build log):
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