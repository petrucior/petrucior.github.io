#include <stdio.h>

#define width 300
#define height 300

typedef struct{
  float x, y;
}Point;

typedef struct{
  float x, y, z;
}Point3D;

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

Point hermite( float t, Point p0, Point p1, Point t0, Point t1 ){
  float h1t = 2*(t*t*t) - 3*(t*t) + 1;
  float h2t = -2*(t*t*t) + 3*(t*t);
  float h3t = (t*t*t) - 2*(t*t) + t;
  float h4t = (t*t*t) - (t*t);

  Point result;
  result.x = h1t * p0.x + h2t * p1.x + h3t * t0.x + h4t * t1.x;
  result.y = h1t * p0.y + h2t * p1.y + h3t * t0.y + h4t * t1.y;

  return result;
}

float h1( float t ){ return 2*(t*t*t) - 3*(t*t) + 1; }
float h2( float t ){ return -2*(t*t*t) + 3*(t*t); }
float h3( float t ){ return (t*t*t) - 2*(t*t) + t; }
float h4( float t ){ return (t*t*t) - (t*t); }

Point3D hermite3D( float u, float v, Point3D p0, Point3D p1, Point3D p2, Point3D p3,
		   Point3D s0, Point3D s1, Point3D s2, Point3D s3,
		   Point3D r0, Point3D r1, Point3D r2, Point3D r3,
		   Point3D t0, Point3D t1, Point3D t2, Point3D t3 ){
  Point3D result;
  result.x =
    h1(u)*h1(v)*p0.x + h1(u)*h2(v)*p1.x + h2(u)*h1(v)*p2.x + h2(u)*h2(v)*p3.x +
    h1(u)*h3(v)*s0.x + h1(u)*h4(v)*s1.x + h2(u)*h3(v)*s2.x + h2(u)*h4(v)*s3.x +
    h3(u)*h1(v)*r0.x + h3(u)*h2(v)*r1.x + h4(u)*h1(v)*r2.x + h4(u)*h2(v)*r3.x +
    h3(u)*h3(v)*t0.x + h3(u)*h4(v)*t1.x + h4(u)*h3(v)*t2.x + h4(u)*h4(v)*t3.x;

  result.y =
    h1(u)*h1(v)*p0.y + h1(u)*h2(v)*p1.y + h2(u)*h1(v)*p2.y + h2(u)*h2(v)*p3.y +
    h1(u)*h3(v)*s0.y + h1(u)*h4(v)*s1.y	+ h2(u)*h3(v)*s2.y + h2(u)*h4(v)*s3.y +	
    h3(u)*h1(v)*r0.y + h3(u)*h2(v)*r1.y	+ h4(u)*h1(v)*r2.y + h4(u)*h2(v)*r3.y +
    h3(u)*h3(v)*t0.y + h3(u)*h4(v)*t1.y	+ h4(u)*h3(v)*t2.y + h4(u)*h4(v)*t3.y;

  result.z =
    h1(u)*h1(v)*p0.z + h1(u)*h2(v)*p1.z + h2(u)*h1(v)*p2.z + h2(u)*h2(v)*p3.z +
    h1(u)*h3(v)*s0.z + h1(u)*h4(v)*s1.z	+ h2(u)*h3(v)*s2.z + h2(u)*h4(v)*s3.z +	
    h3(u)*h1(v)*r0.z + h3(u)*h2(v)*r1.z	+ h4(u)*h1(v)*r2.z + h4(u)*h2(v)*r3.z +
    h3(u)*h3(v)*t0.z + h3(u)*h4(v)*t1.z	+ h4(u)*h3(v)*t2.z + h4(u)*h4(v)*t3.z;

  return result;
}

int main(){
  clr();

  /*Point p1 = {100, 100};
  Point p2 = {400, 400};
  Point t1 = {200, 0};
  Point t2 = {0, 200};

  for (float t = 0.0; t <= 1.0; t = t + 0.001){
    Point h = hermite( t, p1, p2, t1, t2 );
    px( (int)h.x, (int)h.y, 255, 0, 0 ); 
  }*/

  /*Point3D p0 = {0, 0, 0};
  Point3D p1 = {0, 100, 0};
  Point3D p2 = {100, 0, 0};
  Point3D p3 = {100, 100, 100};

  Point3D s0 = {0, 100, 0};
  Point3D s1 = {0, 100, 0};
  Point3D s2 = {0, 100, 0};
  Point3D s3 = {0, 100, 100};

  Point3D r0 = {100, 0, 0};
  Point3D r1 = {100, 0, 0};
  Point3D r2 = {100, 0, 100};
  Point3D r3 = {100, 0, 100};

  Point3D t0 = {0, 0, 0};
  Point3D t1 = {0, 0, 0};
  Point3D t2 = {0, 0, 0};
  Point3D t3 = {0, 0, 0};*/

  // 1. Cantos da superfície (posição x, y, z)
  Point3D p0 = {0, 0, 0};       // Canto inferior esquerdo
  Point3D p1 = {0, 200, 30};    // Canto superior esquerdo (com elevação)
  Point3D p2 = {200, 0, 30};    // Canto inferior direito (com elevação)
  Point3D p3 = {200, 200, 80};  // Canto superior direito (pico mais alto)

  // 2. Tangentes na direção u (derivadas parciais em relação a u)
  Point3D s0 = {50, 0, 40};
  Point3D s1 = {50, 0, 40};
  Point3D s2 = {50, 0, 40};
  Point3D s3 = {50, 0, 60};

  // 3. Tangentes na direção v (derivadas parciais em relação a v)
  Point3D r0 = {0, 50, 40};
  Point3D r1 = {0, 50, 40};
  Point3D r2 = {0, 50, 40};
  Point3D r3 = {0, 50, 60};

  Point3D t0 = {0, 0, 0};
  Point3D t1 = {0, 0, 0};
  Point3D t2 = {0, 0, 0};
  Point3D t3 = {0, 0, 0};

  float d = 200.0;
  float scale = 1.2;

  for (float u = 0.0; u <= 1.0; u = u + 0.02){
    for (float v = 0.0; v <= 1.0; v = v + 0.02 ){
      Point3D h3 = hermite3D( u, v, p0, p1, p2, p3, s0, s1, s2, s3, r0, r1, r2, r3, t0, t1, t2, t3 );
      // projecao em perspectiva
      int x_proj = (int)(/*(width/2) +*/ (h3.x /* scale*/ * d)/(d + h3.z));
      int y_proj = (int)(/*(height/2) +*/ (h3.y /* scale*/ * d)/(d + h3.z));
      px( x_proj, y_proj, 255, 0, 0 );
    }
  }
  
  save();
}
