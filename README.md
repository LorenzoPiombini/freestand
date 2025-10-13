# FREESTAND 
___


I've been using C for the last year and a half, and as my understanding deepens, I've come across [null program](https://nullprogram.com/) website, and I sart reading about freestanding executables.

I've also heard other professionals talking about such a thing, and while I am not even near these profesionals abilities, I want to write my own, functionalities to achieve the same result.

what I've been using the most besides printf() and fprintf(), are strings habdling functions, 

like ***strncpy strncat strncmp strtok***, and so on.

I also use ***memset memcpy*** quite a bit, and if I want to achive my goal to make a free standing
executable, I have to rewrite all of these routines, which is also part of the fun. 

I've started with an implementation of ***memcpy*** and ***memset*** which i renamed:
- copy_memory()
- set_memory() 

the name is different from the original libc functions
because i want to avoid to get yelled at by gcc compiler.

I will implement string functionalities and maybe a string struct to make easy string handling in C, 
I have written a draft of such a thing in the [str_op.c] (https://github.com/LorenzoPiombini/Database/blob/main/src/str_op.c) file for the database utility.





