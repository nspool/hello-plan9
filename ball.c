/*
 * Ball - a first attempt at Plan 9 graphics programming
 * Author:	nspool
 * Date:	2013-02-19
 * Licence: MIT
 * /

/* Build with '8c -FTVw ball.c && 8l -o ball ball.8' */

#include <u.h>
#include <libc.h>
#include <draw.h>
#include <event.h>

void moveball(void);
void initball(void);

Image *ball;

Point p;

/* Menus */
char *buttons[] = {"exit", 0};
Menu menu = { buttons };

/* Radius of ball */
int r = 20;
int borderWidth = 4;



/* Change direction of ball if collision
 * else move the ball and draw to screen
 * The new ball image draws over the previous image
 * so there is no need to redraw the whole screen */

void
moveball()
{
	static Point bp={6, 6}; /* Ball Position */
	static double Δi=4, Δj=4;

	/* Collision detection */
	if(bp.x > p.x - (r*3) || bp.x < -r) Δi = Δi*-1;
	if(bp.y > p.y - (r*3) || bp.y < -r) Δj = Δj*-1;
	
	/* Increment ball position */
	bp.x = bp.x + Δi;
	bp.y = bp.y + Δj;

	draw(screen, rectaddpt(screen->r, bp), ball, nil, ZP);
}



/* Graphics library requires this function */

void
eresized(int new)
{
	if(new && getwindow(display, Refnone) < 0)
		sysfatal("can't reattach to window");

	/* Store new screen coordinates for collision detection */
	p = Pt(Dx(screen->r), Dy(screen->r));

	/* Draw the background DWhite */
	draw(screen, insetrect(screen->r, borderWidth), 
			allocimage(display, Rect(0, 0, 1, 1), screen->chan, 1, DWhite), 
			nil, ZP);
}


/* Draw red ball inside a square of white background
 * When ball is drawn at new position, background will
 * blot out the previous image */

void
initball()
{
	Image *brush;
	brush=allocimage(display, Rect(0,0,1,1), CMAP8, 1, DRed);
	ball=allocimage(display, (Rectangle){(Point){0,0},(Point){r*4,r*4}},
			screen->chan, 0, DWhite);
	fillellipse(ball, (Point){r*2,r*2}, r, r, brush, ZP);
}


void
main(int argc, char *argv[])
{
	USED(argc, argv);

	Event ev;
	int e, timer;

	/* Initiate graphics and mouse */
	if(initdraw(nil, nil, "bouncing ball demo") < 0)
		sysfatal("initdraw failed: %r");

	einit(Emouse);

	/* Start our timer
	 * move the ball every 5 milliseconds
	 * unless there is an Emouse event */

	timer = etimer(0, 5);

	/* Simulate a resize event to draw the background
	 * and acquire the screen dimensions */

	eresized(0);

	initball();

	/* Main event loop */

	for(;;)
	{
		e = event(&ev);

		/* If there is a mouse event, the rightmost button
		 * pressed and the first menu option selected
		 * then exit.. */

		if( (e == Emouse) &&
			(ev.mouse.buttons & 4) && 
			(emenuhit(3, &ev.mouse, &menu) == 0)) exits(nil);
		else 
			if(e == timer)
				moveball();
	}
}



