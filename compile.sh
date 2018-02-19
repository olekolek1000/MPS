
cmake CMakeLists.txt

if [ $? == "0" ]; then
	make -j 4
fi
