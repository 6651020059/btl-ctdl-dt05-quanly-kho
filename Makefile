CXX = g++
CXXFLAGS = -Iinclude -Wall -Wextra -std=c++11

# Tên file thực thi
TARGET = QuanLySieuThi

# Tìm tất cả file .cpp trong thư mục src
SRCS = $(wildcard src/*.cpp)
# Tạo danh sách file .o tương ứng
OBJS = $(SRCS:.cpp=.o)

# Luật mặc định để build chương trình
all: $(TARGET)

# Liên kết các file object thành file thực thi
$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^

# Biên dịch từng file .cpp thành file object .o
src/%.o: src/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Lệnh dọn dẹp (chạy bằng: make clean)
clean:
	rm -f src/*.o $(TARGET)

.PHONY: all clean
