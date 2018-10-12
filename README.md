# hello-plan9

A very simple GUI app for Plan 9 from Bell Labs. [This blog post](https://nspool.me/2013/02/bouncing-ball/) explains the code in more detail.

Just a bouncing ball with menu options. Using libdraw to animate the ball and libevent for the menu.

![Screenshot](ball.png?raw=true "Screenshot showing window, ball and Exit menu item")

## Build Instructions

To compile and link from a i386 Plan 9 terminal:

`8c -FTVw ball.c && 8l -o ball ball.8`
