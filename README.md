Note: The user "Titaniumtown" is now the primary maintainer of this repository; if you need to contact the owner contact "Titaniumtown".

# HDR+ Implementation
Original Document on the subject (by Timothy Brooks): http://timothybrooks.com/tech/hdr-plus

### Compilation instructions:
1. Install libraw, libpng, and libjpeg.¹
2. Download and compile llvm 3.9
3. Install or compile the latest version Halide
4. Go the folder you have the hdr-plus code in.
5. From the project root directory, run the following commands:
```
mkdir build
cd build
cmake -DHALIDE_DISTRIB_DIR=$(Insert halide directory) ..
make -j$(expr $(nproc) \+ 1)
```

### HDR+ algorithm examples:

Timothy Brooks provided images but they had to be taken off of Google Drive because the user titaniumtown doesn't have any space for it. If you have any suggestions, please contact titaniumtown.

### Compiled Binary Usage:
```
Usage: ./hdrplus [-c comp -g gain (optional)] dir_path out_img raw_img1 raw_img2 [...]
```

The -c and -g flags change the amount of dynamic range compression and gain respectively. Although they are optional because they both have default values. 

### Footnotes:
  
¹Also to install libraw, libpng, and libjpeg on macOS run ```brew install libraw libpng libjpeg```


Xavier:

cd build
cmake -DHALIDE_DISTRIB_DIR=/usr/local/Cellar/halide/10.0.0_1 ..
make -j$(expr $(nproc) \+ 1)

## TIFF tags:
tiffdump ../data/g1.ARW


//Under that tree
./hdrplus ../data dng i1.dng i2.dng i3.dng i4.dng i5.dng i6.dng i7.dng i8.dng i9.dng i10.dng

//room 
./hdrplus ../data dng r1.dng r2.dng r3.dng r4.dng r5.dng r6.dng r7.dng


//Paddy under the table
./hdrplus ../data png d1.dng d2.dng d3.dng d4.dng d5.dng d6.dng d7.dng

//Helicopter
./hdrplus ../data png h1.ARW h2.ARW h3.ARW h4.ARW h5.ARW h6.ARW h7.ARW h8.ARW h9.ARW h10.ARW h11.ARW h12.ARW h13.ARW h14.ARW h15.ARW h16.ARW h17.ARW
./hdrplus ../data dng h1.ARW h2.ARW h3.ARW h4.ARW h5.ARW h6.ARW h7.ARW h8.ARW h9.ARW h10.ARW h11.ARW h12.ARW h13.ARW h14.ARW h15.ARW h16.ARW h17.ARW

//Headphones
./hdrplus ../data dng hp.dng hp_1.dng hp_2.dng hp_3.dng hp_4.dng hp_5.dng hp_6.dng hp_7.dng hp_8.dng hp_9.dng hp_10.dng hp_11.dng


// Pot plant
./hdrplus ../data png pp1.ARW pp2.ARW pp3.ARW pp4.ARW pp5.ARW pp6.ARW pp7.ARW pp8.ARW pp9.ARW 

// Pot plant
./hdrplus ../data dng g1.ARW g2.ARW g3.ARW g4.ARW g5.ARW g6.ARW g7.ARW

//Smiles
./hdrplus ../data dng sm1.dng sm2.dng sm3.dng sm4.dng