# child-play

Playing around with C++17 features and experimenting

## file-io

I was wondering how would `std::filesystem` impact the size of binaries. So I created **file-io** folder here.
To compile **file-io** without `std::filesystem` run the following commands:
```console
monster@hamster:~/child-play$ mkdir build && cd build
monster@hamster:~/child-play/build$ cmake -DWRITE_FILE=ON -DCMAKE_BUILD_TYPE=Release ..
monster@hamster:~/child-play/build$ make file-io
monster@hamster:~/child-play/build$ ./file-io/file-io # no output, since this one just writes a dummy binary file
monster@hamster:~/child-play/build$ cmake -DWRITE_FILE=OFF ..
monster@hamster:~/child-play/build$ make file-io
monster@hamster:~/child-play/build$ ./file-io/file-io # should output deadbeef
monster@hamster:~/child-play/build$ strip file-io/file-io
monster@hamster:~/child-play/build$ size file-io/file-io # this should output size of different sections of a program
```
Then, to compile **file-io** with `std::filesystem` run the following commands:
```console
monster@hamster:~/child-play/build$ cmake -DWITH_FILESYSTEM=ON ..
monster@hamster:~/child-play/build$ make file-io
monster@hamster:~/child-play/build$ ./file-io/file-io # should output deadbeed
monster@hamster:~/child-play/build$ strip file-io/file-io
monster@hamster:~/child-play/build$ size file-io/file-io # this should output size of different sections of a program
```

## Results

The `size`s I got on my machine with g++-8 on Ubuntu 18.04
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
Conclusions are not implied here, so you may make them on your own.

Q: _Why does `std::filesystem` bloats the binary size that much?_

A: _Well, because it's a static library._
