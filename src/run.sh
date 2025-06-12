# Remove output file if it exists
rm -f results.txt

# fp_1
echo "=== fp_1 --custom ==="    >> results.txt
bunzip2 -kc ../traces/fp_1.bz2 | ./predictor --custom         >> results.txt
echo "=== fp_1 --gshare:13 ===" >> results.txt
bunzip2 -kc ../traces/fp_1.bz2 | ./predictor --gshare:13      >> results.txt
echo "=== fp_1 --tournament:9:10:10 ===" >> results.txt
bunzip2 -kc ../traces/fp_1.bz2 | ./predictor --tournament:9:10:10 >> results.txt

# fp_2
echo "=== fp_2 --custom ==="    >> results.txt
bunzip2 -kc ../traces/fp_2.bz2 | ./predictor --custom         >> results.txt
echo "=== fp_2 --gshare:13 ===" >> results.txt
bunzip2 -kc ../traces/fp_2.bz2 | ./predictor --gshare:13      >> results.txt
echo "=== fp_2 --tournament:9:10:10 ===" >> results.txt
bunzip2 -kc ../traces/fp_2.bz2 | ./predictor --tournament:9:10:10 >> results.txt

# int_1
echo "=== int_1 --custom ==="    >> results.txt
bunzip2 -kc ../traces/int_1.bz2 | ./predictor --custom         >> results.txt
echo "=== int_1 --gshare:13 ===" >> results.txt
bunzip2 -kc ../traces/int_1.bz2 | ./predictor --gshare:13      >> results.txt
echo "=== int_1 --tournament:9:10:10 ===" >> results.txt
bunzip2 -kc ../traces/int_1.bz2 | ./predictor --tournament:9:10:10 >> results.txt

# int_2
echo "=== int_2 --custom ==="    >> results.txt
bunzip2 -kc ../traces/int_2.bz2 | ./predictor --custom         >> results.txt
echo "=== int_2 --gshare:13 ===" >> results.txt
bunzip2 -kc ../traces/int_2.bz2 | ./predictor --gshare:13      >> results.txt
echo "=== int_2 --tournament:9:10:10 ===" >> results.txt
bunzip2 -kc ../traces/int_2.bz2 | ./predictor --tournament:9:10:10 >> results.txt

# mm_1
echo "=== mm_1 --custom ==="    >> results.txt
bunzip2 -kc ../traces/mm_1.bz2 | ./predictor --custom         >> results.txt
echo "=== mm_1 --gshare:13 ===" >> results.txt
bunzip2 -kc ../traces/mm_1.bz2 | ./predictor --gshare:13      >> results.txt
echo "=== mm_1 --tournament:9:10:10 ===" >> results.txt
bunzip2 -kc ../traces/mm_1.bz2 | ./predictor --tournament:9:10:10 >> results.txt

# mm_2
echo "=== mm_2 --custom ==="    >> results.txt
bunzip2 -kc ../traces/mm_2.bz2 | ./predictor --custom         >> results.txt
echo "=== mm_2 --gshare:13 ===" >> results.txt
bunzip2 -kc ../traces/mm_2.bz2 | ./predictor --gshare:13      >> results.txt
echo "=== mm_2 --tournament:9:10:10 ===" >> results.txt
bunzip2 -kc ../traces/mm_2.bz2 | ./predictor --tournament:9:10:10 >> results.txt
