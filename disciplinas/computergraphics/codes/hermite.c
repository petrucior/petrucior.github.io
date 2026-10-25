#include <stdio.h>

#define width 500
#define height 500

typedef struct{
  float x, y;
}Point;

unsigned char img [width][height][3];

void px( int x, int y, unsigned char r, unsigned char g, unsigned char b ){
  if ((x >= 0) && (x <= width) && (y >= 0) && (y <= height)){
    img[x][y][0] = r;
    img[x][y][1] = g;
    img[x][y][2] = b;
  }
}

void clr(){
  for (int i = 0; i < width; i++)
    for (int j = 0; j < height; j++)
      px( i, j, 255, 255, 255 );
}

void save(){
  printf("P3 \n %d \t %d \n 255 \n", height, width );
  for (int i = 0; i < width; i++){
    for (int j = 0; j < height; j++){
      for (int c = 0; c < 3; c++){
	printf("%d \t", img[i][j][c]);
      }
      printf("\n");
    }
  }
}

float h1( float t ){ return 2*(t*t*t) - 3*(t*t) + 1; }
float h2( float t ){ return -2*(t*t*t) + 3*(t*t); }
float h3( float t ){ return (t*t*t) - 2*(t*t) + t; }
float h4( float t ){ return (t*t*t) - (t*t); }

Point hermite( float t, Point p0, Point p1, Point t0, Point t1 ){
  Point result;
  result.x = h1(t) * p0.x + h2(t) * p1.x + h3(t) * t0.x + h4(t) * t1.x;
  result.y = h1(t) * p0.y + h2(t) * p1.y + h3(t) * t0.y + h4(t) * t1.y;

  return result;
}

int main(){
  clr();

  Point p1 = {100, 100};
  Point p2 = {300, 300};
  Point t1 = {200, 0};
  Point t2 = {0, 200};

  for (float t = 0.0; t <= 1.0; t = t + 0.001){
    Point h = hermite( t, p1, p2, t1, t2 );
    px( (int)h.x, (int)h.y, 255, 0, 0 ); 
  }
  
  save();
}
