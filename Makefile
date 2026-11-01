CXX      ?= g++
CPPFLAGS  = -Iinclude
CXXFLAGS ?= -std=c++11 -Wall -Wextra -O2
LDFLAGS  ?=
OBJDIR    = .build

APP_SRCS = src/main.cpp src/quan_ly_kho.cpp src/lap_hoa_don.cpp src/thongkedoanhthu.cpp
APP_OBJS = $(APP_SRCS:src/%.cpp=$(OBJDIR)/%.o)
BENCH_OBJ = $(OBJDIR)/benchmark.o
DEPS = $(APP_OBJS:.o=.d) $(BENCH_OBJ:.o=.d)
TARGET = main_app
BENCH = benchmark_app

# File tạm do các bài test sinh ra. Tuyệt đối không đụng tới kho.dat thật.
TEST_TMP = tmp_*.dat kho_test.dat *.dat.tmp ket_qua_test.txt

.PHONY: all test clean asan

all: $(TARGET) $(BENCH)

$(OBJDIR):
	mkdir -p $@

$(OBJDIR)/%.o: src/%.cpp | $(OBJDIR)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -MMD -MP -c $< -o $@

$(TARGET): $(APP_OBJS)
	$(CXX) $(LDFLAGS) $^ -o $@

$(BENCH): $(BENCH_OBJ)
	$(CXX) $(LDFLAGS) $^ -o $@

test: all
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) $(LDFLAGS) tests/T23_test_suite.cpp -o test_suite
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) $(LDFLAGS) tests/test_fileio.cpp -o test_fileio
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) $(LDFLAGS) tests/test_kho.cpp src/quan_ly_kho.cpp -o test_kho
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) $(LDFLAGS) tests/test_hoadon.cpp src/lap_hoa_don.cpp src/thongkedoanhthu.cpp -o test_hoadon
	./test_suite
	./test_fileio
	./test_kho
	./test_hoadon
	sh tests/test_khoi_dong.sh
	@rm -f $(TEST_TMP)

asan:
	$(MAKE) clean
	$(MAKE) CXXFLAGS="-std=c++11 -Wall -Wextra -O1 -g -fsanitize=address,undefined -fno-omit-frame-pointer" LDFLAGS="-fsanitize=address,undefined" test

clean:
	rm -rf $(OBJDIR)
	rm -f $(TARGET) $(BENCH) test_suite test_fileio test_kho test_hoadon $(TEST_TMP)

-include $(DEPS)
