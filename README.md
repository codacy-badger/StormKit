# StormKit 

[![Build status](https://ci.appveyor.com/api/projects/status/8e43otvwxd66nyo1?svg=true)](https://ci.appveyor.com/project/Arthapz/stormkit)

## Usefull links

- [StormKit git](http://git.arthapz.fr:3000/Arthapz/Stormkit)
- [StormKit documentation](http://doc.arthapz.fr/StormKit)
- [StormKit wiki](http://git.arthapz.fr:3000/Arthapz/Stormkit/wiki)

## Dependencies


- [LibPNG >= 1.6](http://www.libpng.org/pub/png/libpng.html)
- LibJPEG >= 6b
- glm
 
### configuration backend
There is multiple choice for the backend

- [Rapidjson >= 1.0.2](https://github.com/miloyip/rapidjson)
- Rapidxml
- vili

## Build instructions

Open terminal and download clone sources archive

```sh
git clone http://git.arthapz.fr:3000/Arthapz/Stormkit.git
```

Go to the root of sources and create a build directory

```sh
cd PATHOFSTORMKIT/
mkdir build && cd build
```

Generate the makefile, build, and install

```sh
cmake ../
make -j && make install
``` 

