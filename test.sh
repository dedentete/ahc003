g++ -o main.out main.cpp
cd tools
cargo run --release --bin tester in/0000.txt ../main.out > out/0000.txt
cargo run --release --bin vis in/0000.txt out/0000.txt
cd ..