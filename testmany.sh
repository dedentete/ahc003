n=$(($1-1))
g++ -o main.out main.cpp
cd tools
for i in `seq 0 $n`
do
    t=$(printf "%04d" $i)
    if [ $i = 0 ]; then
        cargo run --release --bin tester in/$t.txt ../main.out > out/$t.txt 2> scores.txt
    else
        cargo run --release --bin tester in/$t.txt ../main.out > out/$t.txt 2>> scores.txt
    fi
done
wait
python calc.py