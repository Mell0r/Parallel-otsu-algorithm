# Test log [windows]

## More info and test log [ubuntu] in Github.Actions: [![statusbadge](../../actions/workflows/buildtest.yaml/badge.svg?branch=main&event=pull_request)](../../actions/workflows/buildtest.yaml)
        
### Compile failed (build log):
```
D:\a\spbsu-comp-arch22-lab4-Mell0r\spbsu-comp-arch22-lab4-Mell0r\LabOpenMp\bit_io.cpp:6:22: warning: function previously declared with an explicit exception specification redeclared with an implicit exception specification [-Wimplicit-exception-spec-mismatch]
    noexcept_stream::~noexcept_stream() {
                     ^
D:\a\spbsu-comp-arch22-lab4-Mell0r\spbsu-comp-arch22-lab4-Mell0r\LabOpenMp/bit_io.hpp:19:17: note: previous declaration is here
        virtual ~noexcept_stream() noexcept;
                ^
1 warning generated.
D:\a\spbsu-comp-arch22-lab4-Mell0r\spbsu-comp-arch22-lab4-Mell0r\LabOpenMp\hard.cpp:7:10: warning: non-portable path to file '"bit_io.hpp"'; specified path differs in case from file name on disk [-Wnonportable-include-path]
#include "bit_IO.hpp"
         ^~~~~~~~~~~~
         "bit_io.hpp"
1 warning generated.

```