CXX      = g++
CXXFLAGS = -std=c++11 -Wall -Wextra -O2 -Iinclude

SRCS     = src/main.cpp src/quan_ly_kho.cpp src/lap_hoa_don.cpp src/thongkedoanhthu.cpp
HEADERS  = $(wildcard include/*.h include/*.tpp)
TARGET   = main_app
BENCH    = benchmark_app

.PHONY: all test clean

all: $(TARGET) $(BENCH)

$(TARGET): $(SRCS) $(HEADERS)
	$(CXX) $(CXXFLAGS) $(SRCS) -o $(TARGET)

$(BENCH): src/benchmark.cpp $(HEADERS)
	$(CXX) $(CXXFLAGS) src/benchmark.cpp -o $(BENCH)

test: all
	$(CXX) $(CXXFLAGS) tests/T23_test_suite.cpp -o test_suite
	$(CXX) $(CXXFLAGS) tests/test_fileio.cpp -o test_fileio
	$(CXX) $(CXXFLAGS) tests/test_kho.cpp src/quan_ly_kho.cpp -o test_kho
	./test_suite
	./test_fileio
	./test_kho
	@rm -f kho.dat rong.dat cut.dat sai.dat bad.dat ket_qua_test.txt

clean:
	rm -f $(TARGET) $(BENCH) test_suite test_fileio test_kho *.dat ket_qua_test.txt
