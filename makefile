# main compiler
CXX := g++

TARGET1 := Eeyore
TARGET2 := Log
TARGET3 := UserInfo
TARGET4 := Alarm
TARGET5 := AlarmList

all: $(TARGET1)

$(TARGET1): 
	@echo "Compiling C program"
	$(CXX) $(CFLAGS) $(TARGET1).cpp $(TARGET2).cpp $(TARGET3).cpp $(TARGET4).cpp $(TARGET5).cpp -o $(TARGET1) $(LDFLAGS) -l$(LIB) -std=c++11

clean:
	@rm -rf $(TARGET1)
