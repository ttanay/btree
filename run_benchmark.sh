#!/bin/bash

dirty=''
if [[ $(git diff --stat) != '' ]]; then
  dirty='-dirty'
fi
head=$(git rev-parse HEAD)
filepath="./benchmarks/${head}${dirty}.log"

mkdir -p ./benchmarks

./build/btree_benchmark --benchmark_time_unit=ms --benchmark_min_time=10 --benchmark_out=${filepath} --benchmark_out_format=json
