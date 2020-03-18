//
// Created by find on 16-7-19.
// cache line = cache block = 原代码里的cache item ~= cache way
//
#ifndef CACHE_SIM 
#define CACHE_SIM
typedef unsigned char _u8;
typedef unsigned int _u32;
typedef unsigned long long _u64;

const unsigned char CACHE_FLAG_VALID = 0x01;
const unsigned char CACHE_FLAG_DIRTY = 0x02;
const unsigned char CACHE_FLAG_MASK = 0xff;

const char OPERATION_READ = 'l';
const char OPERATION_WRITE = 's';

#include "map"
#include<iostream>
#include<vector>
#include <assert.h>
#include <list>
#include <unordered_map>

/**替换算法*/
enum cache_swap_style {
    CACHE_SWAP_LRU,
    CACHE_SWAP_FRU,
    CACHE_SWAP_RAND,
    CACHE_SWAP_SRRIP,
    CACHE_SWAP_SRRIP_FP,
    CACHE_SWAP_BRRIP,
    CACHE_SWAP_DRRIP
};

class Cache_Line {
public:
    _u64 tag;
    _u8 flag;
    _u32 LRU;
    _u32 RRPV;
    _u32 FRU;
};

class CacheSim {
public:
    /**真正的cache地址列。指针数组*/
    Cache_Line *caches;
    /**cache的总大小，单位byte*/
    _u64 cache_size;
    /**cache line(Cache block)cache块的大小*/
    _u64 cache_line_size;
    /**总的行数*/
    _u64 cache_line_num;
    /**每个set有多少way*/
    _u64 cache_mapping_ways;
    /**整个cache有多少组*/
    _u64 cache_set_size;
    /**2的多少次方是set的数量，用于匹配地址时，进行位移比较*/
    _u64 cache_set_shifts;
    /**2的多少次方是line的长度，用于匹配地址*/
    _u64 cache_line_shifts;
    /**缓存替换算法*/
    int swap_style;

    /**指令计数器*/
    _u64 tick_count;

    /**读写内存的指令计数*/
    _u64 cache_r_count, cache_w_count;
    /**实际写内存的计数，cache --> memory */
    _u64 cache_w_memory_count;
    /**实际读内存的计数，memory --> cache */
    _u64 cache_r_memory_count;
    /**cache hit和miss的计数*/
    _u64 cache_hit_count, cache_miss_count;
    /**分别统计读写的命中次数*/
    _u64 cache_r_hit_count, cache_w_hit_count,cache_w_miss_count, cache_r_miss_count;

    /** SRRIP算法中的2^M-1 */
    _u32 SRRIP_2_M_1;
    /** SRRIP算法中的2^M-2 */
    _u32 SRRIP_2_M_2;
    /** SRRIP 替换算法的配置，M值确定了每个block替换权重RRPV的上限*/
    int SRRIP_M;
    /** BRRIP的概率设置，论文里设置是1/32*/
    double EPSILON = 1.0 / 32;
    /** DRRIP算法中的single policy selection (PSEL) counter*/
    long long PSEL;
    int cur_win_replace_policy;

    /** 写miss时，将数据读入cache */
    int write_allocation;
    /**向cache写数据的时候，向memory也写一份。=0为write back*/
    int write_through;
    
    CacheSim();

    ~CacheSim();

    void init(_u64 a_cache_size, _u64 a_cache_line_size, _u64 a_mapping_ways, cache_swap_style a_swap_style);
    void set_M(int m);

     // 检查是否命中
    int cache_check_hit(_u64 set_base, _u64 addr);
    /**获取cache当前set中空余的line*/
    int cache_get_free_line(_u64 set_base);
    /**使用指定的替换策略获取cache当前set中一个被替换的line*/
    int cache_find_victim(_u64 set_base, int a_swap_style, int hit_index);
    /**命中，更新Cache状态*/
    void cache_hit(_u64 set_base, _u64 index, int a_swap_style);
    /**缺失，插入新行后更新Cache状态*/
    void cache_insert(_u64 set_base, _u64 index, int a_swap_style);
    void dump_cache_set_info(_u64 set_base);

    /**对一个指令进行分析*/
    void do_cache_op(_u64 addr, char oper_style);

    /**读入trace文件*/
    void load_trace(const char *filename);

    /**@return 返回miss率*/
    double get_miss_rate();
    double get_hit_rate();

    /** 计算int的次幂*/
    _u32 pow_int(int base, int expontent);
    _u64 pow_64(_u64 base, _u64 expontent);

    /**判断当前set是不是选中作为sample的set，并返回给当前set设置的policy*/
    int get_set_flag(_u64 set_base);
    void re_init();
};

#endif
