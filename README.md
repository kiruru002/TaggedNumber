TaggedNumber は bool int float の3つ型をを扱うためのタグ付き共用体です
テキストから解釈した型と数値を保持します

テスト

```
#include <iostream>
#include "TaggedNumber.h"

int main()
{
    TaggedNumber num1(std::string("0"));
    TaggedNumber num2(std::string("0.0"));
    TaggedNumber num3(std::string("false"));

    std::cout << "num1: " << num1.toString() << " (Type: " << num1.getTypeString() << ")" << std::endl;
    std::cout << "num2: " << num2.toString() << " (Type: " << num2.getTypeString() << ")" << std::endl;
    std::cout << "num3: " << num3.toString() << " (Type: " << num3.getTypeString() << ")" << std::endl;
    
    return 0;
}

```

出力例
```
num1: 0 (Type: integer)
num2: 0.000000 (Type: float)
num3: false (Type: boolean)
```
