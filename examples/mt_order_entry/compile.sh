clang++ -fPIC -shared -undefined dynamic_lookup Market.cpp Order.cpp Util.cpp liquibookmodule.cpp \
-o liquibook.so --include-directory=../../src/ -std=c++14 \
-I/Library/Frameworks/Python.framework/Versions/3.6/include/python3.6m/ \
-L/Library/Frameworks/Python.framework/Versions/3.6/lib/ \
-lpython3.6

