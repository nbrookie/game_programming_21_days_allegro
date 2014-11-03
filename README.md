Teach Yourself Game Programming in 21 Days (with Allegro 5)
===

![book image](http://ecx.images-amazon.com/images/I/510PF0KJ3QL.jpg)

This is a project that rewrites all of Andre Lamothe's examples in *Teach Yourself Game Programming in 21 Days* using the Allegro 5 library. Considering the book was written in 1994 using libraries compiled for DOS, this is quite an update!

I am also using this project to learn about both game and graphics programming in general. It has been fun to poke around and learn something new. Hope you enjoy!

*NOTE:* I have done all development on OS X Mavericks.

First install allegro 5 via homebrew:
```sh
brew tap homebrew/versions
brew install allegro5
```

Choose a tutorial file and compile via:

```sh
gcc -Wall file.c `pkg-config --cflags --libs allegro-5 allegro_main-5`
```


