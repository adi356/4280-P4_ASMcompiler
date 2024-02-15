# 4280 Project 4 ASM compiler

## How to run
1. run make clean command to clean any old builds
2. run make
3. start program by typing ./comp [testFile]

## Known tests that are failing

My logic seems to be messed up for recognizing LOAD which is causing the output for every test to be incorrect

Not properly pushing and popping to the stack

Problem with INn (?)

Failing the second test file due to a static semantics error -> slightly edited test to find error still failing

UPDATE: got it to recognize load, stack operation logic still seems incorrect but at least outputing correct number in VirtMach

## Task

### BNF

<program> -> <vars> xopen <stats> xclose

<vars>    -> empty | xdata <varList>

<varList> -> identifier : integer <Y>

<Y>       -> ; | <varList>

<exp>     -> <M> <X>

<X>       -> / <exp> | * <exp> | empty

<M>       -> <N> <Z>

<Z>       -> + <M> | empty

<N>       -> <R> - <N> | ~<N> | <R>

<R>       -> ( <exp> ) | identifier | integer

<stats>   -> <stat> <mStat>

<mStat>   -> empty | <stat> <mStat>

<stat>    -> <in> | <out> | <block> | <if> | <loop> | <assign>

<block>   -> {<vars> <stats>}

<in>      -> xin >> identifier;

<out>     -> xout << <exp>;

<if>      -> xcond [<exp> <RO> <exp>] <stat>

<loop>    -> xloop [<exp> <RO> <exp>] <stat>

<assign>  -> xlet identifier <exp>;

<R0>      -> <<(onetoken) | >> (one token) | < | >| = | %
