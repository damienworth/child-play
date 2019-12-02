# child-play

Playing around with C++17 features and experimenting

## file-io

I was wondering how would `std::filesystem` impact the size of binaries. So I created file-io folder here.
To compile **file-io** without `std::filesystem` run the following commands:
```
mkdir build && cd build
cmake -DWRITE_FILE=ON -DCMAKE_BUILD_TYPE=Release ..
make file-io
./file-io/file-io # no output, since this one just writes a dummy binary file
cmake -DWRITE_FILE=OFF ..
make file-io
./file-io/file-io # should output deadbeef
strip file-io/file-io
size file-io/file-io # this should output size of different sections of a program
```
To compile **file-io** with `std::filesystem` run the following commands:
```
mkdir build && cd build
cmake -DWRITE_FILE=ON -DCMAKE_BUILD_TYPE=Release ..
make file-io
./file-io/file-io # no output, since this one just writes a dummy binary file
cmake -DWRITE_FILE=OFF -DWITH_FILESYSTEM=ON ..
make file-io
./file-io/file-io # should output deadbeed
strip file-io/file-io
size file-io/file-io # this should output size of different sections of a program
```
The sizes I got on my machine with g++-8 on Ubuntu 18.04
**without** `std::filesystem`
```
size file-io/file-io 
   text	   data	    bss	    dec	    hex	filename
   7556	    840	    568	   8964	   2304	file-io/file-io
```
and **with** `std::filesystem`
```
size file-io/file-io 
   text	   data	    bss	    dec	    hex	filename
 136908	   1936	    568	 139412	  22094	file-io/file-io
```
