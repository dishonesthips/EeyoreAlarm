# main compiler
CXX := g++

TARGET1 := Eeyore
TARGET2 := Log
TARGET3 := UserInfo
TARGET4 := Alarm

all: $(TARGET1)

$(TARGET1): 
	@echo "Compiling C program"
	$(CXX) $(CFLAGS) $(TARGET1).cpp -o $(TARGET1) $(LDFLAGS) -l$(LIB) -std=c++11

clean:
	@rm -rf $(TARGET1)
