# Arena API

## Arena Level Files
| TYPE |   X   |   Y   |   W   |   H   |   R   |   G   |   B   |   A   |   BLOCK   |
|:----:|:-----:|:-----:|:-----:|:-----:|:-----:|:-----:|:-----:|:-----:|:---------:|
| char |  i32  |  i32  |  i32  |  i32  |  i32  |  i32  |  i32  |  i32  |     u8    |
|   R  |   0   |   0   |  100  |  100  |  255  |   0   |   0   |  100  |     1     |

> The file is being parsed line by line by builtin ifstream fixed size data parser