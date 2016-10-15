/**
 * \file basic_forms.pov
 *
 * \brief Arquivo para renderizacao de formas simples com o POV-Ray.
 *
 * \author
 * Petrucio Ricardo Tavares de Medeiros \n
 * Universidade Federal do Rio Grande do Norte \n
 * Departamento de Computacao e Automacao Industrial \n
 * petrucior at gmail (dot) com
 *
 * \version 1.0
 * \date November 2015
 */

// Incluindo arquivos de definicao de cores, formas e texturas diversas
#include "colors.inc"
#include "shapes.inc"
#include "textures.inc"
#include "stones.inc"
#include "glass.inc"
#include "metals.inc"
#include "woods.inc"


// Camera
camera{
	location <0, 2, -5>	// Posicao da camera
	look_at <0, 1, 2>	// Posicao visualizada
}

// Fonte de luz
light_source{
	<0, 1, -2>	// Posicao da fonte de luz
	color White
}

// -----------------------------------------------------
//			CENA
// -----------------------------------------------------

// Fundo
background{
	color Black		// Definindo cor de fundo preta
}

// Plano
plane{
	<0, 1, 0>, -1		// Vetor normal do plano e tendendo ao infinito
	pigment {  		// Estabelecer a cor do objeto
		// Criacao de um xadrez com cores vermelho e azul
		checker color Red, color Blue
	}

	// Propriedades de efeitos para aparência da superfície
	finish {
	       diffuse 0.4
	       ambient 0.2
	       reflection 0.25
	}
}

// Esfera
sphere{
	<-1.5, 0, 3>, 1		// Vetor de posicao da esfera e raio da esfera
	texture{   		// Define a aparencia da esfera
		pigment{   	// Estabelecer a cor do objeto
			White_Marble   // predefinida em textures.inc 
        		scale 0.5      // fator de escala da textura
		}
	}
}

// Caixa
box{
	<0, 0, 0>,  // Canto mais proximo do lado esquerdo
	<2, 0.5, 2>   // Canto mais distante do lado direito
	texture {
		T_Stone25     // Predefinido em stones.inc
		scale 2       // Escala pela mesma quantidade em todas as direcoes
	}
	rotate y*20     // Rotaciona "<0,20,0>"
}

// Cone
cone{
	<0.5, 3, 2.5>, 0.3    // Centro e raio de um lado ( pico )
	<0.5, 0, 2.5>, 1.0    // Centro e raio do outro lado ( base )
	texture {
		pigment{
			color Red	// Cor predefinida em colors.inc
		}
	}
}

// Cilindro
cylinder{
	<-2, 0, 0>,     // Centro de um dos lados
	<-3, 0, 2.5>,     // Centro do outro lado
	0.5            // Raio
	open           // Remover as tampas do cilindro
	texture { 
		pigment{
			// Cor definida atraves do formato rgb + transparencia
			color rgbt<0.7, 0.2, 0.6, 0.2>
		}
	}
}

// Torus
torus{
	0.3, 0.2	// Raio maior e menor, respectivamente
	texture {
		Sapphire_Agate
	}
	rotate <0, 0, 0>	// Rotacionando o torus
	translate <0, 0, -1>	// Translacao do objeto
}

// Construindo um calice atraves de SOR (Surface of Revolution)
#declare calice = sor{
	 8,	// Quantidade de pontos
	 <0.0,  -0.5>,
	 <3.0,   0.0>,
	 <1.0,   0.2>,
	 <0.5,   0.4>,
	 <0.5,   4.0>,
	 <1.0,   5.0>,
	 <3.0,  10.0>,
	 <4.0,  11.0>
	 open	// Abrir lados
	 texture{  // Definicao da textura
		pigment{
			Col_Glass_Dark_Green
		}
	 }
	 translate <6.5, 0, -7>	// Translacao do objeto
	 scale 0.15	    	// Modificando a escala do objeto
}

// Chamando o objeto criado pela superficie de revolucao
calice

// -----------------------------------------------------