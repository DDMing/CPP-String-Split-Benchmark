# CPP-String-Split-Benchmark

Recently, I need a string split function. For example, 

```c++
// example 1
string s1 = "11;22;33;44;55";
auto list1 = split(s1, ";");  // list1 = ["11","22","33", "44", "55"];

// example 2
string s2 = "||ONE||TWO||THREE||FOUR||";
auto list2 = split(s2, "||"); // list2 = ["", "ONE", "TWO", "THREE", "FOUR", ""];
```

So I did a search and get most votes answer from [`stackoverflow`](https://stackoverflow.com/questions/236129/the-most-elegant-way-to-iterate-the-words-of-a-string), the function use `std::getline()` to split string. But `std::getline` accept only one character as delimiter.

Recently, I spent a lot of time learning STL algorithm and STL container, so I am confident to write my own split function.

##### Test Environment

```
Apple LLVM version 9.1.0 (clang-902.0.39.2)
Target: x86_64-apple-darwin17.7.0
macOS Version: 10.13.6 
Boost Version: 1.67.0
```



##### Version 01.  `stackoverflow` (Only support char delimiter)

```c++
// https://stackoverflow.com/questions/236129/the-most-elegant-way-to-iterate-the-words-of-a-string
template<typename Out>
void split(const std::string &s, char delim, Out result) {
    std::stringstream ss(s);
    std::string item;
    while (std::getline(ss, item, delim)) {
        *(result++) = item;
    }
}

std::vector<std::string> split(const std::string &s, char delim) {
    std::vector<std::string> elems;
    split(s, delim, std::back_inserter(elems));
    return elems;
}
```

##### Version 02. Split using `std::string::find` and `string.substr`

```c++
// string split use find and substr.
vector<string> split(const string& origin, string delim)
{
    vector<string> ret;
    string::size_type n = 0;
    string::size_type cur = n;
    string::size_type delim_size = delim.size();
    while (true)
    {
        n = origin.find(delim, cur);
        if (n == string::npos)
        {
            ret.emplace_back(origin.substr(cur, origin.size()));
            // ret.emplace_back(origin, cur, origin.size()-cur); // Emplace Optimization
            break;
        }
        ret.emplace_back(origin.substr(cur, n - cur));
        // ret.emplace_back(origin, cur, n - cur); // Emplace Optimization
        cur = n + delim_size;
    }
    return ret;
}
```

##### Version 03.  Split using `std::search`

```c++
// string split using std::search
vector<string> asplit(const string& origin, string delim)
{
    vector<string> ret;
    auto itr = origin.begin();
    auto end1 = origin.end();
    auto start2 = delim.begin();
    auto end2 = delim.end();
    auto size = delim.length();
    while (true)
    {
        auto find_itr = search(itr, end1, start2, end2);
        if (find_itr == end1)
        {
            ret.emplace_back(itr, end1);
            break;
        }
        ret.emplace_back(itr, find_itr);
        itr = find_itr;
        itr += size;
    }
    return ret;
}
```

##### Version 04. (C++17 only) Split use `std::search` and `boyer_moore_searcher`

```c++
// string split using std::search & std::boyer_moore_searcher
vector<string> bsplit(const string& origin, string delim)
{
    vector<string> ret;
    auto itr = origin.begin();
    auto end1 = origin.end();
    auto start2 = delim.begin();
    auto end2 = delim.end();
    auto size = delim.length();
    while (true)
    {
        auto find_itr = std::experimental::search(itr, end1, std::experimental::boyer_moore_searcher(start2, end2));
        if (find_itr == end1)
        {
            ret.emplace_back(itr, end1);
            break;
        }
        ret.emplace_back(itr, find_itr);
        itr = find_itr;
        itr += size;
    }
    return ret;
}
```

##### Version 05.  Boost split

```c++
vector<string> vec;
boost::split(vec,origin,boost::is_any_of(to_seach));
```



#### Beachmark Result

##### Case 01. small string to split.

```c++
// two commas
string to_search = ";;";
string string_base = "How;; can s;;Ducument more than;;Sample sentence;;holly high happy;;conventional simple word;;just complexsentence;;c++/is/so/confused;;Good morning programmer.";
```

- Result

```
Running ./string_split_comp
Run on (4 X 2900 MHz CPU s)
CPU Caches:
  L1 Data 32K (x2)
  L1 Instruction 32K (x2)
  L2 Unified 262K (x2)
  L3 Unified 4194K (x1)
-------------------------------------------------------------------------------------
Benchmark                                              Time           CPU Iterations
-------------------------------------------------------------------------------------
BM_SplitUsingStrFunc/string_base                    1182 ns       1160 ns     578747
BM_SplitUsingStrFunc/string_10                      6296 ns       6064 ns      97895
BM_SplitUsingStrFunc/string_100                    47063 ns      47039 ns      14916
BM_SplitUsingStrFunc/string_1000                  458041 ns     456297 ns       1553
BM_SplitUsingStrFunc/string_10000                5037270 ns    5015461 ns        128
BM_SplitUsingStrFuncEmplaceOptim/string_base        1117 ns       1107 ns     624827
BM_SplitUsingStrFuncEmplaceOptim/string_10          5497 ns       5471 ns     128170
BM_SplitUsingStrFuncEmplaceOptim/string_100        46660 ns      46377 ns      14730
BM_SplitUsingStrFuncEmplaceOptim/string_1000      437674 ns     437278 ns       1554
BM_SplitUsingStrFuncEmplaceOptim/string_10000    5104739 ns    5048875 ns        144
BM_SplitUsingSearch/string_base                     1132 ns       1118 ns     609374
BM_SplitUsingSearch/string_10                       6161 ns       6106 ns     104392
BM_SplitUsingSearch/string_100                     51417 ns      50970 ns      13248
BM_SplitUsingSearch/string_1000                   468656 ns     468459 ns       1483
BM_SplitUsingSearch/string_10000                 5077659 ns    5073590 ns        134
BM_SplitUsingSOF/string_base                        2538 ns       2526 ns     278405
BM_SplitUsingSOF/string_10                         16000 ns      15968 ns      44449
BM_SplitUsingSOF/string_100                       152520 ns     151488 ns       4628
BM_SplitUsingSOF/string_1000                     1449540 ns    1445775 ns        471
BM_SplitUsingSOF/string_10000                   16327205 ns   16274286 ns         42
BM_SplitUsingBoost/string_base                      2127 ns       2123 ns     324693
BM_SplitUsingBoost/string_10                       10931 ns      10897 ns      62845
BM_SplitUsingBoost/string_100                      98452 ns      98090 ns       7156
BM_SplitUsingBoost/string_1000                    944929 ns     938676 ns        734
BM_SplitUsingBoost/string_10000                 11779040 ns   11674298 ns         57
BM_SplitUsingBMS/string_base                        5750 ns       5719 ns     122500
BM_SplitUsingBMS/string_10                         47580 ns      47078 ns      14764
BM_SplitUsingBMS/string_100                       471326 ns     463712 ns       1508
BM_SplitUsingBMS/string_1000                     4301429 ns    4296975 ns        158
BM_SplitUsingBMS/string_10000                   43948591 ns   43911625 ns         16
```

##### Case 02. Longer string.

```c++
// to_string is in the middle of string_base
string to_seach = "Sample sentence;;holly high happy;;conventional simple word;;just complexsentence;;";
string string_base = "How;; can s;;Ducument more than;;Sample sentence;;holly high happy;;conventional simple word;;just complexsentence;;c++/is/so/confused;;Good morning programmer.";
```

- Result

```
-------------------------------------------------------------------------------------
Benchmark                                              Time           CPU Iterations
-------------------------------------------------------------------------------------
BM_SplitUsingStrFunc/string_base                     640 ns        640 ns     850402
BM_SplitUsingStrFunc/string_10                      2526 ns       2521 ns     267536
BM_SplitUsingStrFunc/string_100                    16910 ns      16896 ns      41003
BM_SplitUsingStrFunc/string_1000                  153513 ns     153368 ns       4397
BM_SplitUsingStrFunc/string_10000                1582487 ns    1580780 ns        432
BM_SplitUsingStrFuncEmplaceOptim/string_base         624 ns        623 ns    1107367
BM_SplitUsingStrFuncEmplaceOptim/string_10          2457 ns       2455 ns     281827
BM_SplitUsingStrFuncEmplaceOptim/string_100        16853 ns      16838 ns      40600
BM_SplitUsingStrFuncEmplaceOptim/string_1000      155791 ns     155634 ns       4043
BM_SplitUsingStrFuncEmplaceOptim/string_10000    1596665 ns    1593074 ns        430
BM_SplitUsingSearch/string_base                      656 ns        655 ns    1036408
BM_SplitUsingSearch/string_10                       3217 ns       3211 ns     216744
BM_SplitUsingSearch/string_100                     22471 ns      22458 ns      30875
BM_SplitUsingSearch/string_1000                   210570 ns     210399 ns       3203
BM_SplitUsingSearch/string_10000                 2133935 ns    2132719 ns        320
BM_SplitUsingSOF/string_base                        2461 ns       2458 ns     278075
BM_SplitUsingSOF/string_10                         15491 ns      15477 ns      44141
BM_SplitUsingSOF/string_100                       143050 ns     142920 ns       4783
BM_SplitUsingSOF/string_1000                     1405069 ns    1404095 ns        475
BM_SplitUsingSOF/string_10000                   15930273 ns   15909571 ns         42
BM_SplitUsingBoost/string_base                     20463 ns      20447 ns      34018
BM_SplitUsingBoost/string_10                      173401 ns     173171 ns       4001
BM_SplitUsingBoost/string_100                    1758713 ns    1749636 ns        407
BM_SplitUsingBoost/string_1000                  18427938 ns   18411270 ns         37
BM_SplitUsingBoost/string_10000                189958493 ns  189860333 ns          3
BM_SplitUsingBMS/string_base                        2679 ns       2676 ns     260999
BM_SplitUsingBMS/string_10                         13991 ns      13981 ns      48737
BM_SplitUsingBMS/string_100                       122895 ns     122740 ns       5555
BM_SplitUsingBMS/string_1000                     1238467 ns    1235400 ns        568
BM_SplitUsingBMS/string_10000                   12049586 ns   12043298 ns         57
```

##### Case 03. Long string to split.

```c++
// to_search = 2*string_base
string to_seach = "How;; can s;;Ducument more than;;Sample sentence;;holly high happy;;conventional simple word;;just complexsentence;;c++/is/so/confused;;Good morning programmer.How;; can s;;Ducument more than;;Sample sentence;;holly high happy;;conventional simple word;;just complexsentence;;c++/is/so/confused;;Good morning programmer.";
 string string_base = "How;; can s;;Ducument more than;;Sample sentence;;holly high happy;;conventional simple word;;just complexsentence;;c++/is/so/confused;;Good morning programmer.";
```

- Result

```
-------------------------------------------------------------------------------------
Benchmark                                              Time           CPU Iterations
-------------------------------------------------------------------------------------
BM_SplitUsingStrFunc/string_base                     355 ns        354 ns    1945233
BM_SplitUsingStrFunc/string_10                       861 ns        859 ns     796922
BM_SplitUsingStrFunc/string_100                     2759 ns       2756 ns     247579
BM_SplitUsingStrFunc/string_1000                   19678 ns      19670 ns      35104
BM_SplitUsingStrFunc/string_10000                 207664 ns     207483 ns       3261
BM_SplitUsingStrFuncEmplaceOptim/string_base         343 ns        343 ns    1834593
BM_SplitUsingStrFuncEmplaceOptim/string_10           831 ns        829 ns     833175
BM_SplitUsingStrFuncEmplaceOptim/string_100         2708 ns       2704 ns     258779
BM_SplitUsingStrFuncEmplaceOptim/string_1000       18833 ns      18821 ns      36543
BM_SplitUsingStrFuncEmplaceOptim/string_10000     197722 ns     197560 ns       3431
BM_SplitUsingSearch/string_base                      338 ns        338 ns    2051907
BM_SplitUsingSearch/string_10                       1804 ns       1802 ns     386158
BM_SplitUsingSearch/string_100                     11699 ns      11680 ns      57942
BM_SplitUsingSearch/string_1000                   108066 ns     107944 ns       6294
BM_SplitUsingSearch/string_10000                 1077910 ns    1076318 ns        628
BM_SplitUsingSOF/string_base                        2539 ns       2537 ns     272558
BM_SplitUsingSOF/string_10                         15774 ns      15762 ns      42857
BM_SplitUsingSOF/string_100                       145368 ns     145005 ns       4696
BM_SplitUsingSOF/string_1000                     1419256 ns    1417667 ns        462
BM_SplitUsingSOF/string_10000                   16097516 ns   16077024 ns         42
BM_SplitUsingBoost/string_base                     31249 ns      31205 ns      22049
BM_SplitUsingBoost/string_10                      269402 ns     269099 ns       2556
BM_SplitUsingBoost/string_100                    2638651 ns    2636479 ns        261
BM_SplitUsingBoost/string_1000                  28572819 ns   28541208 ns         24
BM_SplitUsingBoost/string_10000                289028827 ns  288350500 ns          2
BM_SplitUsingBMS/string_base                        3468 ns       3465 ns     200872
BM_SplitUsingBMS/string_10                         20040 ns      20026 ns      34035
BM_SplitUsingBMS/string_100                       171210 ns     170182 ns       4053
BM_SplitUsingBMS/string_1000                     1601846 ns    1597927 ns        426
BM_SplitUsingBMS/string_10000                   16030974 ns   16018000 ns         42
```

#### Conclusion

- `std::find` and `std::search` is almost same. For long `string_base`,`std::find` is faster, for short `string_base`, `std::search` is faster. In my project, I use `std::search`.
- Util now, I have no idea why `boost` and `std::boyer_moore_searcher` is slower than 10 times.