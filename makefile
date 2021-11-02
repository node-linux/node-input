NODE=-I/usr/include/node -I$(CURDIR)node_modules/node-addon-api/ -I$(shell node -p "require('node-addon-api').include")
INCLUDE=$(NODE)
LIBS=$(shell pkg-config --cflags --libs libinput libudev)

BUILD_DIR=./build
SRC_DIR=./lib

input.node: clean lib.o
	g++ $(BUILD_DIR)/lib.o -shared -fPIC -o $(BUILD_DIR)/input.node -Wall -Wextra $(LIBS) $(INCLUDE)

clean:
	rm -rf $(BUILD_DIR)
	mkdir -p $(BUILD_DIR)

lib.o:
	g++ -c $(SRC_DIR)/lib.cpp -o $(BUILD_DIR)/lib.o -shared -fPIC $(LIBS) $(INCLUDE)

install:
	scp -i ~/.ssh/id_general -P 2222 ./build/input.node jcake@localhost:/home/jcake/node-linux/input/input.node
	scp -i ~/.ssh/id_general -P 2222 ./deploy.json jcake@localhost:/home/jcake/node-linux/input/package.json
	scp -i ~/.ssh/id_general -P 2222 ./test.js jcake@localhost:/home/jcake/node-linux/input/test.js
