#!/bin/bash

BLOCK_SIZES=(64 128 256 512 1024 4096 16384)
RESULT_FILE="blocksize_results.csv"
REPEAT=3

# 输出表头
echo "block_size,time_ms,cycles,instructions,cache_misses" > $RESULT_FILE

for bs in "${BLOCK_SIZES[@]}"
do
    # 修改BLOCK_SIZE
    sed -i "s/static constexpr size_t BLOCK_SIZE = .*/static constexpr size_t BLOCK_SIZE = $bs;/" ../include/BaseOperator.h

    # 重新编译
    cd ../build
    make -j
    cd ../test

    sum_time=0
    sum_cycles=0
    sum_instructions=0
    sum_cache_misses=0

    for ((i=0;i<$REPEAT;i++)); do
        # 运行并收集perf数据
        perf stat -e cycles,instructions,cache-misses -o perf_out.txt ../build/weedb vec > prog_out.txt 2>&1

        # 提取perf数据
        cycles=$(grep "cycles" perf_out.txt | awk '{print $1}' | tr -d ,)
        instructions=$(grep "instructions" perf_out.txt | awk '{print $1}' | tr -d ,)
        cache_misses=$(grep "cache-misses" perf_out.txt | awk '{print $1}' | tr -d ,)

        # 提取weedb输出中的执行时间（假设输出有tVectorAtATime）
        time_ms=$(grep "RELATION_LEN" -A 1 prog_out.txt | tail -n 1 | awk -F, '{print $4}' | tr -d ' ')

        # 累加
        sum_time=$(echo "$sum_time + $time_ms" | bc)
        sum_cycles=$(echo "$sum_cycles + $cycles" | bc)
        sum_instructions=$(echo "$sum_instructions + $instructions" | bc)
        sum_cache_misses=$(echo "$sum_cache_misses + $cache_misses" | bc)
    done

    # 取均值
    avg_time=$(echo "scale=2; $sum_time/$REPEAT" | bc)
    avg_cycles=$(echo "$sum_cycles/$REPEAT" | bc)
    avg_instructions=$(echo "$sum_instructions/$REPEAT" | bc)
    avg_cache_misses=$(echo "$sum_cache_misses/$REPEAT" | bc)

    # 记录结果
    echo "$bs,$avg_time,$avg_cycles,$avg_instructions,$avg_cache_misses" >> $RESULT_FILE
done

echo "Results saved to $RESULT_FILE"