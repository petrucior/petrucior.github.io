// Fovea object
var fovea = {};

// Load functions OpenCV
document.body.classList.add("loading");

// Read Image
let idMedia = document.getElementById('media');
let idInputMedia = document.getElementById('inputMedia');
idInputMedia.addEventListener('change', (e) => { idMedia.src = URL.createObjectURL(e.target.files[0]); }, false);

// Check radio
fovea.verifyRadio = function (){
    var radioGrayscale = document.getElementById('radio0').checked;
    var radioLaplacian = document.getElementById('radio1').checked;
    var radioCanny = document.getElementById('radio2').checked;
    if ( radioGrayscale ) return 0;
    if ( radioLaplacian ) return 1;
    if ( radioCanny ) return 2;
}

// Foveation of the image loaded
idMedia.addEventListener('load', function(){
    var radio = fovea.verifyRadio();
    fovea.foveatedImage("media", "canvasMedia", 50, 50, 4, 0, 0, radio);
});

// Init OpenCV
fovea.initOpencv = function onOpenCvReady() {
    document.body.classList.remove("loading");
    var radio = fovea.verifyRadio();
    fovea.foveatedImage("media", "canvasMedia", 50, 50, 4, 0, 0, radio);
}

// Position fovea
idMedia.addEventListener('mousemove', function(event){
    var canvasRect = idMedia.getBoundingClientRect();
    var x = Math.floor(event.offsetX - canvasRect.width/2);
    var y = Math.floor(event.offsetY - canvasRect.height/2);
    //document.getElementById('output').innerHTML ="( "+ x +" , "+ y +" )";
    var radio = fovea.verifyRadio();
    try{
	fovea.foveatedImage("media", "canvasMedia", 50, 50, 4, x, y, radio);
    }
    catch{
	console.log("Can't processing in this moment");
    }
});

// Fovea Structure
// Size
fovea.sizex = function( numberCols, numberColsWx, levels, level ){
    return ((levels * numberCols) + (numberColsWx * level) - (level * numberCols)) / levels;
}
fovea.sizey = function( numberLines, numberLinesWy, levels, level ){
    return ((levels * numberLines) + (numberLinesWy * level) - (level * numberLines)) / levels;
}
// Delta
fovea.deltax = function( numberCols, numberColsWx, levels, level, Fx ){
    return ( level * ( numberCols - numberColsWx + ( 2 * Fx ) ) )/ ( 2 * levels );
}
fovea.deltay = function( numberLines, numberLinesWy, levels, level, Fy ){
    return ( level * ( numberLines - numberLinesWy + ( 2 * Fy ) ) )/ ( 2 * levels );
}

// MapLevels
fovea.mapLevels2Imagex = function( ux, wx, niveis, k, fx, px ){
    return ( (k * wx) * (ux - wx) + (2 * k * wx * fx) + (2 * px) * ( (niveis * ux) - (k * ux) + (k * wx) ) )/ (2 * niveis * wx);
}
fovea.mapLevels2Imagey = function( uy, wy, niveis, k, fy, py ){
    return ( (k * wy) * (uy - wy) + (2 * k * wy * fy) + (2 * py) * ( (niveis * uy) - (k * uy) + (k * wy) ) )/ (2 * niveis * wy);
}

// MMF
fovea.mmf = function( img, ux, uy, wx, wy, niveis, k, fx, fy, modeOperation ){
    let dx = Math.floor(this.deltax( ux, wx, niveis, k, fx ));
    let dy = Math.floor(this.deltay( uy, wy, niveis, k, fy ));
    let sx = Math.floor(this.sizex( ux, wx, niveis, k ));
    let sy = Math.floor(this.sizey( uy, wy, niveis, k ));
    //console.log("dx: "+dx+", dy: "+dy+", sx: "+sx+", sy: "+sy);
    // img1 = img[dy:dy+sy, dx:dx+sx]
    let img1 = new cv.Mat();
    let rect = new cv.Rect(dx, dy, sx, sy);
    img1 = img.roi(rect);
    if ( k < niveis ){
	cv.resize(img1, img1, new cv.Size(wx, wy));
    }
    switch ( modeOperation ){
    case 0:
	// Grayscale
	break;
    case 1:
	// Laplacian
	cv.Laplacian(img1, img1, cv.CV_8U, 1, 1, 0, cv.BORDER_DEFAULT);
	break;
    case 2:
	// Canny
	cv.Canny(img1, img1, 50, 100, 3, false);
	break;
    default:
	alert("Don't do nothing");
	break;
    }
    return img1;
}

// Routine otimized of print levels' borders
fovea.printBorder = function( imgInput,  imgOutput, limiteInicial, limiteFinal){
    for (let j = limiteInicial[0]; j < limiteInicial[2]; j++){ // Cols
	// North and South
	for (let i = limiteInicial[1]; i < limiteInicial[3]; i++){ // Rows
	    if ( i == limiteFinal[1] ) i = limiteFinal[3] - 1;
	    else imgOutput.ucharPtr(i, j)[0] = imgInput.ucharPtr(i - limiteInicial[1], j - limiteInicial[0])[0];
	}
	
	// West and East
	if ( ( j < limiteFinal[0] ) || ( j > limiteFinal[2] ) ){
	    for (let i = limiteFinal[1]; i < limiteFinal[3]; i++){ // Rows
		imgOutput.ucharPtr(i, j)[0] = imgInput.ucharPtr(i - limiteInicial[1], j - limiteInicial[0])[0];
	    }
	}
    }
}

// Foveated image
fovea.foveatedImage = function( idInput, idOutput, wx, wy, niveis, fx, fy, modeOperation){
    let img = cv.imread(idInput);
    var ux = img.cols; var uy = img.rows;
    cv.cvtColor(img, img, cv.COLOR_RGBA2GRAY, 0);
    let imgFoveated = img.clone();
    let imgLevel;
    for (var k = 0; k < niveis + 1; k++){
	var vectorInit = [];
	var vectorFinish = [];
	console.log("level "+k);
	imgLevel = this.mmf( img, ux, uy, wx, wy, niveis, k, fx, fy, modeOperation );
	var xi = Math.floor(this.mapLevels2Imagex( ux, wx, niveis, k, fx, 0 ));
	var yi = Math.floor(this.mapLevels2Imagey( uy, wy, niveis, k, fy, 0 ));
	var xf = Math.floor(this.mapLevels2Imagex( ux, wx, niveis, k, fx, wx ));
	var yf = Math.floor(this.mapLevels2Imagey( uy, wy, niveis, k, fy, wy ));
	vectorInit = [xi, yi, xf, yf];
	//console.log("xi: "+xi+", yi: "+yi+", xf: "+xf+", yf: "+yf);
	
    	if (k < niveis){
	    cv.resize(imgLevel, imgLevel, new cv.Size(xf - xi, yf - yi));
	    let xi2 = Math.floor(this.mapLevels2Imagex( ux, wx, niveis, k+1, fx, 0 ));
	    let yi2 = Math.floor(this.mapLevels2Imagey( uy, wy, niveis, k+1, fy, 0 ));
	    let xf2 = Math.floor(this.mapLevels2Imagex( ux, wx, niveis, k+1, fx, wx ));
	    let yf2 = Math.floor(this.mapLevels2Imagey( uy, wy, niveis, k+1, fy, wy ));
	    vectorFinish = [xi2, yi2, xf2, yf2];
	    //console.log("xi2: "+xi2+", yi2: "+yi2+", xf2: "+xf2+", yf2: "+yf2);
	    var start = new Date();
	    this.printBorder( imgLevel,  imgFoveated, vectorInit, vectorFinish);
	    var duration = (new Date() - start);
	    console.log("duracao: "+duration+" ms");
	}
	else{
	    vectorFinish = [xf, yf, xf, yf];
	    this.printBorder( imgLevel,  imgFoveated, vectorInit, vectorFinish);
	}
	
	// Deleting vectors
	delete vectorInit;
	delete vectorFinish;
	
	//((document.getElementById('canvasMedia')).getContext('2d')).drawImage(imgLevel, xi, yi);
	
	cv.rectangle(imgFoveated, new cv.Point(xi, yi), new cv.Point(xf, yf), [255, 255, 255, 255]);
    }
    
    cv.imshow(idOutput, imgFoveated);
    delete imgFoveated;
    delete imgLevel;
}
