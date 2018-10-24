// Fovea object
var fovea = {};

// Load functions OpenCV
document.body.classList.add("loading");

// Read Image
let idMedia = document.getElementById('media');
let idInputMedia = document.getElementById('inputMedia');
idInputMedia.addEventListener('change', (e) => { idMedia.src = URL.createObjectURL(e.target.files[0]); }, false);

// Init OpenCV
fovea.initOpencv = function onOpenCvReady() {
  document.body.classList.remove("loading");
}


// Position fovea
/*var region = new ZingTouch.Region(document.getElementById('parent'));
var target = document.getElementById('media');

region.bind(target, 'pan', function(e){
    var canvasRect = target.getBoundingClientRect();
    var x = e.detail.events[0].x - Math.floor(canvasRect.left);
    var y = e.detail.events[0].y - Math.floor(canvasRect.top);
    document.getElementById('output').innerHTML ="( "+ x +" , "+ y +" )";
})*/

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
fovea.mmf = function( img, ux, uy, wx, wy, niveis, k, fx, fy ){
    var dx = Math.floor(this.deltax( ux, wx, niveis, k, fx ));
    var dy = Math.floor(this.deltay( uy, wy, niveis, k, fy ));
    var sx = Math.floor(this.sizex( ux, wx, niveis, k ));
    var sy = Math.floor(this.sizey( uy, wy, niveis, k ));
    //console.log("dx: "+dx+", dy: "+dy+", sx: "+sx+", sy: "+sy);
    // img1 = img[dy:dy+sy, dx:dx+sx]
    let img1 = new cv.Mat();
    let rect = new cv.Rect(dx, dy, sx, sy);
    img1 = img.roi(rect);
    if ( k < niveis ){
	cv.resize(img1, img1, new cv.Size(wx, wy));
    }
    return img1;
}

// Routine otimized of print levels' borders
fovea.printBorder = function( imgInput,  imgOutput, limiteInicial, limiteFinal){
    // North
    /*for (let i = limiteInicial[1]; i < limiteFinal[1]; i++) { // Rows
	for (let j = limiteInicial[0]; j < limiteInicial[2]; j++){ // Cols
	    imgOutput.ucharPtr(i, j)[0] = imgInput.ucharPtr(i - limiteInicial[1], j - limiteInicial[0])[0];
	}
    }
    // South
    for (let i = limiteFinal[3]; i < limiteInicial[3]; i++) { // Rows
	for (let j = limiteInicial[0]; j < limiteInicial[2]; j++) { // Cols
	    imgOutput.ucharPtr(i, j)[0] = imgInput.ucharPtr(i - limiteInicial[1], j - limiteInicial[0])[0];
	}
    }
    // West and East
    for (let i = limiteFinal[1]; i < limiteFinal[3]; i++) { // Rows
	for (let j = limiteInicial[0]; j < limiteInicial[2]; j++) { // Cols
	    if ( j == limiteFinal[0] ) j+= (limiteFinal[2] - limiteFinal[0]);
	    imgOutput.ucharPtr(i, j)[0] = imgInput.ucharPtr(i - limiteInicial[1], j - limiteInicial[0])[0];
	}
    }*/

    for (let j = limiteInicial[0]; j < limiteInicial[2]; j++){ // Cols
	// North
	for (let i = limiteInicial[1]; i < limiteFinal[1]; i++) { // Rows
	    imgOutput.ucharPtr(i, j)[0] = imgInput.ucharPtr(i - limiteInicial[1], j - limiteInicial[0])[0];
	}
	
	// South
	for (let i = limiteFinal[3]; i < limiteInicial[3]; i++) { // Rows
	    imgOutput.ucharPtr(i, j)[0] = imgInput.ucharPtr(i - limiteInicial[1], j - limiteInicial[0])[0];
	}
	
	// West and East
	if ( ( j < limiteFinal[0] ) || ( j > limiteFinal[2] ) ){
	    for (let i = limiteFinal[1]; i < limiteFinal[3]; i++) { // Rows
		imgOutput.ucharPtr(i, j)[0] = imgInput.ucharPtr(i - limiteInicial[1], j - limiteInicial[0])[0];
	    }
	}
    }
}

// Foveated image
fovea.foveatedImage = function( idInput, idOutput, wx, wy, niveis, fx, fy){
    let img = cv.imread(idInput);
    var ux = img.cols; var uy = img.rows;
    cv.cvtColor(img, img, cv.COLOR_RGBA2GRAY, 0);
    let imgFoveated = img.clone();
    let imgLevel;
    for (var k = 0; k < niveis + 1; k++){
	var vectorInit = [];
	var vectorFinish = [];
	console.log("level "+k);
	imgLevel = this.mmf( img, ux, uy, wx, wy, niveis, k, fx, fy );
	var xi = Math.floor(this.mapLevels2Imagex( ux, wx, niveis, k, fx, 0 ));
	var yi = Math.floor(this.mapLevels2Imagey( uy, wy, niveis, k, fy, 0 ));
	var xf = Math.floor(this.mapLevels2Imagex( ux, wx, niveis, k, fx, wx ));
	var yf = Math.floor(this.mapLevels2Imagey( uy, wy, niveis, k, fy, wy ));
	vectorInit = [xi, yi, xf, yf];
	console.log("xi: "+xi+", yi: "+yi+", xf: "+xf+", yf: "+yf);
	
    	if (k < niveis){
	    cv.resize(imgLevel, imgLevel, new cv.Size(xf - xi, yf - yi));
	    var xi2 = Math.floor(this.mapLevels2Imagex( ux, wx, niveis, k+1, fx, 0 ));
	    var yi2 = Math.floor(this.mapLevels2Imagey( uy, wy, niveis, k+1, fy, 0 ));
	    var xf2 = Math.floor(this.mapLevels2Imagex( ux, wx, niveis, k+1, fx, wx ));
	    var yf2 = Math.floor(this.mapLevels2Imagey( uy, wy, niveis, k+1, fy, wy ));
	    vectorFinish = [xi2, yi2, xf2, yf2];
	    if ( k == 4 )
		console.log("entrei aqui");
	    this.printBorder( imgLevel,  imgFoveated, vectorInit, vectorFinish);
	    
	    /*for (let i = 0; i < xf - xi; i++) {
		for (let j = 0; j < yf - yi; j++) {
		    imgFoveated.ucharPtr(i + xi, j + yi)[0] = imgLevel.ucharPtr(i, j)[0];
		}
	    }*/
	}
	/*else{
	    for (let i = 0; i < xf - xi; i++) {
		for (let j = 0; j < yf - yi; j++) {
		    imgFoveated.ucharPtr(i + yi, j + xi)[0] = imgLevel.ucharPtr(i, j)[0];
		}
	    }
	}*/

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
