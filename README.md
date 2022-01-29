# Approximation using Chebyshev polynomials

## About project

This was my frst bigger project at Warsaw University Of Technology. We were given a gaus folder and some basic code in files like main etc. It was our job to make this thing work
and optimize it, so there is no data leaking and that it runs quickly. 

## Table of content

- /gaus - Gaus method to solve linear equations
- /test - Data which we will be approximating (dane.&), gen.c is a generator for our data, you can use it as well for your own function (initially fifth degree polynomial)
- Notatki.md - I've decided to keep it there for all my Polish folks, who would like to see me and my friends notes for the project.
- Aproksymator_czebyszew.c - This file contains functions returning Chebyshev polynomials of a specific degree from given argument (x).
- main.c - main file
- makespl.&, splines.& - Here we calculate our slope factors for different base functions. (mathematical stuff)
- points.& - Reads and stores points from a data file.

## How to run it

I will explain how to correctly run it step by step, but before that make sure that you have gnuplot package installed.

1. Go to project folder
2. go to /gaus
3. Use `make` commend and cd back to project folder
4. Here use `make` as well


Now your program is compilled and you're good to run it. You can run `./aprox` to see help message that shows basic info on how to run the program. But here is the commend as well:

`./aprox -s spl -p test/dane.1 -g myplot`

This, will create two new files **spl** and **myplot**. Spl contains slope factors for Taylors method, and myplot contains points to plot.

Now we can see use gnuplot for our myplot. Below you can read how to view the plot on unix cli.

First things firt, we open gnuplot with command `gnuplot`. Make sure you have your terminal set to x11 (you can do it, by running `set term x11`).
Then we use `plot 'test/dane.1', 'myplot'`. This should open up a new window with our data from dane.1 and our aproximation.
