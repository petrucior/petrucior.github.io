/**
 * \file canoa.pov
 *
 * \brief Arquivo para renderizacao de uma canoa.
 *
 * \author
 * Friedrich A. Lohmueller
 * http://www.f-lohmueller.de/pov_tut/x_sam/tec_550e.htm
 *
 * \modified by
 * Petrucio Ricardo Tavares de Medeiros \n
 * Universidade Federal do Rio Grande do Norte \n
 * Departamento de Computacao e Automacao Industrial \n
 * petrucior at gmail (dot) com
 *
 * \version 1.0
 * \date November 2015
 */

#global_settings{ assumed_gamma 1.0 }
#default{ finish{ ambient 0.1 diffuse 0.7 }}

// Arquivos de descricao de cores, formas e texturas
#include "colors.inc"
#include "shapes.inc"
#include "textures.inc"

// Camera
camera{
	location <0, 1.5, -8>	// Posicao da camera
	look_at <0.4, 0.2, 0>	// Posicao de visualizacao
}

// Fonte de luz
light_source{<1500,2500,-2500> color rgb<1,0.9,0.8>}

// Ceu
sky_sphere{
	/// Cor azul com gradiente
	pigment{
		gradient y	// Gradiente em torno do eixo y
		// Mapa de cores
		color_map{
			[0.000 0.002 color rgb <1.0, 0.2, 0.0> color rgb <1.0, 0.2, 0.0>]
			[0.002 0.200 color rgb <0.8, 0.1, 0.0> color rgb <0.2, 0.2, 0.3>]
		}
	scale 2			// Escala de distribuicao do mapa e gradiente pela esfera
	translate -1		// Translacao -1 em todas as direcoes
	}
	// Nuvens
	pigment {
		bozo		// Função randomica de ruido que tradicionalmente modela as nuvens
		// Variaveis para denotar uma grandeza aleatória
		turbulence 0.65
		octaves 6
		omega 0.7
		lambda 2
		// Mapa de cores
		color_map {
			  [0.0 0.1 color rgb <0.85, 0.85, 0.85> color rgb <0.75, 0.75, 0.75>]
			  [0.1 0.5 color rgb <0.75, 0.75, 0.75> color rgbt <1, 1, 1, 1>]
			  [0.5 1.0 color rgbt <1, 1, 1, 1> color rgbt <1, 1, 1, 1>]
		}
		// Escala de distribuicao do mapa pela esfera
		scale <0.2, 0.5, 0.2>
	}
	rotate -135*x
}

// Plano onde sera inserida a canoa e a agua
plane{
	<0,1,0>, 1 // Plano xz (normal para a coordenada y)
	hollow	    // Plano dentro da esfera
	texture{
		pigment{
			bozo
			turbulence 0.92
			color_map{ [0.00 rgb <0.20, 0.20, 1.0>*0.9]
				   [0.50 rgb <0.20, 0.20, 1.0>*0.9]
				   [0.70 rgb <1,1,1>]
				   [0.85 rgb <0.25,0.25,0.25>]
				   [1.0 rgb <0.5,0.5,0.5>]
			}
			scale<1,1,1.5>*2.5  translate< -1.25,0,0>
		}
		finish {ambient 1 diffuse 0}
	}
	scale 10000
}


// Posicao da canoa
#declare posicao_canoa = transform{rotate<0,30,0> /*rotate<0, 360*clock, 0>*/ translate<0,0,1>}

// Canoa
#declare lado_fora_canoa = sphere{ <0,0,0>,1 scale <3,1.5,1> rotate<0,0,0> translate<0,0.5,0> }

#declare canoa = 
	 union{
		// Construcao da estrutura da canoa
		intersection{
			object{ lado_fora_canoa }
			object{ lado_fora_canoa scale <0.98,0.95,0.94>  inverse}
			sphere {<0,0,0>,1 scale <2.5,1,20> rotate<0,0,0> translate<0,1.3,0>  inverse}
			texture{ 
				 pigment{ 
				 	  color White//White*1.1
				 }
				 finish{ 
				 	 phong 1
				 }
			}
		}
		
		// Interseção entre a estrutura e os pedaços de madeira
		intersection{
			object{ lado_fora_canoa
				texture{ 
					  pigment{
						 color White*1.1
				 	  }
					  finish{
						phong 1
					  }
				}
			}
			// Adicionando madeiras dentro da canoa
			union{
				box {<-0.20,0,-1>,<0.20,0.05,1> rotate<0,0,0> translate< 0.0,0.21,0>}
				box {<-0.20,0,-1>,<0.20,0.05,1> rotate<0,0,-10> translate<-2.0,0.40,0>}
				box {<-0.20,0,-1>,<0.20,0.05,1> rotate<0,0,10> translate< 2.0,0.40,0>}
				texture{
					pigment{
						color MediumWood
				 	}
					finish{
						phong 1
					}
				}
			}
		}
}

object{ canoa transform posicao_canoa}

// Agua
difference{
	plane{<0,1,0>, 0 }
	object{lado_fora_canoa transform posicao_canoa }
	texture{
		Polished_Chrome
		normal{ crackle 1 scale 5 turbulence 1 translate<0,0,5>}
		finish{ diffuse 0.5 reflection 0.30}
	}
}
