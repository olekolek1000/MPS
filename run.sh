
cmake CMakeLists.txt

if [ $? == "0" ]; then
	make -j 4
	if [ $? == "0" ]; then
		cd APP/macOS64/ && ./MPS64 ; cd -
	fi
fi
