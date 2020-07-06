# CGAL 获取相关功能的依赖头文件

由于CGAL是header include only。只需要头文件就可以实现相关的功能。有时候为了实现一个简单的功能， 在研究具体实现的时候能够知道这个功能对应的头文件有哪些，就可以大概知道涉及到了哪些模块，可以从哪几个地方入手进行深入学习。于是此处给处理基于python，头文件依赖获取的示例脚本，如下：

```python
import os
import re

# Parameters
include_paths = [
    "C:/dev/CGAL-5.0.2/include/"
]

root_file = "./test.cpp"
out_file = "./out.txt"

# Global variables
include_patterns = [
    re.compile(r'.*#include.*<(.*)>.*'),
    re.compile(r'.*#include.*"(.*)"')
]

all_dependent_files = []

def get_all_include_in_one_file(filename):
    print(filename)
    if filename in all_dependent_files:
        return []
    all_dependent_files.append(filename)
    dependent_files = []
    with open(filename, 'r', encoding='utf8') as fid:
        for line in fid:
            line = line.strip()
            if line.find("#include") == -1:
                continue
            include_file = ""
            for pattern in include_patterns:
                match = pattern.match(line)
                if match:
                    include_file = match.group(1)
                    break
            if include_file == "":
                continue
            
            dependent_files.append(include_file)
    return dependent_files


def parse_header_files(filename):
    dependent_files = get_all_include_in_one_file(filename)
    if len(dependent_files) == 0:
        return
    for include_file in dependent_files:
        for include_path in include_paths:
            parse_file = os.path.join(include_path, include_file)
            if not os.path.isfile(parse_file):
                print(parse_file + " is not file")
            else:
                parse_header_files(parse_file)


parse_header_files(root_file)

with open(out_file, 'w', encoding="utf8") as fid:
    for line in all_dependent_files:
        fid.write(line + "\n")
```

