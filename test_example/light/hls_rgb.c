#include "ya_common.h"
#include "hls_rgb.h"

#define SCALE   255
#define H_SCALE 180
#define H_GETA  1
#define GETA    1

double HLS2RGBvalue(double n1,double n2, double hue);

void HSVtoRGB(unsigned char*r, unsigned char*g, unsigned char*b, unsigned short int h, unsigned short int s, unsigned short int v)
{
	int i;
	float RGB_min, RGB_max;
	RGB_max = v*2.55f;
	RGB_min = RGB_max*(100 - s) / 100.0f;
	i = h / 60;
	int difs = h % 60;
	float RGB_Adj = (RGB_max - RGB_min)*difs / 60.0f;
	switch (i) {
		case 0:
			*r = RGB_max;
			*g = RGB_min + RGB_Adj;
			*b = RGB_min;
			break;
		case 1:
			*r = RGB_max - RGB_Adj;
			*g = RGB_max;
			*b = RGB_min;
			break;
		case 2:
			*r = RGB_min;
			*g = RGB_max;
			*b = RGB_min + RGB_Adj;
			break;
		case 3:
			*r = RGB_min;
			*g = RGB_max - RGB_Adj;
			*b = RGB_max;
			break;
		case 4:
			*r = RGB_min + RGB_Adj;
			*g = RGB_min;
			*b = RGB_max;
			break;
		default:
			*r = RGB_max;
			*g = RGB_min;
			*b = RGB_max - RGB_Adj;
			break;		

	}

}



void RGB2HSV( unsigned short int * h, unsigned short int * s, unsigned short int * v, unsigned char r,  unsigned char g,  unsigned char b)
{
    double rr, gg, bb;
    double hh, ss, vv;
    double cmax, cmin, cdes;
 
    rr = r;
    gg = g;
    bb = b;
 
    cmax = (rr > gg) ? rr : gg;
    if (bb > cmax) {
        cmax = bb;
    }
 
    cmin = (rr < gg) ? rr : gg;
    if (bb < cmin) {
        cmin = bb;
    }
 
    cdes = cmax - cmin;
    vv = cmax;
    if (cdes != 0) {
        ss = cdes * SCALE / cmax;
        if (cmax == rr) {
            hh = (gg - bb) * SCALE / cdes;
        }else if (cmax == gg) {
            hh = (bb - rr) * SCALE / cdes + 2 * H_SCALE;
        }else {
            hh = (rr - gg) * SCALE / cdes + 4 * H_SCALE;
        }
    }else if (cmax != 0) {
        ss = cdes * SCALE / cmax;
        hh = 0;
    }else {
        ss = 0;
        hh = 0;
    }
    if (hh < 0) {
        hh += 6 * H_SCALE;
    }
 
    *h = (unsigned short int)hh * H_GETA;
    *s = (unsigned short int)ss * H_GETA;
    *v = (unsigned short int)vv * H_GETA;
}
 
void HSV2RGB( unsigned char *r, unsigned char  *g, unsigned char  *b, unsigned short int h, unsigned short int s, unsigned short int v)
{
    double rr = 0, gg = 0, bb = 0;
    double hh, ss, vv;
 
    if (h == 6 * H_GETA * H_SCALE) {
        h = 0;
    }
    hh = (double)h / H_GETA;
    ss = (double)s / GETA;
    vv = (double)v / GETA;
 
    switch((int)(hh / H_SCALE)) {
        case 0:
            rr = SCALE;
            gg = hh;
            bb = 0;
            break;
        case 1:
            rr = 2 * H_SCALE - hh;
            gg = SCALE;
            bb = 0;
            break;
        case 2:
            rr = 0;
            gg = SCALE;
            bb = hh - 2 * H_SCALE;
            break;
        case 3:
            rr = 0;
            gg = 4 * H_SCALE - hh;
            bb = SCALE;
            break;
        case 4:
            rr = hh - 4 * H_SCALE;
            gg = 0;
            bb = SCALE;
            break;
        case 5:
            rr = SCALE;
            gg = 0;
            bb = 6 * H_SCALE - hh;
            break;
    }
 
    rr = (rr + (SCALE - rr) * (SCALE - ss) / SCALE) * vv / SCALE;
    gg = (gg + (SCALE - gg) * (SCALE - ss) / SCALE) * vv / SCALE;
    bb = (bb + (SCALE - bb) * (SCALE - ss) / SCALE) * vv / SCALE;
 
    *r =(unsigned char) rr;
    *g =(unsigned char)  gg;
    *b = (unsigned char) bb;
    if (*r > 255)*r = 255;
    if (*g > 255)*g = 255;
    if (*b > 255)*b = 255;
}
 
void RGB2HLS( double *h, double *l, double *s, unsigned char r, unsigned char g, unsigned char b)
{
    double dr = (double)r/255;
    double dg = (double)g/255;
    double db = (double)b/255;
    double cmax = MAX(dr, MAX(dg, db));
    double cmin = MIN(dr, MIN(dg, db));
    double cdes = cmax - cmin;
    double hh, ll, ss;
 
    ll = (cmax+cmin)/2;
    if(cdes){
        if(ll <= 0.5)
            ss = (cmax-cmin)/(cmax+cmin);
        else
            ss = (cmax-cmin)/(2-cmax-cmin);
 
        if(cmax == dr)
            hh = (0+(dg-db)/cdes)*60;
        else if(cmax == dg)
            hh = (2+(db-dr)/cdes)*60;
        else// if(cmax == b)
            hh = (4+(dr-dg)/cdes)*60;
        if(hh<0)
            hh+=360;
    }else
        hh = ss = 0;
 
    *h = hh;
    *l = ll;
    *s = ss;
}
 
void HLS2RGB( unsigned char *r, unsigned char *g, unsigned char *b, double h, double l, double s)
{
    double cmax,cmin;
 
    if(l <= 0.5)
        cmax = l*(1+s);
    else
        cmax = l*(1-s)+s;
    cmin = 2*l-cmax;
 
    if(s == 0){
        *r = *g = *b = l*255;
    }else{
        *r = HLS2RGBvalue(cmin,cmax,h+120)*255;
        *g = HLS2RGBvalue(cmin,cmax,h)*255;
        *b = HLS2RGBvalue(cmin,cmax,h-120)*255;
    }
}
 
double HLS2RGBvalue(double n1,double n2, double hue)
{
    if(hue > 360)
        hue -= 360;
    else if(hue < 0)
        hue += 360;
    if(hue < 60)
        return n1+(n2-n1)*hue/60;
    else if(hue < 180)
        return n2;
    else if(hue < 240)
        return n1+(n2-n1)*(240-hue)/60;
    else
        return n1;
}