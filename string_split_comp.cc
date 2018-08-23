#include <benchmark/benchmark.h>
#include <sstream>
#include <algorithm>
#include <vector>
#include <string>
#include <experimental/functional>
#include <experimental/algorithm> // include experimental search
#include <functional> // include boyer_moore_searcher
#include <boost/algorithm/string.hpp>

using namespace std;

// string split with one char. Code from stackoverflow
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

// string split use `find`, emplace_back to call constructor directly.
vector<string> csplit(const string& origin, string delim)
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
            ret.emplace_back(origin, cur, origin.size() -cur);
            break;
        }
        // ret.emplace_back(origin.substr(cur, n - cur));
        ret.emplace_back(origin, cur, n - cur);
        cur = n + delim_size;
    }
    return ret;
}

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
            break;
        }
        ret.emplace_back(origin.substr(cur, n - cur));
        // ret.emplace_back(origin, cur, n - cur);
        cur = n + delim_size;
    }
    return ret;
}

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

// string concatenation
string catNTimes(string origin, size_t N)
{
  string ret;
  while(N--)
  {
    ret += origin;
  }
  return ret;
}

namespace {
  string to_seach = ";;";
  string string_base = "/url/to_parse_string;;port:1818;;ip:127.0.0.1;;count:19999;;some other string;;empty;;;;";
  string string_10 = catNTimes(string_base, 10);
  string string_100 = catNTimes(string_base, 100);
  string string_1000 = catNTimes(string_base, 1000);
  string string_10000 = catNTimes(string_base, 10000);
}


static void BM_SplitUsingStrFunc(benchmark::State& state,string & origin) {
  for (auto _ : state)
  {
      vector<string> vec;
      benchmark::DoNotOptimize(vec = split(origin, to_seach));
      // vec = split(origin, to_seach);
  }
}
BENCHMARK_CAPTURE(BM_SplitUsingStrFunc, string_base, string_base);
BENCHMARK_CAPTURE(BM_SplitUsingStrFunc, string_10, string_10);
BENCHMARK_CAPTURE(BM_SplitUsingStrFunc, string_100, string_100);
BENCHMARK_CAPTURE(BM_SplitUsingStrFunc, string_1000, string_1000);
BENCHMARK_CAPTURE(BM_SplitUsingStrFunc, string_10000, string_10000);


static void BM_SplitUsingStrFuncEmplaceOptim(benchmark::State& state,string & origin) {
  for (auto _ : state)
  {
      vector<string> vec;
      benchmark::DoNotOptimize(vec = csplit(origin, to_seach));
      // vec = csplit(origin, to_seach);
  }
}
BENCHMARK_CAPTURE(BM_SplitUsingStrFuncEmplaceOptim, string_base, string_base);
BENCHMARK_CAPTURE(BM_SplitUsingStrFuncEmplaceOptim, string_10, string_10);
BENCHMARK_CAPTURE(BM_SplitUsingStrFuncEmplaceOptim, string_100, string_100);
BENCHMARK_CAPTURE(BM_SplitUsingStrFuncEmplaceOptim, string_1000, string_1000);
BENCHMARK_CAPTURE(BM_SplitUsingStrFuncEmplaceOptim, string_10000, string_10000);

static void BM_SplitUsingSearch(benchmark::State& state,string & origin) {
  for (auto _ : state)
  {
      vector<string> vec;
      benchmark::DoNotOptimize(vec = asplit(origin, to_seach));
      // vec = asplit(origin, to_seach
  }
}
BENCHMARK_CAPTURE(BM_SplitUsingSearch, string_base, string_base);
BENCHMARK_CAPTURE(BM_SplitUsingSearch, string_10, string_10);
BENCHMARK_CAPTURE(BM_SplitUsingSearch, string_100, string_100);
BENCHMARK_CAPTURE(BM_SplitUsingSearch, string_1000, string_1000);
BENCHMARK_CAPTURE(BM_SplitUsingSearch, string_10000, string_10000);


static void BM_SplitUsingSOF(benchmark::State& state,string & origin) {
  for (auto _ : state)
  {
      vector<string> vec;
      benchmark::DoNotOptimize(vec = split(origin, ';'));
      // vec = split(origin, ';');
  }
}
BENCHMARK_CAPTURE(BM_SplitUsingSOF, string_base, string_base);
BENCHMARK_CAPTURE(BM_SplitUsingSOF, string_10, string_10);
BENCHMARK_CAPTURE(BM_SplitUsingSOF, string_100, string_100);
BENCHMARK_CAPTURE(BM_SplitUsingSOF, string_1000, string_1000);
BENCHMARK_CAPTURE(BM_SplitUsingSOF, string_10000, string_10000);

static void BM_SplitUsingBoost(benchmark::State& state,string & origin) {
  for (auto _ : state)
  {
      vector<string> vec;
      benchmark::DoNotOptimize(boost::split(vec,origin,boost::is_any_of(to_seach)));
      // boost::split(vec,origin,boost::is_any_of(to_seach))
  }
}
BENCHMARK_CAPTURE(BM_SplitUsingBoost, string_base, string_base);
BENCHMARK_CAPTURE(BM_SplitUsingBoost, string_10, string_10);
BENCHMARK_CAPTURE(BM_SplitUsingBoost, string_100, string_100);
BENCHMARK_CAPTURE(BM_SplitUsingBoost, string_1000, string_1000);
BENCHMARK_CAPTURE(BM_SplitUsingBoost, string_10000, string_10000);


static void BM_SplitUsingBMS(benchmark::State& state,string & origin) {
  for (auto _ : state)
  {
      vector<string> vec;
      benchmark::DoNotOptimize(vec = bsplit(origin, to_seach));
      //vec = bsplit(origin, to_seach)
  }
}
BENCHMARK_CAPTURE(BM_SplitUsingBMS, string_base, string_base);
BENCHMARK_CAPTURE(BM_SplitUsingBMS, string_10, string_10);
BENCHMARK_CAPTURE(BM_SplitUsingBMS, string_100, string_100);
BENCHMARK_CAPTURE(BM_SplitUsingBMS, string_1000, string_1000);
BENCHMARK_CAPTURE(BM_SplitUsingBMS, string_10000, string_10000);

BENCHMARK_MAIN();
