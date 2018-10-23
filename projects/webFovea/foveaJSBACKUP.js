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
var region = new ZingTouch.Region(document.getElementById('parent'));
var target = document.getElementById('media');

region.bind(target, 'pan', function(e){
    var canvasRect = target.getBoundingClientRect();
    var x = e.detail.events[0].x - Math.floor(canvasRect.left);
    var y = e.detail.events[0].y - Math.floor(canvasRect.top);
    document.getElementById('output').innerHTML ="( "+ x +" , "+ y +" )";
})

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
    console.log("dx: "+dx+", dy: "+dy+", sx: "+sx+", sy: "+sy);
    // img1 = img[dy:dy+sy, dx:dx+sx]
    let img1 = new cv.Mat();
    let rect = new cv.Rect(dx, dy, sx, sy);
    img1 = img.roi(rect);
    if ( k < niveis ){
	cv.resize(img1, img1, new cv.Size(wx, wy));
    }
    return img1;
}


// Foveated image
fovea.foveatedImage = function( idInput, idOutput, wx, wy, niveis, fx, fy){
    let img = cv.imread(idInput);
    var ux = img.cols; var uy = img.rows;
    cv.cvtColor(img, img, cv.COLOR_RGBA2GRAY, 0);
    let imgFoveated = img.clone();
    let imgLevel;
    for (var k = 0; k < niveis + 1; k++){
	console.log("level "+k);
	imgLevel = this.mmf( img, ux, uy, wx, wy, niveis, k, fx, fy );
	var xi = Math.floor(this.mapLevels2Imagex( ux, wx, niveis, k, fx, 0 ));
	var yi = Math.floor(this.mapLevels2Imagey( uy, wy, niveis, k, fy, 0 ));
	var xf = Math.floor(this.mapLevels2Imagex( ux, wx, niveis, k, fx, wx ));
	var yf = Math.floor(this.mapLevels2Imagey( uy, wy, niveis, k, fy, wy ));
	console.log("xi: "+xi+", yi: "+yi+", xf: "+xf+", yf: "+yf);
	
    	if (k < niveis){
	    cv.resize(imgLevel, imgLevel, new cv.Size(xf - xi, yf - yi));
	    for (let i = 0; i < yf - yi; i++) {
		for (let j = 0; j < xf - xi; j++) {
		    imgFoveated.ucharPtr(i + xi, j + yi)[0] = imgLevel.ucharPtr(i, j)[0];
		}
	    }
	}
	else{
	    for (let i = 0; i < xf - xi; i++) {
		for (let j = 0; j < yf - xi; j++) {
		    imgFoveated.ucharPtr(i + yi, j + xi)[0] = imgLevel.ucharPtr(i, j)[0];
		}
	    }
	}
	//cv.rectangle(imgFoveated, new cv.Point(xi, yi), new cv.Point(xf, yf), [255, 255, 255, 255]);
    }
    cv.imshow(idOutput, imgFoveated);
}
