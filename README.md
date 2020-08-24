# STEGANO
#### An image manipulation app

This is Stegano, a pet project by [Piyush Aggarwal](https://www.google.com/search?q=piyush+aggarwal+taskmaster9001+brute4s99).

I found this project idea on a freelancing website and decided to give it a shot. There still might be some rough edges in the code, so tread with care!

## The What

- This application enables the user to hide an image inside a color, 32bit RGB image `(0xAARRGGBB)`.

- This application also enables the user to retrieve the hidden image and also separate out the modulation-free image.

- Modular code - simply reimplement the hideImage() and retrieveImage() functions to apply your own logic to the hiding and retrieval process! Go wild! :D

## The Why

for fun :D

## The How

- Data from every pixel from the payload image is distributed in the RGB pixel of the carrier image.

```
  8 bits of a byte in any image      -> [00] [01] [02] [03] [04] [05] [06] [07]

  The 8 bits of a B/W payload        -> [p0] [p1] [p2] [p3] [p4] [p5] [p6] [p7]

  Red Component of carrier image     -> [r0] [r1] [r2] [r3] [r4] [r5] [r6] [r7]
  Green Component of carrier image   -> [g0] [g1] [g2] [g3] [g4] [g5] [g6] [g7]
  Blue Component of carrier image    -> [b0] [b1] [b2] [b3] [b4] [b5] [b6] [b7]

  Red Component of modulated image   -> [r0] [r1] [r2] [r3] [r4] [  ] [  ] [  ]
  Green Component of modulated image -> [g0] [g1] [g2] [g3] [g4] [g5] [  ] [  ]
  Blue Component of modulated image  -> [b0] [b1] [b2] [b3] [b4] [  ] [  ] [  ]
```

> LSBs explained below. Keep reading!

-  One input from the user is number of bits to be used in RGB components of the modulated image

```
  ____________________________________________________________________________
  |                 |            Number of bits (input in GUI)               |
  |_________________|________________________________________________________|
  |  carrier bits   |        1         |        2         |        3         |
  |_________________|__________________|__________________|__________________|
  |                 |                  |                  |                  |
  | [r5] [r6] [r7]  |  [r5] [r6] [p2]  |  [r5] [p2] [p3]  |  [p2] [p3] [p4]  |
  | [g5] [g6] [g7] =|> [g5] [g6] [p0]  |  [g5] [p0] [p1]  |  [g5] [p0] [p1]  |
  | [b5] [b6] [b7]  |  [b5] [b6] [p5]  |  [b5] [p5] [p6]  |  [p5] [p6] [p7]  |
  |                 |                  |                  |                  |
  ____________________________________________________________________________
                        poor quality     moderate quality   lossless quality
                      (8 levels only)        (lossy)        (perfect hiding)
```

- Passing 1 as number of bits would be great to hide the image, but the result would be extremely unpredictable - we are operating on raw bits in the supplied logic.

```
So basically,   [p0][p1]  |  [p2][p3][p4]  |  [p5][p6][p7]
              _____________________________________________
                  green   |      red       |      blue
                component |       ,,       |       ,,
```

- In the steganography logic supplied with the project, the payload slice for the green component might seem to be complicating things a bit.
    >- Green component has been allotted the MSBs
    >- Green component has been chosen to store two bits
    - This has been done because human eyes are most sensitive to the color green. This is supported by the equation used by Qt Framework to generate a qGray() value ranging from 0-255 (8 bits) from a color image. [link](https://doc.qt.io/qt-5/qcolor.html#qGray)
    ```
    (r * 11 + g * 16 + b * 5)/32
    ```
    ^ that's 50% weightage to the color green!
    - Furthermore, allotting the MSBs gives the best assurity that these bits would be occupied in the least number of cases. Other bits have an equal probability but we can be more sure by looking at the image in this case.


## Inviting Contributions For

- add options that allow scaling the payload (+ whether to ignore AspectRatio) when hiding payload into carrier

- improve performance for number of bits = 1

- the supplied logic saves the black-and-white version of the payload in the carrier; add logic to store colored version of the image.
    - explore further to use the 0xAA bits (alpha value) within the carrier to hide more data. (that's 33% more space!)

- extend the software to work on videos - hiding one video into another.

## Concept Art

![concept_art.png](https://i.imgur.com/RLOfZ4L.jpg)