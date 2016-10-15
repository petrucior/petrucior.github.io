// Fovea object
var fovea = {};

// Load image of web
fovea.loading = function() {
    // Precisa carregar o novo contexto
    var end = document.getElementById("endereco");
};

// Load image in canvas and extract data of image
fovea.getImage = function( contexto ) {
    //var c = document.getElementById("tela");
    //var ctx = c.getContext("2d");
    var img = document.getElementById("imagem");
    contexto.drawImage(img, 0, 0, img.width, img.height);
    return contexto.getImageData(0, 0, img.width, img.height);
};

// Transform image in grayscale
fovea.grayscale = function() {
    var c = document.getElementById("tela");
    var ctx = c.getContext("2d");
    var img = ctx.createImageData(c.width, c.height);
    var contexto = this.getImage(ctx);
    for (var i = 0; i < img.data.length; i += 4) {
	img.data[i+0] = contexto.data[i+0]; //0.2126*contexto.data[i+0] + 0.7152*contexto.data[i+1] + 0.0722*contexto.data[i+2];
	img.data[i+1] = contexto.data[i+0]; //0.2126*contexto.data[i+0] + 0.7152*contexto.data[i+1] + 0.0722*contexto.data[i+2];
	img.data[i+2] = contexto.data[i+0]; //0.2126*contexto.data[i+0] + 0.7152*contexto.data[i+1] + 0.0722*contexto.data[i+2];
	img.data[i+3] = 255;
    }
    ctx.putImageData(img, 0, 0);
};

//
// Convolution
//
/*
 example kernel
 k = [[1, 1, 1], [1, 1, 1], [1, 1, 1]];
*/
fovea.convolution = function(dados, kernel, flag) {
    var soma = 0; var maior = 0; var menor = 1000;
    var resposta = dados.slice();
    var m = Math.floor(kernel[0].length/2);
    // Image
    for ( var rows = m; rows < dados.length - m; rows++ ) {
	for ( var cols = m; cols < dados[0].length - m; cols++ ) {
	    // Filter
	    for ( var rowsf = -m; rowsf < m; rowsf++ ) {
		for ( var colsf = -m; colsf < m; colsf++ ) {
		    soma += dados[rows - rowsf][cols - colsf] * kernel[rowsf + m][colsf + m];
		}
	    }
	    resposta[rows][cols] = soma;
	    if ( soma > maior ){ maior = soma; }
	    if ( soma < menor ){ menor = soma; }
	    soma = 0;
	}
    }
    // Precisa normalizar os valores
    for ( var rows = 0; rows < dados.length; rows++ ){
	for ( var cols = 0; cols < dados[0].length; cols++ ){
	    switch ( flag ){
	    case 0:
		resposta[rows][cols] = 255*( (resposta[rows][cols])/(maior - menor) );
		break;
	    default:
		resposta[rows][cols] = resposta[rows][cols];
		break;
	    }
	}
    }
    return resposta;
};


fovea.extrairDados = function(img, kernel){
    var r = []; //var g = []; var b = []; var alfa = [];
    var rows = 0; var cols = 0;
    for (var i = 0; i < img.data.length; i += 4) {
	if ( i == 0 ){
	    r[rows] = []; //g[rows] = []; b[rows] = []; alfa[rows] = [];
	}
	if ( ( i % (img.width*4) == 0 ) && ( i != 0 ) ){
	    rows++; cols = 0;
	    r[rows] = []; //g[rows] = []; b[rows] = []; alfa[rows] = [];
	}
	/*r[rows][cols] = img.data[i+0]; g[rows][cols] = img.data[i+1];
	b[rows][cols] = img.data[i+2]; alfa[rows][cols] = img.data[i+3];*/
	r[rows][cols] = img.data[i+0];
	cols++;
    }
    return r;
};

// Transform image in red
fovea.red = function() {
    var c = document.getElementById("tela");
    var ctx = c.getContext("2d");
    var img = ctx.createImageData(c.width, c.height);
    var contexto = this.getImage(ctx);
    var dados = this.extrairDados(contexto, 0);
    var rows = -1; var cols = 0;
    for (var i = 0; i < img.data.length; i += 4) {
	if ( i % (img.width*4) == 0 ){ rows++; cols = 0;}
	img.data[i+0] = 255;
	img.data[i+1] = dados[rows][cols];
	img.data[i+2] = dados[rows][cols];
	img.data[i+3] = 255;
	cols++;
    }
    ctx.putImageData(img, 0, 0);
    //document.getElementById("demo").innerHTML = dados[0].length;
};



// Convolution
fovea.teste = function() {
    var c = document.getElementById("tela");
    var ctx = c.getContext("2d");
    var img = ctx.createImageData(c.width, c.height);
    var contexto = this.getImage(ctx);
    var dados = this.extrairDados(contexto, 0);
    //var k = [[1/9, 1/9, 1/9], [1/9, 1/9, 1/9], [1/9, 1/9, 1/9]];
    /*var gaussiana = [[2/115, 4/115, 5/115, 4/115, 2/115],
		     [4/115, 9/115, 12/115, 9/115, 4/115],
		     [5/115, 12/115, 15/115, 12/115, 5/115],
		     [4/115, 9/115, 12/115, 9/115, 4/115],
		     [2/115, 4/115, 5/115, 4/115, 2/115]];
    var resposta = this.convolution(dados, gaussiana, 0);*/
    
    /*var s1 = [[0, 0, -1], [0, 1, 0], [0, 0, 0]];
    var s2 = [[-1, 0, 0], [0, 1, 0], [0, 0, 0]];
    var resposta = this.convolution(dados, s1, 1);
    resposta = this.convolution(resposta, s2, 1);*/
    
    var l = [[1, -2, 1], [-2, 5, -2], [1, -2, 1]];
    var resposta = this.convolution(dados, l, 1);
	
    var rows = -1; var cols = 0;
    for (var i = 0; i < img.data.length; i += 4) {
	if ( i % (img.width*4) == 0 ){ rows++; cols = 0;}
	img.data[i+0] = resposta[rows][cols];
	img.data[i+1] = resposta[rows][cols];
	img.data[i+2] = resposta[rows][cols];
	img.data[i+3] = 255;
	cols++;
    }
    ctx.putImageData(img, 0, 0);
    //document.getElementById("demo").innerHTML = "teste";
};


